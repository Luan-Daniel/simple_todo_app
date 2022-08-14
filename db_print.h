#ifndef DB_PRINT
#define DB_PRINT

#include "db_connect.h"

// Define funcoes para desenhar tabelas, resultados de querys e obter elementos de tabelas.

// Desenha tabelas resultante de uma query
void
_db_print_result(MYSQL* conn){
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	res = mysql_store_result(conn);
	if(!res) {_perr(conn, "_db_print_result"); return;}
	
	for(int i=0; i<res->field_count; i++)
		printf("|%-*.*s\t", 22, 22, res->fields[i].name);
	printf("\n");
	while((row = mysql_fetch_row(res)) != NULL){
		for(int i=0; i<res->field_count; i++)
			printf("| %-*.*s\t", 20, 20, row[i]);
		printf("\n");
	}
	printf("\n");
	mysql_free_result(res);
}

// Retorna string alocada contendo elemento indicado
char*
_db_get_item(MYSQL* conn, unsigned int i, unsigned int j){
	MYSQL_RES *res;
	MYSQL_ROW row;
	res = mysql_store_result(conn);
	if(!res) {_perr(conn, "_db_get_item"); return NULL;}
	if(i>=res->row_count || j>=res->field_count) {mysql_free_result(res); return NULL;}

	char* ret = calloc(1024, sizeof(char));

	unsigned int c=0;
	while((row = mysql_fetch_row(res)) != NULL){
		if(i!=c++) continue;
		if(row[j] == NULL) {free(ret); ret = NULL;}
		else strcpy(ret, row[j]);
	}
	mysql_free_result(res);
	return ret;
}

// Desenha uma tabela
void
db_print_table(MYSQL* conn, char* tabname){
	char query[512];
	sprintf(query, "select * from %s;", tabname);
	printf("[%s]\n", tabname);
	db_query(
		conn, 
		query
	);
	_db_print_result(conn);
}

#endif