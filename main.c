#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "app_todo.h"

int
main(){
	setlocale(LC_ALL, "en_US");
	
	// Coneta ao banco de dados
	MYSQL* conn = db_connect( "localhost", "root", "", "todo_app");
	if(conn == NULL){
		db_close(conn);
		return 1;
	}

	app_main(conn);
	
	db_close(conn);
	return 0;
}

// funcao 	=> funcao independente
// _funcao  => funcao que precida da resposta de outra funcao
// __funcao => funcao ajudante

