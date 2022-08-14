// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
// Criado em agosto de 2022.
//
// - db_insert.h -
// Define funcoes de insercao em tabelas

#ifndef DB_INSERT
#define DB_INSERT

#include <stdlib.h>
#include "db_connect.h"
#include "db_get.h"
#include "c_helper.h"
#include "db_helper.h"

int __insert_error=0;
// 0= noerror
// 1= chave primaria já existe (erro em nivel de aplicação)
// 2= argumento muito grande (erro em nivel de aplicação)

// Funções de inserção
// {safe input}

int
db_insert_into_pessoa(MYSQL *conn, char *nome, char *email){
	// Impede emails muito grandes
	if(strlen(email)>320) {__insert_error=2; return -2;}
	char *safe_nome = __escape_str(nome);
	char *safe_email = __escape_str(email);
	// Checa e impede 2 usuarios com mesmo email
	if(__count_eq(conn, "pessoa", "email", safe_email, 1)) {__insert_error=1; return 0;}

	int id = __get_next_seq(conn, "pessoa", "pessoa_id");

	char query[1024];
	sprintf(
		query,
		"insert into pessoa\n"
		"(pessoa_id, nome, email)\n"
		"values (%d, '%s', '%s');",
		id, safe_nome, safe_email
	);
	db_query( conn, query );

	free(safe_email);
	free(safe_nome);
	__insert_error=0;
	return id;
}

int
db_insert_into_lista(MYSQL *conn, char *nome){
	char *safe_nome = __escape_str(nome);
	// Impede criação de 2 listas com mesmo nome
	if(__count_eq(conn, "lista", "nome", safe_nome, 1)) {__insert_error=1; return 0;}

	int id = __get_next_seq(conn, "lista", "lista_id");

	char query[1024];
	sprintf(
		query,
		"insert into lista\n"
		"(lista_id, nome)\n"
		"values (%d, '%s');",
		id, safe_nome
	);
	db_query( conn, query );

	free(safe_nome);
	__insert_error=0;
	return id;
}

int
db_insert_into_item(MYSQL *conn, char *nome, char *descr, int progresso, int lista_id){
	if(progresso<0 || progresso>100) {__insert_error=2; return -2;}
	char *safe_nome = __escape_str(nome);
	char *safe_descr = __escape_str(descr);
	int id = __get_next_seq(conn, "item", "item_id");
	char query[2048];
	sprintf(
		query,
		"insert into item\n"
		"(item_id, nome, descr, progresso, lista_id)\n"
		"values (%d, '%s', '%s', %d, %d);",
		id, safe_nome, safe_descr, progresso, lista_id
	);
	db_query( conn, query );

	free(safe_nome);
	free(safe_descr);
	__insert_error=0;
	return id;
}

int
db_insert_into_item_pessoa(MYSQL *conn, int item_id, int pessoa_id){
	char query[512];
	sprintf(
		query,
		"insert into item_pessoa\n"
		"(item_id, pessoa_id)\n"
		"values (%d, %d);",
		item_id, pessoa_id
	);
	db_query( conn, query );

	__insert_error=0;
	return 0;
}


#endif