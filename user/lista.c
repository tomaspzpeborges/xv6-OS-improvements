
#include "lista.h"
#include "memory_management.c"

#include "kernel/types.h"
#include "user/user.h"



l_elemento* novo_elemento(const char* valor)
{
	/* aloca memoria para a estrutura lista */
	l_elemento *item = (l_elemento *) _malloc(sizeof(l_elemento));
	printf("malloc l_elemento\n");
	if(item == 0)
		return 0;

	/* aloca memoria para string */
  	item->str = (char *) _malloc((strlen(valor)+1)*sizeof(char));
  	printf("malloc for string\n");

	if(item->str == 0)
	{

		_free(item);
		return 0;
	}

	/* copia valor */
	strcpy(item->str, valor);

	/* item ainda nao tem proximo */
  item->proximo = 0;
  item->anterior = 0;

	return item;
}

lista* lista_nova()
{
	/* cria lista */
	lista *lst = (lista*) _malloc(sizeof(lista));
	printf("malloc for list\n");
	if(lst == 0)
		return 0;

	/* lista esta' vazia */
	lst->inicio = 0;
	lst->fim = 0;
	lst->tamanho = 0;

  return lst;
}


void lista_apaga(lista *lst)
{
	l_elemento *aux;

	if(lst == 0)
		return;

	/* percorre toda a lista e liberta memoria de
	   cada item e respectiva string */
	while(lst->inicio)
	{
		aux = lst->inicio;
		lst->inicio = lst->inicio->proximo;
		_free(aux->str);
		_free(aux);
		printf("freeing string\n");
		printf("freeing l_elemento\n");

	}

	printf("freeing list\n");
	_free(lst);
	return;
}


int lista_tamanho(lista *lst)
{
	if(lst == 0)
		return -1;
	return lst->tamanho;
}


l_elemento* lista_inicio(lista *lst)
{
	if(lst == 0)
		return 0;
	return lst->inicio;
}


l_elemento* lista_fim(lista *lst)
{
	if(lst == 0)
		return 0;
	return lst->fim;
}


l_elemento* lista_elemento(lista *lst, int pos)
{
	int i;
	l_elemento *curr = 0;

	if (lst == 0 || pos < 0 || pos >= lst->tamanho)
		return 0;

	curr = lst->inicio;
	for(i=0; i<pos; i++)
		curr = curr->proximo;

	return curr;
}


l_elemento* lista_insere(lista *lst, const char* valor, l_elemento *pos)
{
	l_elemento *curr = 0;

	if (lst == 0 || valor == 0)
		return 0;

	/* cria novo item */
	curr = novo_elemento(valor);


	if (curr == 0)
        return 0;

	lst->tamanho++;

	/* caso especial: inserir no fim da lista */
	if(pos == 0)
	{

		/* se e' primeiro l_elemento */
		if (lst->inicio == 0)
		{

			lst->inicio = lst->fim = curr;
		}
		else
		{
			curr->anterior = lst->fim;
			lst->fim->proximo = curr;
			lst->fim = curr;
		}

		return curr;
	}

	/* caso especial: inserir no inicio da lista */
	if (pos == lst->inicio)
	{
		curr->proximo = lst->inicio;
		lst->inicio->anterior = curr;
		lst->inicio = curr;
        return curr;
	}

	/* troca apontadores para inserir item */
	curr->anterior = pos->anterior;
	curr->proximo = pos;
	pos->anterior->proximo = curr;
	pos->anterior = curr;

	return curr;
}


l_elemento* lista_remove(lista *lst, l_elemento *pos)
{
	l_elemento *aux;

	if (lst == 0 || pos == 0)
		return 0;

	lst->tamanho--;

	/* troca apontadores para remover item */
	if(pos->anterior != 0)
		pos->anterior->proximo = pos->proximo;
	else /* e' o primeiro */
		lst->inicio = pos->proximo;
	if(pos->proximo != 0)
		pos->proximo->anterior = pos->anterior;
	else /* e' o ultimo */
		lst->fim = pos->anterior;

	/* liberta memoria associada ao item removido */
	aux = pos->proximo;
	_free(pos->str);
	_free(pos);

	return aux;
}

/*
int lista_atribui(lista *lst, l_elemento *pos, const char* str)
{
	if(lst == 0 || pos == 0 || str == 0)
		return -1;

		//realoca memoria para nova string 
	  pos->str = (char *) realloc(pos->str, (strlen(str)+1)*sizeof(char));
		if(pos->str == 0)
			return -1;
		strcpy(pos->str, str);

		return 0;
}
*/

l_elemento* lista_pesquisa(lista *lst, const char* str, int origem)
{
	l_elemento *aux;

	if(lst == 0 || str == 0 || (origem != LISTA_INICIO && origem != LISTA_FIM))
		return 0;

	if(origem == LISTA_INICIO)
	{
		/* pesquisa sequencial: a partir do inicio */
		for (aux = lst->inicio; aux != 0; aux = aux->proximo)
		{
			if (strcmp(aux->str, str) == 0)
			{
				return aux;
			}
		}
		return 0;
	}

	/* pesquisa sequencial: a partir do fim */
	for (aux = lst->fim; aux != 0; aux = aux->anterior)
	{
		if (strcmp(aux->str, str) == 0)
		{
			return aux;
		}
	}
	return 0;
}


int lista_ordena(lista *lst)
{
    l_elemento *next, *curr, *min;
    char *aux;

	if(lst == 0)
		return -1;

	/* lista vazia ja' esta' ordenada */
	if(lst->tamanho == 0)
		return 0;

	/* algoritmo de ordenacao por seleccao */
    for(curr = lst->inicio; curr->proximo != 0; curr=curr->proximo)
    {
		min = curr;
		next = curr->proximo;
		while(next != 0)
		{
            if(strcmp(next->str, min->str) < 0)
            	min = next;
            next = next->proximo;
		}
		/* basta trocar strings */
		if (min != curr)
		{
            aux = curr->str;
	    	curr->str = min->str;
	    	min->str = aux;
		}
    }

	return 0;
}



