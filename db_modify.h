// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
// Criado em agosto de 2022.
//
// - db_modify.h -
// Funcoes para excluir e atualizar tupla,
// tambem para executar arquivo .sql

#ifndef DB_MODIFY
#define DB_MODIFY

#include "c_helper.h"
#include "db_helper.h"
#include "db_get.h"
#include "db_connect.h"
#include "db_insert.h"


// Executa um arquivo .sql no banco de dados
// {não usado}
void
db_exec_file(MYSQL *conn, char *filename){
	if(!filename || !conn) return;
	
	FILE* file 	= fopen(filename, "rb");
	if(!file) {fprintf(stderr, "[!] %s nao pode ser lido\n", filename); return;}
	
	char c, query[1024];
	long qstart=0, qend=0;
	while(1){
		c = fgetc(file);
		if(feof(file)) break;
		if(c==';'){
			qend = ftell(file);
			fseek(file, qstart, SEEK_SET);
			fread(query, 1, (qend-qstart)+1, file);
			__rem_art(query);
			db_query(conn, query);
			memset(query,0,strlen(query));
			fseek(file, qend, SEEK_SET);
			qstart = qend;
		}
	}
	
	fclose(file);
}

// Deleta tuplas de tabelas
// !{safe input}
void
db_delete_from(MYSQL *conn, char *rel, char *pred){
	if(!conn || !rel) return;
	char query[1024];
	if(!pred)
		sprintf(query, "delete from %s;", rel);
	else
		sprintf(query, "delete from %s where %s;", rel, pred);
	db_query(conn, query);
}

// Atualiza colunas de tabelas
// !{safe input}
void
db_update(MYSQL *conn, char *rel, char *atr, char *expr, char *pred){
	if(!conn || !rel || !atr || !expr) return;
	char query[1024];
	if(!pred)
		sprintf(query, "update %s set %s = %s;", rel, atr, expr);
	else
		sprintf(query, "update %s set %s = %s where %s;", rel, atr, expr, pred);
	db_query(conn, query);
}


// Funções de delete complexas
// {safe input}

void
db_delete_item_pessoa(MYSQL *conn, int item_id, int pessoa_id){
	char pred[60];
	sprintf(pred, "item_id = %d and pessoa_id = %d", item_id, pessoa_id);
	db_delete_from(conn, "item_pessoa", pred);
}

void
db_delete_pessoa(MYSQL *conn, int pessoa_id){
	char pred[30];
	sprintf(pred, "pessoa_id = %d", pessoa_id);
	db_delete_from(conn, "item_pessoa", pred);
	db_delete_from(conn, "pessoa", pred);
}

void
db_delete_item(MYSQL *conn, int item_id){
	char pred[30];
	sprintf(pred, "item_id = %d", item_id);
	db_delete_from(conn, "item_pessoa", pred);
	db_delete_from(conn, "item", pred);
}

void
db_delete_lista(MYSQL *conn, int lista_id){
	// Deleta tuplas de item_pessoa
	char query[512];
	sprintf(
		query,
		"select distinct item.item_id as id\n"
		"from item natural join item_pessoa\n"
		"where item.lista_id = %d;",
		lista_id
	);
	db_query(conn, query);
	unsigned int r=0, f=0;
	char ***table = _get_table(conn, &r, &f);
	char pred[60];
	for(unsigned int i=0; i<r; i++){
		memset(pred,0,60);
		sprintf(pred, "item_id = %s", table[i][0]);
		db_delete_from(conn, "item_pessoa", pred);
	}
	__free_table(table, r, f);

	// Deleta tuplas de item
	memset(pred,0,60);
	sprintf(pred, "lista_id = %d", lista_id);
	db_delete_from(conn, "item", pred);

	// Deleta lista
	memset(pred,0,60);
	sprintf(pred, "lista_id = %d", lista_id);
	db_delete_from(conn, "lista", pred);
}

#endif