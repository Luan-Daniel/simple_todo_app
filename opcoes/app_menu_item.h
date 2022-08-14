// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
// Criado em agosto de 2022.
//
// - app_menu_item.h -
// Funções para lidar com entrada do usuario no menu de items da lista

#ifndef APP_MENU_ITEM
#define APP_MENU_ITEM

#include "../c_helper.h"
#include "../db_helper.h"
#include "../db_modify.h"
#include "../db_insert.h"
#include "../db_get.h"
#include "app_menu_pessoa.h"

unsigned int
__app_count_item(MYSQL *conn, int lista_id){
	char pred[64];
	sprintf(pred, "lista_id = %d", lista_id);
	return __count_eq(conn, "item", "item_id", pred, -1);
}

int
__app_select_item(MYSQL *conn, int lista_id){
	if (!__app_count_item(conn, lista_id)){
		printf("[!] Sem itens para selecionar\n");
		return 0;
	}
	int id;
	while(1){
		printf("> ID do item: ");
		char sid[10];
		scanf("%9s", sid);
		fflush(stdin);
		sscanf(sid, "%d", &id);
		if(!sscanf(sid, "%d", &id)){
			printf("[!] ID inexistente ou invalido\n");
			continue;
		};
		char pred[64];
		sprintf(pred, "lista_id = %d and item_id = %s", lista_id, sid);
		if(__count_eq(conn, "item", "item_id", pred, -1)) break;
		printf("[!] ID inexistente ou invalido\n");
	}
	return id;
}

void
__app_novo_item(MYSQL *conn, int lista_id){
	char nome[201];
	printf("> Nome do novo item: ");
	scanf("%[^\n]200s", nome);
	fflush(stdin);
	db_insert_into_item(conn, nome, "", 0, lista_id);
}

void
__app_adicionar_pessoa_item(MYSQL *conn, int lista_id){
	if (!__app_count_pessoa(conn)){
		printf("[!] Sem pessoas para selecionar\n");
		return;
	}
	if (!__app_count_item(conn, lista_id)){
		printf("[!] Sem itens para selecionar\n");
		return;
	}
	int item_id = __app_select_item(conn, lista_id);
	int pessoa_id;
	while(1){
		printf("> Email da pessoa: ");
		char email[301];
		scanf("%[^\n]300s", email);
		fflush(stdin);
		pessoa_id = db_get_pessoa_id(conn, email);
		if(__found_id) break;
		printf("[!] Email inexistente ou invalido\n");
	}
	db_insert_into_item_pessoa(conn, item_id, pessoa_id);
}

void
__app_remover_pessoa_item(MYSQL *conn, int lista_id){
	if (!__app_count_pessoa(conn)){
		printf("[!] Sem pessoas para selecionar\n");
		return;
	}
	if (!__app_count_item(conn, lista_id)){
		printf("[!] Sem itens para selecionar\n");
		return;
	}
	int item_id = __app_select_item(conn, lista_id);
	int pessoa_id;
	while(1){
		printf("> Email da pessoa: ");
		char email[301];
		scanf("%[^\n]300s", email);
		fflush(stdin);
		pessoa_id = db_get_pessoa_id(conn, email);
		if(__found_id) break;
		printf("[!] Email inexistente ou invalido\n");
	}
	db_delete_item_pessoa(conn, item_id, pessoa_id);
}

void
__app_deletar_item(MYSQL *conn, int lista_id){
	if (!__app_count_item(conn, lista_id)){
		printf("[!] Sem itens para selecionar\n");
		return;
	}
	int item_id = __app_select_item(conn, lista_id);
	db_delete_item(conn, item_id);
}

void
__app_editar_nome_item(MYSQL *conn, int lista_id){
	if (!__app_count_item(conn, lista_id)){
		printf("[!] Sem itens para selecionar\n");
		return;
	}
	int item_id = __app_select_item(conn, lista_id);
	
	char nome[201];
	printf("> Novo nome: ");
	scanf("%[^\n]200s", nome);
	fflush(stdin);
	char *safe_nome = __escape_str(nome);
	char s_safe_nome[402];
	sprintf(s_safe_nome, "'%s'", safe_nome);
	free(safe_nome);

	char pred[20];
	sprintf(pred, "item_id = %d", item_id);
	db_update(conn, "item", "nome", s_safe_nome, pred);
}

void
__app_editar_progresso_item(MYSQL *conn, int lista_id){
	if (!__app_count_item(conn, lista_id)){
		printf("[!] Sem itens para selecionar\n");
		return;
	}
	int item_id = __app_select_item(conn, lista_id);
	
	int progr;
	while(1){
		printf("> Progresso: ");
		int r= scanf("%d", &progr);
		fflush(stdin);
		if(r && progr>=0 && progr<=100) break;
	}

	char pred[20];
	sprintf(pred, "item_id = %d", item_id);
	char sprogr[4];
	sprintf(sprogr, "%d", progr);
	db_update(conn, "item", "progresso", sprogr, pred);
}

void
__app_editar_descr_item(MYSQL *conn, int lista_id){
	if (!__app_count_item(conn, lista_id)){
		printf("[!] Sem itens para selecionar\n");
		return;
	}
	int item_id = __app_select_item(conn, lista_id);
	
	char descr[481];
	printf("> Nova descricao: ");
	scanf("%[^\n]480s", descr);
	fflush(stdin);
	char *safe_descr = __escape_str(descr);
	char s_safe_descr[980];
	sprintf(s_safe_descr, "'%s'", safe_descr);
	free(safe_descr);

	char pred[20];
	sprintf(pred, "item_id = %d", item_id);
	db_update(conn, "item", "descr", s_safe_descr, pred);
}

void
__app_escrever_info(MYSQL *conn, int lista_id){
	if (!__app_count_item(conn, lista_id)){
		printf("[!] Sem itens para selecionar\n");
		return;
	}
	int item_id = __app_select_item(conn, lista_id);
	char query[200];
	sprintf(
		query,
		"select nome, progresso, descr\n"
		"from item\n"
		"where item_id = %d;",
		item_id
	);
	db_query(conn, query);
	unsigned int r,f;
	char ***table = _get_table(conn, &r, &f);
	if(!r) {printf("[!] Erro __app_escrever_info query 1\n"); return;}
	printf(
		"\n - %s -\n"
		" Progresso: %s%%\n"
		" Descricao: %s\n",
		table[0][0], table[0][1], table[0][2]
	);
	__free_table(table, r, f);

	sprintf(
		query,
		"select pessoa.nome, pessoa.email\n"
		"from pessoa\n"
		"natural join item_pessoa\n"
		"where item_id = %d;",
		item_id
	);
	db_query(conn, query);
	table = _get_table(conn, &r, &f);
	if(r) printf(" Pessoas Encarregadas:\n");
	for(unsigned int i=0; i<r; i++){
		printf(
			" |- %s\n"
			" |  %s\n",
			table[i][0], table[i][1]
		);
	}
	__free_table(table, r, f);

	printf("\n (Precione qualquer tecla para voltar)");
	_getch();
	printf("\n");
}

// Direciona entrada à respectiva funcionalidade
char
app_select_opt_items(MYSQL *conn, int *lista_id){
	char input= __getkey("ciarqnpdx");
	int item_select;
	
	printf("[%c]\n", input-32);
	switch(input){
		case 'c':{
			__app_novo_item(conn, *lista_id);
			break;
		}
		case 'i':{
			__app_escrever_info(conn, *lista_id);
			break;
		}
		case 'a':{
			__app_adicionar_pessoa_item(conn, *lista_id);
			break;
		}
		case 'r':{
			__app_remover_pessoa_item(conn, *lista_id);
			break;
		}
		case 'x':{
			__app_deletar_item(conn, *lista_id);
			break;
		}
		case 'n':{
			__app_editar_nome_item(conn, *lista_id);
			break;
		}
		case 'p':{
			__app_editar_progresso_item(conn, *lista_id);
			break;
		}
		case 'd':{
			__app_editar_descr_item(conn, *lista_id);
			break;
		}
		case 'q':{
			printf(" Voltando...\n");
			break;
		}
		default:{
			printf("[!] Tecla Invalida...\n");
			return 0;
		}
	}

	return input;
}

#endif