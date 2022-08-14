// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
// Criado em agosto de 2022.
//
// - db_connect.h -
// Funcoes para gerenciar conexao com
// banco de dados;
// E escrever mensagem erro do DB.

#ifndef DB_CONNECT
#define DB_CONNECT

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>


// Print MySQL error
void
_perr(MYSQL* conn, char *msg){
	fprintf(stderr, "[!] %s: %s\n", (msg!=NULL)?msg:"", mysql_error(conn));
}

// Conecta ao banco de dados MySQL
MYSQL*
db_connect(
  char* server,
  char* user,
  char* password,
  char* dbname
  ){
	MYSQL *conn	= mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, password, dbname, 0, NULL, 0)){
		_perr(conn, "db_connect");
		mysql_close(conn);
		return NULL;
	}
	return conn;
}

// Fecha conexão com banco de dados
void
db_close( MYSQL* conn){
	mysql_close(conn);
}


// Envia query para banco de dados
int
db_query(MYSQL *conn, char* query){
	if(!conn)  fprintf(stderr, "[!] db_query: MYSQL *conn == NULL\n");
	if(!query) fprintf(stderr, "[!] db_query: char *query == NULL\n");
	if(!conn || !query) return ~0;

	int ret = mysql_query(conn, query);
	if(ret) _perr(conn, "db_query");
	return ret;
}

#endif