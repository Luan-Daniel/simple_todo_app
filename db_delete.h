#ifndef DB_DELETE
#define DB_DELETE

// Define funcoes que limpam tabelas ou limpam o banco

#include "db_connect.h"

void
db_clear_table(
  MYSQL *conn,
  char	*tabname
  ){
	char query[512];
	sprintf(query, "TRUNCATE TABLE %s;", tabname);
	db_query(
		conn, 
		query
	);
}

void
db_drop_table(
  MYSQL	*conn,
  char 	*tabname
  ){
	char query[512];
	sprintf(query, "DROP TABLE %s;", tabname);
	db_query(
		conn, 
		query
	);
}

#endif