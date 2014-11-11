// INCLUDE ---------------------------
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// END INCLUDE -----------------------
// STRUCTS --------------------------------
typedef struct Arq_invertido{
	char* word;
	bool* docs;
	struct Arq_invertido* prox;
}Arq_invertido;

typedef struct String{
	char* string;
	short int length;
}String;
//----------------------------------------
// Constant -------------------------------
#define BUFFER_SIZE 256

//END Constant ----------------------------

// Var-------------------------------------
Arq_invertido* No_arq;


//------------------------------------------
// Functions -------------------------------------------------------------------

char lower(char a_c){
	//-------------------------------------------------------------
	//Return:
	//			char : a lower case
	//
	//Arguments:
	//			char a_c: letter in lower or upper case
	//
	//Description of functions:
	//			Turn an letter uppercase in lowercase
	//-------------------------------------------------------------

	if(a_c>='A' && a_c<='Z')
		return a_c+32;//ASCII Table
	else
		return a_c;
}//End lower

bool is_letter(char a_c){
	//-------------------------------------------------------------
	//Return:
	//			bool : true = is an char between 'a' and 'z' (in lowercase)
	//					false = is something that isn't between 'a' and 'z' (minusculo)
	//
	//Arguments:
	//			char a_c: letter that you wants to compare
	//
	//Description of function:
	//			Compare if the char 'a_c' is an letter lowercase
	//-------------------------------------------------------------

	if((a_c>='a' && a_c<='z')||(a_c>='A' && a_c<='Z'))
		return true;
	else
		return false;
}//End is_letter()

char* append(char* a_str, char a_c) {
	//-------------------------------------------------------------
	//Return:
	//			char* : Returns the concatenated string;
	//
	//Arguments:
	//			char* a_str: Receives the string as base;
	//			char c: The letter that you wants to concatenate
	//
	//Description of function:
	//			 Add a letter at the end of the string, Example.: cas + a = casa
	//-------------------------------------------------------------

	char * ls_new_str;
	if(a_str=='\0')
		a_str = NULL;

	if(a_str==NULL){// If it's null, allocate a memory space for store the letter and '\0'
		a_str = (char*)malloc(2);
		a_str[0] = a_c;
		a_str[1] = '\0';
	}
	else{
		int i;
		ls_new_str = a_str;// save in temporary pointer
		i = (strlen(ls_new_str))+2;
		a_str = (char*)malloc(i);//Alocate a new memory space
		for(i=0; i< strlen(ls_new_str); i++)
			a_str[i] = ls_new_str[i];
		a_str[i] = a_c;
		i++;
		a_str[i] = '\0';
		//ls_new_str = NULL;
		free(ls_new_str);
	}
	return a_str;
}//End append()


bool insere_word(Arq_invertido** No,char* word, int number_file, int total_file){

	if(*No==NULL){
		(*No) = (Arq_invertido*)malloc(sizeof(Arq_invertido));
		(*No)->docs = (bool*) calloc (total_file,sizeof(bool));

		(*No)->word = word;
		(*No)->docs[number_file]=true;
		(*No)->prox = NULL;
	}else{
		Arq_invertido* aux;
		int cmp=0;
		aux = (*No);
		while(true){
			if(aux->prox!=NULL){
				cmp = strcmp(word,aux->word);
				if(cmp<0){
					Arq_invertido* novo_no;
					novo_no = (Arq_invertido*)malloc(sizeof(Arq_invertido));
					novo_no->docs = (bool*) calloc (total_file,sizeof(bool));
					novo_no->word = word;
					novo_no->docs[number_file]=true;

					novo_no->prox = aux;
					(*No) = novo_no;
					break;
				}else if(cmp==0){
					aux->docs[number_file]=true;
					break;
				}
			}else{
				cmp = strcmp(word,aux->word);
				if(cmp<0){
					Arq_invertido* novo_no;
					novo_no = (Arq_invertido*)malloc(sizeof(Arq_invertido));
					novo_no->docs = (bool*) calloc (total_file,sizeof(bool));
					novo_no->word = word;
					novo_no->docs[number_file]=true;

					novo_no->prox = aux;
					(*No) = novo_no;
					break;
				}else if(cmp==0){
					aux->docs[number_file]=true;
					break;
				}else{
					Arq_invertido* novo_no;
					novo_no = (Arq_invertido*)malloc(sizeof(Arq_invertido));
					novo_no->docs = (bool*) calloc (total_file,sizeof(bool));
					novo_no->word = word;
					novo_no->docs[number_file]=true;
					novo_no->prox = NULL;
					aux->prox = novo_no;
					break;
				}
			}
			aux = aux->prox;
		}
	}
	return true;
}

bool initialize_file(char* file_name, int number_file, int total_file){
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
					insere_word(&No_arq,ls_str, number_file-1,total_file);
				}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
					lc_c = lower(lc_c);
					ls_str = append(ls_str,lc_c);
					insere_word(&No_arq,ls_str,number_file-1,total_file);
				}//End else
			}//End if

		}//End while

		fclose(lf_file);
	}else
		return false;

return true;
}//End initialize_file()

bool* search_list(Arq_invertido* list,char* word){
	short int cmp;
	while(true){
		if(list==NULL)
			break;
		cmp = strcmp(word,list->word);

		if(cmp==0)
			return list->docs;
		else if(cmp<0)
			break;

		list = list->prox;
	}
	return NULL;
}//END search_list()
/*
bool* ocurrence_word(char* a_str,int number_word,int argc){
	bool* result = (bool*) calloc(argc,sizeof(bool));
	while(number_word!=0){

	}
	return true;
}*/

// END Functions --------------------------------------------------------------

int main(int argc, char **argv) {
int i=1;
No_arq = NULL;
	while(i<argc){//Reading the input files
		if(!initialize_file(argv[i],i,argc-1)){
			printf("Error: File %s can't be loaded.", argv[i]);
			exit(-1);
		}//End if
		i++;
	}//End While

char ls_str[BUFFER_SIZE];
short int length;
int end_text,begin_word;
bool* ocorrencia;
bool* aux=NULL;
String* word;

ocorrencia = (bool*)calloc((argc-1),sizeof(bool));
word = (String*)malloc(sizeof(String));
	while(fgets(ls_str,BUFFER_SIZE,stdin)!=NULL){
		end_text=0;
		while(ls_str[end_text]!='\n'){
			begin_word = end_text;
			while(is_letter(ls_str[end_text])){
				end_text++;
			}
			length = end_text-begin_word;
			word->string = (char*)calloc((length),sizeof(char));
			memmove(word->string,ls_str+begin_word,(length));
			word->length = length;

			aux = search_list(No_arq,word->string);
			if(aux!=NULL)
					ocorrencia[i] = aux[i] || ocorrencia[i];


			if(ls_str[end_text]=='\n')
				break;

			free(word->string);
			free(word);
			end_text++;
		}

		for(i=0;i<argc-1;i++)
			if(ocorrencia[i])
				printf("%s ",argv[i+1]);

	}

	return EXIT_SUCCESS;
}
