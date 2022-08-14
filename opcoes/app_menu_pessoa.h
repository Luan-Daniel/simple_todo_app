// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
// Criado em agosto de 2022.
//
// - app_menu_pessoa.h -
// Funções para lidar com entrada do usuario no menu de pessoas

#ifndef APP_MENU_PESSOA
#define APP_MENU_PESSOA

#include "../c_helper.h"
#include "../db_helper.h"
#include "../db_modify.h"
#include "../db_insert.h"

unsigned int
__app_count_pessoa(MYSQL *conn){
	return __count_eq(conn, "pessoa", "pessoa_id", NULL, 0);
}

int
__app_select_pessoa(MYSQL *conn){
	if (!__app_count_pessoa(conn)){
		printf("[!] Sem pessoas para selecionar\n");
		return 0;
	}
	int id;
	while(1){
		printf("> ID da Pessoa: ");
		char sid[10];
		scanf("%9s", sid);
		fflush(stdin);
		sscanf(sid, "%d", &id);
		if(!sscanf(sid, "%d", &id));
		else if(__count_eq(conn, "pessoa", "pessoa_id", sid, 0)) break;
		printf("[!] ID inexistente ou invalido\n");
	}
	return id;
}

void
__app_nova_pessoa(MYSQL *conn){
	char nome[201];
	printf("> Nome da nova pessoa: ");
	scanf("%[^\n]200s", nome);
	fflush(stdin);
	while(1){
		printf("> Email da nova pessoa: ");
		char email[301];
		scanf("%[^\n]300s", email);
		fflush(stdin);
		db_insert_into_pessoa(conn, nome, email);
		if(__insert_error) printf("[!] Email ja existente ou invalido\n");
		else break;
	}
}

void
__app_editar_pessoa(MYSQL *conn){
	if (!__app_count_pessoa(conn)){
		printf("[!] Sem pessoas para editar\n");
		return;
	}
	int id = __app_select_pessoa(conn);

	char nome[201];
	printf("> Novo nome: ");
	scanf("%[^\n]200s", nome);
	fflush(stdin);
	while(1){
		char email[301];
		printf("> Novo email: ");
		scanf("%[^\n]300s", email);
		fflush(stdin);
		
		// Prepara strings
		char *safe_email = __escape_str(email);
		char *safe_nome = __escape_str(nome);
		char s_safe_nome[401];
		char s_safe_email[601];
		sprintf(s_safe_nome, "'%s'", safe_nome);
		sprintf(s_safe_email, "'%s'", safe_email);

		// Checa se email já existe
		if(!__count_eq(conn, "pessoa", "pessoa_id", safe_email, 1)){
			// Atualiza lista
			char pred[30];
			sprintf(pred, "pessoa_id = %d", id);
			db_update(conn, "pessoa", "nome", s_safe_nome, pred);
			db_update(conn, "pessoa", "email", s_safe_email, pred);
			free(safe_email);
			free(safe_nome);
			break;
		}
		printf("[!] Email ja existente ou invalido\n");
		free(safe_email);
		free(safe_nome);
	}
}

void
__app_remover_pessoa(MYSQL *conn){
	if (!__app_count_pessoa(conn)){
		printf("[!] Sem pessoas para remover\n");
		return;
	}
	int id = __app_select_pessoa(conn);
	db_delete_pessoa(conn, id);
}

// Direciona entrada à respectiva funcionalidade
char
app_select_opt_pessoas(MYSQL *conn, int *select){
	char input= __getkey("crxq");
	
	printf("[%c]\n", input-32);
	switch(input){
		case 'c':{
			__app_nova_pessoa(conn);
			break;
		}
		case 'r':{
			__app_editar_pessoa(conn);
			break;
		}
		case 'x':{
			__app_remover_pessoa(conn);
			break;
		}
		case 'q':{
			printf(" Voltando...\n");
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