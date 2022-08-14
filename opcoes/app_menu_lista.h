#ifndef APP_MENU_LISTA
#define APP_MENU_LISTA

// Funções para lidar com entrada do usuario no menu de listas

#include "../c_helper.h"
#include "../db_helper.h"
#include "../db_modify.h"
#include "../db_insert.h"

unsigned int
__app_count_lista(MYSQL *conn){
	return __count_eq(conn, "lista", "lista_id", NULL, 0);
}

int
__app_select_lista(MYSQL *conn){
	if (!__app_count_lista(conn)){
		printf("[!] Sem listas para selecionar\n");
		return 0;
	}
	int id;
	while(1){
		printf("> ID da Lista: ");
		char sid[10];
		scanf("%9s", sid);
		fflush(stdin);
		if(!sscanf(sid, "%d", &id));
		else if(__count_eq(conn, "lista", "lista_id", sid, 0)) break;
		printf("[!] Lista inexistente ou invalida\n");
	}
	return id;
}

void
__app_nova_lista(MYSQL *conn){
	while(1){
		printf("> Nome da nova lista: ");
		char nome[201];
		scanf("%[^\n]200s", nome);
		fflush(stdin);
		db_insert_into_lista(conn, nome);
		if(__insert_error) printf("[!] Nome ja existente ou invalido\n");
		else break;
	}
}

void
__app_renomear_lista(MYSQL *conn){
	if (!__app_count_lista(conn)){
		printf("[!] Sem listas para renomear\n");
		return;
	}
	int id = __app_select_lista(conn);
	while(1){
		printf("> Novo nome da lista: ");
		char nome[201];
		scanf("%[^\n]200s", nome);
		fflush(stdin);
		char *safe_nome = __escape_str(nome);
		if(!__count_eq(conn, "lista", "nome", safe_nome, 1)){
			char s_safe_nome[402];
			sprintf(s_safe_nome, "'%s'", safe_nome);
			char pred[32];
			sprintf(pred, "lista_id = %d", id);
			db_update(conn, "lista", "nome", s_safe_nome, pred);
			free(safe_nome);
			break;
		}
		free(safe_nome);
		printf("[!] Nome ja existente ou invalido\n");
	}
}

void
__app_remover_lista(MYSQL *conn){
	if (!__app_count_lista(conn)){
		printf("[!] Sem listas para remover\n");
		return;
	}
	int id = __app_select_lista(conn);
	db_delete_lista(conn, id);
}

char
app_select_opt_listas(MYSQL *conn, int *select){
	char input= __getkey("acrxpq");
	
	printf("[%c]\n", input-32);
	switch(input){
		case 'a':{
			if(select)
				*select = __app_select_lista(conn);
			break;
		}
		case 'c':{
			__app_nova_lista(conn);
			break;
		}
		case 'r':{
			__app_renomear_lista(conn);
			break;
		}
		case 'x':{
			__app_remover_lista(conn);
			break;
		}
		case 'p':{
			break;
		}
		case 'q':{
			printf(" Deixando programa...\n");
			break;
		}
		default:{
			printf("[!] Tecla Invalida...\n");
			return 0;
		}
	}

	return input;
}

#endif