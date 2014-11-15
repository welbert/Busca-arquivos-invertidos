// INCLUDE ---------------------------
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// END INCLUDE -----------------------
// STRUCTS --------------------------------
typedef unsigned short int ui;

typedef struct Ocorrencia{
	ui indice;
	struct Ocorrencia* prox;
}Ocorrencia;

typedef struct Docs{
	ui file;
	struct Docs* prox;
	Ocorrencia* n_ocorrencia;
	Ocorrencia* last_ocorrencia;
}Docs;

typedef struct Arq_invertido{
	char* word;
	//bool* docs;
	Docs* ocorrencia;
	//TODO add Ocorrencia* ocorrencia and remove bool* docs (Make adaptations)
	struct Arq_invertido* prox;
	struct Arq_invertido* prev;
}Arq_invertido;

typedef struct String{
	char* string;
	ui length;
}String;
//----------------------------------------
// Constant -------------------------------
#define BUFFER_SIZE 256
#define MAX_HASH 50
//END Constant ----------------------------

// Var-------------------------------------
//Arq_invertido* No_arq;
Arq_invertido* Hash[MAX_HASH];

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

void insere_Ocorrencia(Docs** No,ui file,ui indice){
	Ocorrencia* aux;
	Docs* aux_docs;

	if(*No==NULL){
		(*No) = (Docs*) malloc(sizeof(Docs));
		(*No)->file = file;
		(*No)->prox = NULL;


		aux = (Ocorrencia*) malloc(sizeof(Ocorrencia));
		aux->indice = indice;
		aux->prox = NULL;

		(*No)->last_ocorrencia = aux;
		(*No)->n_ocorrencia = aux;
	}else{
		aux_docs = *No;
		bool flag = true;
		while(flag){
			if(aux_docs->file == file){
				aux = (Ocorrencia*) malloc(sizeof(Ocorrencia));
				aux->indice = indice;
				aux->prox = NULL;

				aux_docs->last_ocorrencia->prox = aux;
				aux_docs->last_ocorrencia = aux;
				break;
			}
			if(aux_docs->prox!=NULL)
				aux_docs = aux_docs->prox;
			else
				flag=false;
		}
		if(!flag){
			aux_docs->prox = (Docs*) malloc(sizeof(Docs));
			aux_docs->prox->file = file;
			aux_docs->prox->prox = NULL;


			aux = (Ocorrencia*) malloc(sizeof(Ocorrencia));
			aux->indice = indice;
			aux->prox = NULL;

			aux_docs->prox->last_ocorrencia = aux;
			aux_docs->prox->n_ocorrencia = aux;
		}
	}
}

bool insere_word(Arq_invertido** No,char* word, int number_file, int indice_ocorrencia){

	if(*No==NULL){
		(*No) = (Arq_invertido*)malloc(sizeof(Arq_invertido));
		insere_Ocorrencia(&((*No)->ocorrencia),number_file,indice_ocorrencia);

		(*No)->word = word;
		(*No)->prox = NULL;
		(*No)->prev = NULL;
	}else{
		Arq_invertido* aux=NULL;
		Arq_invertido* novo_no=NULL;
		int cmp=0;
		aux = (*No);
		while(true){
			if(aux->prox!=NULL){
				cmp = strcmp(word,aux->word);
				if(cmp<0){
					novo_no = (Arq_invertido*)malloc(sizeof(Arq_invertido));
					insere_Ocorrencia(&((novo_no)->ocorrencia),number_file,indice_ocorrencia);
					novo_no->word = word;

					novo_no->prox = aux;
					novo_no->prev = aux->prev;
					aux->prev = novo_no;
					if(novo_no->prev != NULL)
						novo_no->prev->prox = novo_no;
					else
						(*No) = novo_no;

					break;
				}else if(cmp==0){
					insere_Ocorrencia(&((aux)->ocorrencia),number_file,indice_ocorrencia);
					break;
				}
			}else{
				cmp = strcmp(word,aux->word);
				if(cmp<0){
					novo_no = (Arq_invertido*)malloc(sizeof(Arq_invertido));
					insere_Ocorrencia(&((novo_no)->ocorrencia),number_file,indice_ocorrencia);
					novo_no->word = word;

					novo_no->prox = aux;
					novo_no->prev = aux->prev;
					aux->prev = novo_no;
					if(novo_no->prev != NULL)
						novo_no->prev->prox = novo_no;
					else
						(*No) = novo_no;
					break;
				}else if(cmp==0){
					insere_Ocorrencia(&((aux)->ocorrencia),number_file,indice_ocorrencia);
					break;
				}else{
					novo_no = (Arq_invertido*)malloc(sizeof(Arq_invertido));
					insere_Ocorrencia(&((novo_no)->ocorrencia),number_file,indice_ocorrencia);
					novo_no->word = word;
					novo_no->prox = NULL;
					novo_no->prev = aux;

					aux->prox = novo_no;
					break;
				}
			}
			aux = aux->prox;
		}
	}
	return true;
}

ui hash_str(char* str){
	int h = str[0];
	h = h%MAX_HASH;
	int i;
	for(i=1;str[i]!='\0';i++)
		h=(h*256+str[i])%MAX_HASH;

	return h;
}

bool insere_hash(char* word, int number_file, int indice_ocorrencia){
	return insere_word(&Hash[hash_str(word)],word,number_file,indice_ocorrencia);
}


bool initialize_file(char* file_name, int number_file, int total_file){
	FILE *lf_file;
	lf_file = fopen(file_name,"r");

	if(lf_file!=NULL){
		char lc_c;
		char* ls_str;
		long int li_file_size;
		int li_indice_ocorrencia = 1;

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
					insere_hash(ls_str, number_file-1,li_indice_ocorrencia);
					li_indice_ocorrencia++;
				}else{//Necessário devido ao final de texto(código exclui a ultima letra por causa do while)
					lc_c = lower(lc_c);
					ls_str = append(ls_str,lc_c);
					insere_hash(ls_str,number_file-1,li_indice_ocorrencia);
					li_indice_ocorrencia++;
				}//End else
			}//End if

		}//End while

		fclose(lf_file);
	}else
		return false;

return true;
}//End initialize_file()

Docs* search_list(Arq_invertido* list,char* word){
	ui cmp;
	while(true){
		if(list==NULL)
			break;
		cmp = strcmp(word,list->word);

		if(cmp==0)
			return list->ocorrencia;//list->docs;
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


Docs* compare_No(Docs* No1 , Docs* No2){// volta a interseção dos Docs*
	Docs* result = NULL;
	Ocorrencia* oc1 = NULL;
	Ocorrencia* oc2 = NULL;
	int cmp;
	while(No1!=NULL && No2!=NULL){
		cmp = No1->file - No2->file;
		if(cmp==0){
			oc1 = No1->n_ocorrencia;
			oc2 = No2->n_ocorrencia;
			while(oc1!=NULL && oc2!=NULL){
				if(oc1->indice +1 == oc2->indice){
					insere_Ocorrencia(&(result),No1->file,oc2->indice);
					oc1 = oc1->prox;
					oc2 = oc2->prox;
				}else if(oc1->indice < oc2->indice){
					oc1 = oc1->prox;
				}else{
					oc2 = oc2->prox;
				}
			}
			No1 = No1->prox;
			No2 = No2->prox;
		}else if(cmp<0)
			No1 = No1->prox;
		else
			No2 = No2->prox;

	}

	return result;
}

// END Functions --------------------------------------------------------------

int main(int argc, char **argv) {
int i=1;
//No_arq = NULL;
memset(Hash,0x0,MAX_HASH);
	while(i<argc){//Reading the input files
		if(!initialize_file(argv[i],i,argc-1)){
			printf("Error: File %s can't be loaded.", argv[i]);
			exit(-1);
		}//End if
		i++;
	}//End While

char ls_str[BUFFER_SIZE];
ui length;
int end_text,begin_word;
Docs* ocorrencia;
Docs* aux;
bool flag;
String* word;

word = (String*)malloc(sizeof(String));
	while(fgets(ls_str,BUFFER_SIZE,stdin)!=NULL){
		ocorrencia = NULL;//(bool*)calloc((argc-1),sizeof(bool));
		aux = NULL;
		end_text=0;
		flag=false;


		while(ls_str[end_text]!='\n'){

			begin_word = end_text;
			while(is_letter(ls_str[end_text])){
				ls_str[end_text] = lower(ls_str[end_text]);
				end_text++;
			}
			length = end_text-begin_word;
			word->string = (char*)calloc((length),sizeof(char));
			memmove(word->string,ls_str+begin_word,(length));
			word->length = length;

			aux = search_list(Hash[hash_str(word->string)],word->string);
			if(aux!=NULL){
				if(flag){
					ocorrencia = compare_No(ocorrencia,aux);
				}else{//Primeira ocorrencia
					ocorrencia = aux;
					flag = true;

				}

			}else{
				ocorrencia = NULL;
				break;
			}

			if(ls_str[end_text]=='\n')
				break;

			free(word->string);
			end_text++;
		}
		/*flag=false;
		for(i=0;i<argc-1;i++)
			if(ocorrencia[i]){
				flag = true;
				printf("%s ",argv[i+1]);
			}
		if(!flag)
			printf("FRASE NAO ENCONTRADA!");
		*/
		if(ocorrencia!=NULL){
			while(ocorrencia!=NULL){
				printf("%s ",argv[ocorrencia->file + 1]);
				ocorrencia = ocorrencia->prox;
			}
		}else
			printf("FRASE NAO ENCONTRADA!");

		printf("\n");
	}
	free(word);

	return EXIT_SUCCESS;
}
