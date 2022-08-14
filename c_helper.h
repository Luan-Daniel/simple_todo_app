// Autor: 	Luan Daniel de Oliveira Melo.
//			(luandanielmelo@gmail.com)
// Criado em agosto de 2022.
//
// - c_helper.h -
// Funções C uteis

#ifndef C_HELPER_H
#define C_HELPER_H

#include <conio.h>
#include <errno.h>

unsigned int
__count_char(char *str, const char ch){
	if(!str) return 0;
	char *c = str;
	unsigned int count=0;
	do{
		if(*c == ch) count++;
	}while(*(c++));
	return count;
}

char*
__escape_str(char *str){
	if(!str) return 0;
	unsigned int n= __count_char(str, '\'');
	unsigned int str_size= strlen(str);
	char *c= str;
	char *safe_str= malloc((str_size+n+1)*sizeof(char));
	if(!safe_str) return NULL;

	unsigned int i=0;
	for(; *c; c++){
		if(*c=='\''){
			safe_str[i++]= '\\';
			safe_str[i++]= '\'';
			continue;
		}
		if(*c=='\\'){
			safe_str[i++]= '\\';
			safe_str[i++]= '\\';
			continue;
		}
		safe_str[i++]= *c;
	}
	safe_str[i]='\0';
	return safe_str;
}

char
__getkey(char *keys){
	char res=0;
	while(1){
		res = _getch();
		if(!res) continue;
		if(65<=res && res<=90) res+=32;
		if(__count_char(keys, res)) break;
	}
	return res;
}

void
__rem_art(char *str){
	if (str==NULL) return;
	unsigned int pv=0, sl=strlen(str);
	for(int i=0; i<sl; i++){
		if(str[i]==';' && i+1<sl){
			memset(str+i+1,0,sl-i);
			break;
		}
	}
}

#endif