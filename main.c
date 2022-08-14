// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
//
// Criado em agosto de 2022.
//
// - Compilador -
// Compilado para Microsoft Windowns usando a ferramenta gcc
// do projeto MinGW-w64 (https://www.mingw-w64.org/) versão 12.1.0.
// Binarios adquiridos em http://www.equation.com/servlet/equation.cmd?fa=fortran.
// 
// - Conexão MySQL -
// Cabeçalhos e binario para C API (libmysqlclient) são inclusos na instalção
// de MySQL 8.0 para Windowns: https://dev.mysql.com/downloads/c-api/
// 
// - Compilação -
// > gcc main.c -llibmysql -o app.exe
//
// - main.c -

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

	// Entra na interface
	app_main(conn);
	
	db_close(conn);
	return 0;
}

// - Sobre comentarios e nomeção de funções -
// funcao 	=> funcao independente
// _funcao  => funcao que precida da resposta de outra funcao
// __funcao => funcao ajudante
// {x} 		=> é x			ex:	| {input safe} função com comportamento
//								| bem definido para todo argumento;
// !{x} 	=> precisa de x		| !{query result} função presisa do
//								| resultado de uma query (tabela) para
//								| funcionar;