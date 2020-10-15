
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
		//free(aux->str); //TODO
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


int main(){



    char *b[3];
    char* str = "Ola";
    b[0] = malloc(sizeof(str));
    strcpy(b[0], str);

    str = "Adeus";
    b[1] = malloc(sizeof(str));
    strcpy(b[1], str);

    str = "Bom dia";
    b[2] = malloc(sizeof(str));
    strcpy(b[2], str);
    int args_size1 = 3;


    commandS *cmds = new_commandS();
    add_to_commandS(cmds, b, args_size1);
    add_to_commandS(cmds, b, args_size1);


    free(b[0]);
    free(b[1]);
    free(b[2]);



    printf("%s\n", cmds->head->args[0]);
    printf("%s\n", cmds->tail->args[1]);

    delete_commandS(cmds);


    exit(0);
}