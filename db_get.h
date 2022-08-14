// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
// Criado em agosto de 2022.
//
// - db_get.h -
// Define funcoes para obter tabelas resultantes
// de querys e obter elementos de dessas tabelas.

#ifndef DB_GET
#define DB_GET

#include "db_connect.h"
#include "c_helper.h"

#ifndef nullptr
#define nullptr (void*)0
#endif

// Retorna string alocada contendo elemento indicado
// {input safe}
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

// Retorna tabela resultante da ultima query
// !{query result}
char***
_get_table(MYSQL *conn, unsigned int *nr, unsigned int *nf){
	if(!nr || !nf) return NULL;
	MYSQL_RES *res;
	res = mysql_store_result(conn);
	if(!res) {_perr(conn, "_get_table"); return NULL;}
	*nr = res->row_count;
	*nf = res->field_count;
	char ***ret= malloc(*nr *sizeof(char**));
	for(unsigned int r=0; r< *nr; r++){
		ret[r]= malloc(*nf*sizeof(char*));
		for(unsigned int f=0; f< *nf; f++){
			ret[r][f]= malloc(1024*sizeof(char));
		}
	}
	
	MYSQL_ROW row;
	unsigned int c=0;
	while((row = mysql_fetch_row(res)) != nullptr){
		for(unsigned int f=0; f< *nf; f++){
			strcpy(ret[c][f], row[f]);
		}
		c++;
	}

	mysql_free_result(res);
	return ret;
}

// Desaloca a tabela
void
__free_table(char ***table, unsigned int r, unsigned int f){
	if(!table) return;
	char ***t=table;
	for(unsigned int i=0; i<r; i++){
		for(unsigned int j=0; j<f; j++) free(table[i][j]);
		free(table[i]);
	}
	free(table);
}

// Encontra id de uma pessoa baseada no email
// {safe email}
int __found_id= 0;
int
db_get_pessoa_id(MYSQL *conn, char *email){
	char *safe_email= __escape_str(email);
	char fquery[1024];
	sprintf(
		fquery,
		"select pessoa_id from pessoa where email='%s';",
		safe_email
	);
	db_query(conn, fquery);
	char* sid = _db_get_item(conn, 0, 0);
	int id;
	if(sid){
		sscanf(sid, "%d", &id);
		free(sid);
		__found_id=1;
	}
	else __found_id=0;
	free(safe_email);
	return id;
}



#endif