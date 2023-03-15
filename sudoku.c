#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>

#define BUFSIZE 1000

int igual,igual2,igual3;

typedef struct 
{
   int linha;
   int sublinha;
   int coluna;
   int subcoluna;
   int **matriz;
} datastruct;

typedef struct {   
    pthread_t thread_id;       
    int thread_num;   
} ThreadData;

void *verifySubGrade(void *matriz){
    
    datastruct * ds;
    ds = (datastruct *) matriz;
    
    for (int i = 0; i < ds->linha; i += ds->sublinha) {
        for (int j = 0; j < ds->coluna; j += ds->subcoluna) {
            for (int k = i; k < i + ds->sublinha; k++) {
                for (int l = j; l < j + ds->subcoluna; l++) {
                    for (int m = i; m < i + ds->sublinha; m++) {
                        for (int o = j; o < j + ds->subcoluna; o++) {
                            if (k != m || l != o) {
                                if (ds->matriz[k][l] == ds->matriz[m][o]) {
                                    igual = 1;
                                    break;
                                }
                            }
                        }    
                    }    
                }     
            }     
        }
    }
    return NULL;

    
}
void *verifyLinha(void *matriz){

    int temp;
    datastruct * ds;
    ds = (datastruct *) matriz;

   

    for (int i = 0; i < ds->linha; i++){
        for (int j = 1; j < ds->coluna; j++){       
            for(int pos = j + 1; pos < ds->coluna; pos++){
                if(ds->matriz[i][j] == ds->matriz[i][pos]){
                    igual2 = 1;
                    break;
                
                }
            }
        }
        
    }
    return NULL;
}
void *verifyColuna(void *matriz){



    int temp;
    datastruct * ds;
    ds = (datastruct *) matriz;
    

    for (int i = 0; i < ds->linha; i++){
        for (int j = 1; j < ds->coluna; j++){       
            for(int pos = i + 1; pos < ds->coluna; pos++){
                if(ds->matriz[i][j] == ds->matriz[pos][j]){
                    igual3 = 1;
                    break;
                
                }
            }
        }
        
    }
    return NULL;
    
}
void printMatriz(void *matriz){

    int temp;
    datastruct * ds;
    ds = (datastruct *) matriz;

    for (int i = 0; i < ds->linha; i++)
    {
        for (int j = 0; j < ds->coluna; j++)
        {
            printf("%d ",ds->matriz[i][j]);
        }
        printf("\n");
    }

}
int ApenasInt(char **ptr) {
    char *string = *ptr;
    int valor = 0;
    while (isdigit(*string)) {
        valor = valor * 10 + (*string - '0'); 									//! VERIFICANDO NUMEROS COM MAIS DE 1 CASA TAMBEM
        string++;
    }
    *ptr = string;
    return valor;
}

int main(int argc, char **argv) {

    igual = 0;
    igual2 = 0;
    igual3 = 0;
    FILE *fp;
    char line[BUFSIZE],line2[BUFSIZE];
    int linhas, colunas,SubLinhas,SubColunas;

    if (argc != 2) {
        printf("Invalid number of parameters\n");
        return 0;
    }

    char buff[BUFSIZE];
    fp = fopen(argv[1], "r");

    if (fp == NULL) {
        printf("Error to read file\n");
        return 0;
    }

    																			// PEGANDO A PRIMEIRA LINHA E ANALISANDO OS TAMANHOS DO SUDOKU
    fgets(line, BUFSIZE, fp);

    char *ptr = line;
    linhas = ApenasInt(&ptr);
    while (*ptr && !isdigit(*ptr)) 
        ptr++; 
    colunas = ApenasInt(&ptr);

    if (colunas != linhas){
            printf("coluna e linha\n");
            return 0;
    }

    																			//PEGA LINHA E COLUNA DA SUBGRADE
    fgets(line2, BUFSIZE, fp);

    char *ptr2 = line2;
    SubLinhas = ApenasInt(&ptr2);
    while (*ptr2 && !isdigit(*ptr2)) 
        ptr2++; 
    SubColunas = ApenasInt(&ptr2);


    if (SubColunas * SubLinhas != colunas){
            printf("subcoluna\n");
            return 0;
    }

    int qntValores = 0;

    datastruct ds = {linhas,SubLinhas,colunas,SubColunas,malloc(sizeof *ds.matriz * ds.linha)};
    
    int valor,i,j;
    for(size_t i = 0; i < ds.linha; i++){
        ds.matriz[i] = malloc(sizeof * ds.matriz[i] * ds.coluna);

        for(size_t j = 0; j < ds.coluna; j++){
            if(fscanf(fp, "%d",&valor) == EOF){
                printf("File out of format.\n");
                return 0;
            }
            if (valor > ds.coluna || valor <= 0){
                
                printf("File out of format\n");
                return 0;
            }
            ds.matriz[i][j] = valor;
            qntValores++;

        }

        
    }   
    if(fgetc(fp) != EOF){

        printf("File out of format\n");
        return 0;
    }
    int sizeGrid = (linhas*colunas)/(SubLinhas*SubColunas);
    ThreadData *threadLinhas = malloc(ds.linha * sizeof(ThreadData));
    ThreadData *threadColunas = malloc(ds.coluna * sizeof(ThreadData)); //!VERIFICAR CORE DUMPED MESMO O CODIGO RODANDO
    ThreadData *threadGrades = malloc(sizeGrid * sizeof(ThreadData)); 

    for(int i = 0; i < linhas; i++){
        threadLinhas[i].thread_num = i;
        pthread_create(&(threadLinhas[i].thread_id),NULL,(void *) verifyLinha,(void *) &ds);
        pthread_create(&(threadColunas[i].thread_id),NULL,(void *) verifyColuna,(void *) &ds);
    }
    int sizeofGrade = (linhas*colunas)/(SubLinhas*SubColunas);

    for(int i = 0; i < sizeofGrade; i++){
        threadGrades[i].thread_num = i;
        pthread_create(&(threadGrades[i].thread_id),NULL,(void *) verifySubGrade,(void *) &ds);
    }


    // if(igual == 1||igual2 == 1 || igual3 == 1)
    //     printf("teste\n");
    
    // verifyColuna(&ds);
    // verifyColuna(&ds);
    // verifySubGrade(&ds);

    // if(igual == 1 || igual2 == 1 || igual3 == 1){
    //     printf("FAIL\n");
    //     return 0;

    // }
    // printf("SUCCESS\n");


    
    printf("%d %d %d\n",igual,igual2,igual3);
    //printf("e\n");
    //printMatriz(&ds);

    for (size_t i = 0; i < ds.linha; i++) {
        free(ds.matriz[i]);
    }
    free(ds.matriz);
    free(threadLinhas);
    free(threadColunas);
    free(threadGrades);
    fclose(fp);
    return 0;
}
