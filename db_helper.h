// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
// Criado em agosto de 2022.
//
// - db_helper.h -
// Define funções uteis

#ifndef DB_HELPER_H
#define DB_HELPER_H

#include "db_connect.h"
#include "db_get.h"

// Pega resultado de busca por maior _id e retorna incremento
// !{resultado de busca por maior _id}
int
_get_new_id(MYSQL* conn){
	unsigned int max_id;
	char* smax_id = _db_get_item(conn, 0, 0);
	if(smax_id){
		sscanf(smax_id, "%d", &max_id);
		free(smax_id);
		return 1+max_id;
	}
	return 1;
}

// Retorna proximo _id
int
__get_next_seq(MYSQL* conn, char* tab, char* atr){
	char query[1024];
	sprintf(query, "select max(%s) from %s;", atr, tab);
	db_query(conn, query);
	return _get_new_id(conn);
}

// val==NULL => Conta quantinade de atr em tab
// val!=NULL => Conta quantinade de atr==val em tab
// isstr==0  => !{non text val}
// isstr==1  => !{safe val}
// isstr==-1  => !{safe val} val é lido como predicado
unsigned int
__count_eq(MYSQL* conn, char* tab, char* atr, char* val, short isstr){
	unsigned int count;
	char query[1024], *f;
	char f1[] = "select count(%s) from %s;";
	char f2[] = "select count(%s) from %s where %s=%s;";
	char f3[] = "select count(%s) from %s where %s='%s';";
	char f4[] = "select count(%s) from %s where %s;";
	if(!val) f=f1;
	else if (isstr == 1) f=f3;
	else if (isstr == -1) f=f4;
	else f=f2;
	if(isstr!=-1)
	sprintf(
		query,
		f,
		atr, tab, atr, val
	);
	else
	sprintf(
		query,
		f,
		atr, tab, val
	);
	db_query(conn, query);
	char* scount = _db_get_item(conn, 0, 0);
	if(scount){
		sscanf(scount, "%d", &count);
		free(scount);
		return count;
	}
	_perr(conn, "__count_eq");
	return 0;
}

#endif