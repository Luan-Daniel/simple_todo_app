#ifndef DB_CONNECT
#define DB_CONNECT

#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>

// Funcoes para gerenciar conexao com banco de dados

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
db_query(MYSQL* conn, char* query){
	int ret = mysql_query(conn, query);
	if(ret) _perr(conn, "db_query");
	return ret;
}

#endif