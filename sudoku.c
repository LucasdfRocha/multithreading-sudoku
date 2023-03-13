#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFSIZE 1000


int verifySubGrade(int col,int lin,int subLin, int subCol, int matriz[][col]){
    
    int igual = 0;
    
    for (int i = 0; i < lin; i += subLin) {
        for (int j = 0; j < col; j += subCol) {
            for (int k = i; k < i + subLin; k++) {
                for (int l = j; l < j + subCol; l++) {
                    for (int m = i; m < i + subLin; m++) {
                        for (int o = j; o < j + subCol; o++) {
                            if (k != m || l != o) {
                                if (matriz[k][l] == matriz[m][o]) {
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
    
    return igual;

    
}
int verifyLinha(int lin, int col, int matriz[][col]){

    int temp;
    int igual = 0;

    for (int i = 0; i < lin; i++){
        for (int j = 1; j < col; j++){       
            for(int pos = j + 1; pos < col; pos++){
                if(matriz[i][j] == matriz[i][pos]){
                    igual = 1;
                    break;
                
                }
            }
        }
        
    }
    return igual;
}
int verifyColuna(int lin, int col, int matriz[][col]){



    int temp;
    int igual = 0;

    for (int i = 0; i < lin; i++){
        for (int j = 1; j < col; j++){       
            for(int pos = i + 1; pos < lin; pos++){
                if(matriz[i][j] == matriz[pos][j]){
                    igual = 1;
                    break;
                
                }
            }
        }
        
    }
    return igual;
    return igual;
}
void printMatriz(int lin, int col, int matriz[][col]){


    for (int i = 0; i < lin; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%d ",matriz[i][j]);
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

    int matriz[linhas][colunas],qntValores = 0;
    
    int valor,i,j;
    for(int i = 0; i < linhas; i++){
        for(int j = 0; j < colunas; j++){
            if(fscanf(fp, "%d",&valor) == EOF){
                printf("File out of format.\n");
                return 0;
            }
            if (valor > colunas || valor <= 0){
                
                printf("File out of format\n");
                return 0;
            }
            matriz[i][j] = valor;
            qntValores++;

        }

        
    }   
    if(fgetc(fp) != EOF){

        printf("File out of format\n");
        return 0;
    }

    if(verifyColuna(linhas,colunas,matriz) == 1|| verifyLinha(linhas,colunas,matriz) == 1 || verifySubGrade(colunas,linhas,SubLinhas,SubColunas,matriz) == 1){

        //printMatriz(linhas,colunas,matriz);
        printf("nao é\n");
        return 0;
    }
    printf("e\n");
     
    fclose(fp);
    return 0;
}
