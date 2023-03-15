#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
#include <pthread.h>
#include <stdbool.h>

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

void *verifySubGrade(void *matriz){
    
    datastruct * ds;
    ds = (datastruct *) matriz;

    int verificaIgual[ds->linha + 1];
    for (int i = 0; i < ds->linha; i += ds->sublinha) {
        for (int j = 0; j < ds->coluna; j += ds->subcoluna) {
            
            memset(verificaIgual,0,sizeof(verificaIgual));
            
            for (int k = i; k < i+ds->sublinha; k++) {
                for (int l = j; l < j+ds->subcoluna; l++) {
                    
                    int temp = ds->matriz[k][l];
                    if (verificaIgual[temp]) 
                        igual = 1;
                    
                    verificaIgual[temp] = 1;
                }
            }
        }
        return NULL;
}
    
}
void *verifyLinha(void *matriz){

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
    FILE *fp,*fp2;
    char line[BUFSIZE],line2[BUFSIZE];
    int linhas, colunas,SubLinhas,SubColunas;

    if (argc != 2) {
        printf("Invalid number of parameters\n");
        return 0;
    }

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
    
    int valor;
    for(int i = 0; i < ds.linha; i++){
        ds.matriz[i] = malloc(sizeof * ds.matriz[i] * ds.coluna);

        for(int j = 0; j < ds.coluna; j++){
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

    int amount = 0;
    int tamanho = linhas;
    pthread_t *threads = malloc(tamanho * sizeof(pthread_t));

    for(int i = 0; i < tamanho; i++){
        
        pthread_create(&(threads[i]),NULL,(void *) verifyLinha,(void *) &ds);
        pthread_create(&(threads[i]),NULL,(void *) verifyColuna,(void *) &ds);
        pthread_create(&(threads[i]), NULL, (void *) verifySubGrade, (void *) &ds);

        amount += 3;
    }   

    for (int i = 0; i < linhas; i++) { 
        pthread_join(threads[i], NULL);
    }

    fp2 = fopen("sudoku_ldfr.out","w");

    if(igual == 1 || igual2 == 1 || igual3 == 1){
        fprintf(fp2, "FAIL");
        printf("\n%d\n", amount);
        return 0;

    }
    fprintf(fp2, "SUCCESS");

    //printMatriz(&ds);

    for (int i = 0; i < ds.linha; i++) {
        free(ds.matriz[i]);
    }
    free(ds.matriz);

    fclose(fp2);
    fclose(fp);
    return 0;
}
