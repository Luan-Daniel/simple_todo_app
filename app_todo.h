#ifndef APP_TODO
#define APP_TODO

// Funções que escrevem as listas e menus

#include "db_todo.h"
#include "c_helper.h"
#include "opcoes\app_menu_lista.h"
#include "opcoes\app_menu_pessoa.h"
#include "opcoes\app_menu_item.h"

char
app_escrever_listas(MYSQL *conn, int *select){
	// Pega informações da tabela no banco de dados
	unsigned int nlista= __count_eq(conn, "lista", "lista_id", NULL, 0);
	db_query(conn, "select lista_id, nome from lista order by lista_id;");
	unsigned int r,f;
	char ***table= _get_table(conn, &r, &f);

	// Escreve tabela
	printf(
		"\n - Listas -\n"
		"|ID\t|Nome\n"
		"|\t|\n"
	);
	for(unsigned int i=0; i<nlista; i++){
		char *id= table[i][0];
		char *nome= table[i][1];
		printf("| %s\t| %s\n", id, nome);
	}
	printf(
		"\n - Opcoes:\n"
		" [A] Abrir\t[C] Nova Lista\n"
		" [R] Renomear\t[X] Deletar\n"
		" [P] Pessoas\t[Q] Sair\n"
	);

	__free_table(table, r, f);
	return app_select_opt_listas(conn, select);
}

char
app_escrever_pessoas(MYSQL *conn, int *select){
	// Pega informações da tabela no banco de dados
	unsigned int nlista= __count_eq(conn, "pessoa", "pessoa_id", NULL, 0);
	db_query(conn, "select pessoa_id, nome, email from pessoa order by pessoa_id;");
	unsigned int r,f;
	char ***table= _get_table(conn, &r, &f);

	// Escreve tabela
	printf(
		"\n - Pessoas -\n"
		"|ID\t|Info\n"
		"|\t|\n"
	);
	for(unsigned int i=0; i<nlista; i++){
		char *id= table[i][0];
		char *nome= table[i][1];
		char *email= table[i][2];
		printf(
			"| %s\t| Nome:  %s\n"
			"|\t| Email: %s\n",
			id, nome, email
		);
	}
	printf(
		"\n - Opcoes:\n"
		" [C] Registrar\t[R] Editar\n"
		" [X] Deletar\t[Q] Voltar\n"
	);

	__free_table(table, r, f);
	return app_select_opt_pessoas(conn, NULL);
}

char
app_escrever_itens(MYSQL *conn, int *select){
	// Pega nome da lista no banco de dados
	char query[200];
	sprintf(query, "select nome from lista where lista_id = %d", *select);
	db_query(conn, query);
	char *_nome = _db_get_item(conn, 0, 0);

	// Conta numero de itens na lista
	char pred[64];
	sprintf(pred, "lista_id = %d", *select);
	unsigned int nitens= __count_eq(conn, "item", "item_id", pred, -1);

	// Pega tabela
	memset(query, 0, 150);
	sprintf(
		query,
		"select item_id, nome, progresso\n"
		"from item where lista_id = %d\n"
		"order by item_id;",
		*select
	);
	db_query(conn, query);
	unsigned int r,f;
	char ***table= _get_table(conn, &r, &f);

	// Escreve tabela
	printf(
		"\n - %s -\n"
		"|ID\t|Nome\n"
		"|\t|\n",
		_nome
	);
	for(unsigned int i=0; i<nitens; i++){
		char *id= table[i][0];
		char *nome= table[i][1];
		char *progresso= table[i][2];
		printf(
			"| %s\t| %s\n"
			"|\t| %s%%\n",
			id, nome, progresso
		);
	}
	printf(
		"\n - Opcoes:\n"
		" [C] Criar\t[N] Editar Nome\n"
		" [I] Info\t[P] Editar Progresso\n"
		" [A] AddPessoa\t[D] Editar Descricao\n"
		" [R] RemPessoa\t[X] Deletar\n"
		" [Q] Voltar\n"
	);

	free(_nome);
	__free_table(table, r, f);
	
	return app_select_opt_items(conn, select);
}

char
app_main(MYSQL *conn){
	int pos=0;
	int select=0;
	char out=0;
	while(pos!=-1)
	switch (pos){
		case 0:{
			out= app_escrever_listas(conn, &select);
			if(out=='q') pos=-1;
			else if(out=='p') pos=1;
			else if(out=='a') pos=2;
			break;
		}
		case 1:{
			out = app_escrever_pessoas(conn, &select);
			if(out=='q') pos=0;
			break;
		}
		case 2:{
			out = app_escrever_itens(conn, &select);
			if(out=='q') pos=0;
			break;
		}
	}
	
	return out;
}

#endif