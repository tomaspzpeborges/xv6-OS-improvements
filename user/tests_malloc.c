#include "lista.c"

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"



int main(int argc, char *argv[]) {


    lista *lst = lista_nova();
    //lista *lst2 = lista_nova();

    //STILL HAVEN'T TESTED JUST FOR lista_insere()
    //lista_insere(lst, "Joana", 0);
    //lista_insere(lst, "Arménio", 0);
    //lista_insere(lst, "Rufus", 0);

    //printf("sizeof(lst): %d\n ", sizeof(*lst)); //gives 24 in both malloc() and _malloc()
    //printf("sizeof(lista *): %d\n ", sizeof(lista)); 
    //printf("sizeof(lst2): %d\n ", sizeof(*lst2)); //gives 24 in both malloc() and _malloc()

	//l_elemento *aux;

	//for (aux = lst->inicio; aux != 0; aux = aux->proximo)
	//{
    //        printf("string: %s\n", aux->str);
	//}
    


    //int *p = (int*)  _malloc(200* sizeof(int));
    //*p= 2;
    //*(p+1) = 5;
    //*(p+199) = 10;


    //printf("%d\n", *(p+199)); 
    //_free(p);


    char* c = (char*) _malloc(150000);
    *c = 'a';
    *(c+5) = 'f';

    _free(c);

    int *p = (int*)  _malloc(200* sizeof(int));
    *p= 2;
    *(p+1) = 5;
    *(p+199) = 10;


    printf("%d\n", *(p+199)); 

    _free(p);
    lista_apaga(lst);


    //lista_apaga(lst2);


    exit(0);

}