// INCLUDE ---------------------------
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// END INCLUDE -----------------------

// Constant -------------------------------
#define BUFFER_SIZE 256

//END Constant ----------------------------

// Functions -------------------------------------------------------------------

char lower(char a_c){
	//-------------------------------------------------------------
	//Retorno:
	//			char : a letra minuscula
	//
	//Argumentos:
	//			char a_c: letra que se deseja torna minuscula
	//
	//Descrição da função:
	//			Tornar uma letra maiuscula em minuscula
	//-------------------------------------------------------------

	if(a_c>='A' && a_c<='Z')
		return a_c+32;//Tabela ASCII
	else
		return a_c;
}//End lower

bool is_letter(char a_c){
	//-------------------------------------------------------------
	//Retorno:
	//			bool : true = é um char entre 'a' e 'z'(minusculo)
	//					false = é algo que não é entre 'a' e 'z' (minusculo)
	//
	//Argumentos:
	//			char a_c: letra que se deseja comparar
	//
	//Descrição da função:
	//			verificar se o 'char c' é uma letra minuscula(a-z)
	//-------------------------------------------------------------

	if((a_c>='a' && a_c<='z')||(a_c>='A' && a_c<='Z'))
		return true;
	else
		return false;
}//End is_letter()

char* append(char* a_str, char a_c) {
	//-------------------------------------------------------------
	//Retorno:
	//			char* : Retorna a string concatenada;
	//
	//Argumentos:
	//			char* a_str: Recebe a string como base;
	//			char c: A letra que deseja-se concatenar
	//
	//Descrição da função:
	//			Adiciona uma letra ao final da string, Ex.: cas + a = casa
	//-------------------------------------------------------------

	char * ls_new_str;
	if(a_str=='\0')
		a_str = NULL;

	if(a_str==NULL){// Se for nulo, aloca um espaço de memoria pra armazenar a letra e o '\0'
		a_str = (char*)malloc(2);
		a_str[0] = a_c;
		a_str[1] = '\0';
	}
	else{
		int i;
		ls_new_str = a_str;// salva em um ponteiro temporario
		i = (strlen(ls_new_str))+2;//testar com realloc
		a_str = (char*)malloc(i);//Aloca um novo espaço do tamanho da nova palavra
		for(i=0; i< strlen(ls_new_str); i++)
			a_str[i] = ls_new_str[i];
		a_str[i] = a_c;
		i++;
		a_str[i] = '\0';
		//ls_new_str = NULL; //TODO ver se no debug se ele limpa a memória certa
		free(ls_new_str);
	}
	return a_str;
}//End append()


bool initialize_file(char* file_name){
	FILE *lf_file;
	lf_file = fopen(file_name,"r");

	if(lf_file!=NULL){
		char lc_c;
		char* ls_str;
		long int li_file_size;

		fseek(lf_file, 0L, SEEK_END);//deslocar o curso para o fim para poder pegar seu tamanho maximo
		li_file_size = ftell(lf_file);//pegar o tamanho do arquivo
		fseek(lf_file,0,SEEK_SET);//setar o cursor do arquivo para o ��nicio

		lc_c=' ';
		while(lc_c!=EOF && ftell(lf_file) < li_file_size){
			ls_str=NULL;
			fscanf(lf_file,"%c",&lc_c);

			while (is_letter(lc_c) && ftell(lf_file) < li_file_size){
				lc_c = lower(lc_c);
				ls_str = append(ls_str,lc_c);
				fscanf(lf_file,"%c",&lc_c);
			}//End while

			if(ls_str!=NULL){
				if(!is_letter(lc_c)){
					//TODO ver o que tenho que fazer com a palavra (ls_str)
				}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
					lc_c = lower(lc_c);
					ls_str = append(ls_str,lc_c);
					//TODO ver o que tenho que fazer com a palavra (ls_str)
				}//End else
			}//End if

		}//End while

		fclose(lf_file);
	}else
		return false;

return true;
}//End initialize_file()

// END Functions --------------------------------------------------------------

int main(int argc, char **argv) {
int i=1;

	while(i<argc){//Reading the input files
		if(!initialize_file(argv[i])){
			printf("Error: File %s can't be loaded.", argv[i]);
			exit(-1);
		}//End if
	}//End While

char ls_str[BUFFER_SIZE];
int end_word,begin_word;

	while(fgets(ls_str,BUFFER_SIZE,stdin)!=NULL){
		end_word=0;
		while(ls_str[end_word]!='\n'){
			begin_word = end_word;
			while(is_letter(ls_str[end_word])){
				end_word++;
			}

			end_word++;
		}

	}

	return EXIT_SUCCESS;
}
