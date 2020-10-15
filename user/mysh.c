
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"


//Using a list to handle command line input
typedef struct _command
{
	
	char *args[20]; //array of arguments
    int args_size;
    struct _command *next;
    struct _command *previous;
} command;

typedef struct _commandS
{
	command *head;
	command *tail;
	int size;
} commandS;

commandS* new_commandS()
{
	//create new list of commandS
	commandS *cmds = (commandS*) malloc(sizeof(commandS));
	if(cmds == 0)
		return 0;

	//empty
	cmds->head = 0;
	cmds->tail = 0;
	cmds->size = 0;

  return cmds;
}

void delete_commandS(commandS *cmds)
{
	command *aux;

	if(cmds == 0)
		return;

	/* percorre toda a lista e liberta memoria de
	   cada item e respectiva string */
	while(cmds->head)
	{
		aux = cmds->head;
		cmds->head = cmds->head->next;
        for(int i = 0; i <aux->args_size; i++){

            free(aux->args[i]);

        }
		free(aux);
	}

	free(cmds);

	return;
}

command* new_command(char ** args, int args_size){

	
	command *cmd = (command *) malloc(sizeof(command));

	if(cmd == 0){
		return 0;
    }

	
    //cmd->args = args;
    cmd->args_size = args_size;

    for(int i =0; i < args_size; i++) {

        cmd->args[i] = (char*) malloc(sizeof(args[i]));
        strcpy(cmd->args[i], args[i]);
    }

	
    cmd->next = 0;
    cmd->previous = 0;

	return cmd;
}

void add_to_commandS(commandS *cmdS, char ** args, int args_size ){

	command *new_cmd = new_command(args, args_size);

	cmdS->size++;

	
	if (cmdS->head == 0)
	{
		cmdS->head = cmdS->tail = new_cmd;
	}
	else
	{
		new_cmd->previous = cmdS->tail;
		cmdS->tail->next = new_cmd;
		cmdS->tail = new_cmd;
	}
} 

void run_comd(command* cmd){

    int p[2];
  
    if (cmd->next){
        if(strcmp(cmd->next->args[0], "|") == 0){

            run_comd(cmd->next);

        }
        if(strcmp(cmd->next->args[0], ">") == 0){

            run_comd(cmd->next);

        }
        if(strcmp(cmd->next->args[0], "<") == 0){

            run_comd(cmd->next);

        }

    }else{ //if it is just one command execute it

        //execute a normal command
        if(cmd->args[0] == 0) exit(1);
        exec(cmd->args[0], cmd->args);
        fprintf(2, "exec %s failed\n", cmd->args[0]);
    }


    if(strcmp(cmd->args[0], "|") == 0){

        pipe(p);


        if(fork() == 0){
            close(1);
            dup(p[1]);
            close(p[0]);
            //run_comd(pcmd->left);
            if(cmd->previous){
                exec(cmd->previous->args[0], cmd->previous->args ); //left process writes to the pipe 
                //run_comd(cmd->previous);
            }
            close(p[1]);

        }
        if(fork() == 0){
            close(0);
            dup(p[0]); //cmd->next will receive input from pipe instead of stdin
            close(p[0]);
            close(p[1]);
            //run_comd(pcmd->right);
            if (cmd->next){
                //exec(cmd->next->args[0], cmd->next->args );
                run_comd(cmd->next);
            }

        }
        close(p[0]);
        close(p[1]);
        wait(0);
        wait(0);
    }


    if (strcmp(cmd->args[0], ">") == 0){

        if( (cmd->next) && (cmd->previous) ){

			close(1);
            if(open(cmd->next->args[0], O_WRONLY) < 0){
                fprintf(2, "open %s failed\n", cmd->next->args[0]);
                exit(1);
            }
            
            exec(cmd->previous->args[0], cmd->previous->args);
        }else{

            fprintf(2, "failed\n", cmd->next->args[0]);

        }
  
	}

    if (strcmp(cmd->args[0], "<") == 0){

        if( (cmd->next) && (cmd->previous) ){

            close(0);

            if(open(cmd->next->args[0], O_RDONLY) < 0){
                fprintf(2, "open %s failed\n", cmd->next->args[0]);
                exit(1);
            }
                
            exec(cmd->previous->args[0], cmd->previous->args);

        }else{

            fprintf(2, "failed\n", cmd->next->args[0]);

        }
		
	}



}

void args_to_cmds(char **strs, int gen_count, commandS *list_cmds){

    char* cmds[20];//arguments in one command, remember max args is 20
    int args_size = 0;

    //Separating all the actual commands by special characters <, >, |
    //adding them to the list of commands
    for(int i =0; i <gen_count; i++){
        //printf("%sB\n", strs[i]);
        if( (strcmp(strs[i], ">") ==0) || (strcmp(strs[i], "<") ==0) || (strcmp(strs[i], "|") ==0) ) {
          
            //adding the full command previous to the special sign
            add_to_commandS(list_cmds, cmds, args_size);

            //adding a command just for the special sign
            cmds[0] = strs[i]; //i know there can be more arguments in cmds but args_size avoids wrong accessing when adding to the list
            args_size=1;
            add_to_commandS(list_cmds, cmds, args_size);

            //new command next, start writing from index 0 again
            args_size =0;

        }else{
            args_size++;
            cmds[args_size-1] = strs[i];

            //if we reached the end we have to write this last command to the list
            if(i == gen_count-1) {
                
                add_to_commandS(list_cmds, cmds, args_size);
                continue;
            }            


        }
    
    }

    //So now I have a list with the commands properly separated

}

int parse_cmd(char* input, commandS* list_cmds ){


    int gen_count = 0; //counts general number of arguments
    char buff[25] = "";
    int count = 0; //counts number of chars in 1 argument
    char *strs[20];//max 20 arguments, every argument is dinamically allocated and needs to be freed


    //SEPARATING ALL THE ARGUMENTS TO AN ARRAY OF STRINGS
    for(int i =0; i <strlen(input); i++){
        
        //replace (i==(strlen(input)-1) with (input[i] == ';') ?
        if ( (input[i] == ' ') || (input[i] == ';') || (input[i] == '\n') || (input[i] == '\t') ){



            if( strcmp(buff, "") != 0 ) { //avoiding saving whitespaces if by mistake there is one at the end

                //copying word from buffer to array
                gen_count++;
                if(gen_count > 20) {
                    return -1;
                }
                strs[gen_count-1] = (char*) malloc(sizeof(buff));
                strcpy(strs[gen_count-1], buff);

                //resetting buffer
                count = 0;
                memset(buff,0,sizeof(buff));

            }

        }
        else{

            count++;
            buff[count-1] = input[i];

            if(count == 24) { //word has to end, because buff[25], due to some ambiguity that may come from the offset at end and whitespaces,, i'm limitng it to 24chars
                return -1;
            }
        }

    }

    //safety lock
    if(gen_count == 0) {
        return 0;
    }else{

        args_to_cmds(strs, gen_count, list_cmds);

    }


    for(int i =0; i <gen_count; i++){

        free(strs[i]); //avoiding memory leaks!
    }
    

    return gen_count;


}

int get_cmd(char *buffer, int nbuffer) {
  
  fprintf(2, ">>> ");
  memset(buffer, 0, nbuffer);
  gets(buffer, nbuffer);
  if(buffer[0] == 0) // EOF
    return -1;
  if(strcmp(buffer, "exit\n") == 0) return -1; //IMPLEMENTING AN EXIT OF THE SHELL 
  return 0;

}

int main(int argc, char *argv[]) {

    printf("TZB shell\n");

    int fd;
    static char buffer[100];//buffer for the command line input
    static char commands_buffer[100][100]; //buffer for command line input, separated by ;

    int num_commands = 0;//num of commands inside commands_buffer
    int index = 0; //auxiliar index for populating commands_buffer

    int gen_count = 0; //general number of arguments, per command line separated by ;


    // Ensure that three file descriptors are open.
    while((fd = open("console", O_RDWR)) >= 0){
        if(fd >= 3){
        close(fd);
        break;
        }
    }

    // Read and run input commands.
    while(get_cmd(buffer, sizeof(buffer)) >= 0){
        
        //populating commands_buffer, command line input, separated by ;
        num_commands = 0;
        index = 0; 
        for(int i =0; i <strlen(buffer); i++){

            if(index == 0) {
                num_commands++;
            }
            commands_buffer[num_commands-1][index] = buffer[i];
            index++;
            if ( (buffer[i] == ';')  ){
                index =0;
            }

        }

       //printf("Number of commands: %d\n",num_commands);    

        //run commands separated by ;
        for(int i = 0; i < num_commands; i++){

            /*
            If cd were run as a regular command, then the shell would fork a child process, the child process would run cd, and cd would change the child ’s working directory. 
            The parent’s (i.e., the shell’s) working directory would not change.
            */
            if(commands_buffer[i][0] == 'c' && commands_buffer[i][1] == 'd' && commands_buffer[i][2] == ' '){
            // Chdir must be called by the parent, not the child.
            commands_buffer[i][strlen(commands_buffer[i])-1] = 0;  // chop \n                                                                        
            if(chdir(commands_buffer[i]+3) < 0)
                fprintf(2, "cannot cd %s\n", commands_buffer[i]+3);
            continue;
            } 


            //calling fork, which creates a copy of the shell process. The parent calls wait, while the child runs the command
            if(fork() == 0){

                //populating list, separating by special signs
                commandS *list_cmds = new_commandS();
                gen_count = parse_cmd(commands_buffer[i], list_cmds);

                if(gen_count == 0){
                    exit(0);
                }
                else if(gen_count == -1){
                    printf("Can't read single arguments with more than 25 characters. and/or can't only read 20 arguments\n");
                }
                else{

                    //running the commands from the list
                    run_comd(list_cmds->head);
                    delete_commandS(list_cmds);

                }

                exit(0);
            }else{

                wait(0);
                memset(commands_buffer[i], 0, sizeof(commands_buffer[i])); //safely freeing the buffer for the next command line input

                //printf("gen_count parent: %d\n", gen_count);//parent process doesn't get affected !!!

            }

        }

    }

    exit(0);
}