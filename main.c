#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void freeBoard(int** board, int lines){
    int i;
    for(i = 0; i < lines; i++){
        free(board[i]);
    }
    free(board);
}

int** makeBoard(){
    int **board;
    int i;
    board = (int**) malloc(sizeof(int*)*1);
    if (board == NULL){
        printf("Falha na alocacao!\n");
        exit(0);
    }else{
        board[0] = (int*) malloc(sizeof(int)*1);
        if(board[0] == NULL){
            printf("Falha na alocacao!\n");
            exit(0);
        }else{
            board[0][0] = -1;
            return board;
        }
    }
}

void printBoard(int **board, int lins, int cols){
    int l,c;
    for(l = 0; l < lins; l++){
        for(c = 0; c < cols; c++){
            printf(" %d ", board[l][c]);
        }
        printf("\n");
    }
}

int** expandBoard(int **board, int* lin, int* col, int* centerL, int* centerC){
    int auxboard[*lin][*col];
    int l,c;

    //Criando copia da matriz tabuleiro
    for(l = 0; l < *lin; l++){
        for(c = 0; c < *col; c++){
            auxboard[l][c] = board[l][c];
        }
    }
    int revL= 0, fowL = 0, revC = 0, fowC = 0;

    //Verificando quais limites possuem elementos
    for(c = 0; c < *col; c++){
        if(board[0][c] != -1){  //horizontal superior
            revL = 1;           //deve adicionar linha acima
        }
        if(board[*lin-1][c]!=-1){   //horizontal inferior
            fowL = 1;               //deve adicionar linha abaixo
        }
    }
    for(l = 0; l < *lin; l++){
        if(board[l][0] != -1){  //Vertical esquerda
            revC = 1;           // deve adicionar coluna a esquerda
        }
        if(board[l][*col-1]!=-1){   //vertical direita
            fowC = 1;               //deve adicionar coluna a direita
        }
    }

    int nLin = *lin + revL + fowL;
    int nCol = *col + revC + fowC;

    freeBoard(board, *lin);

    //aumentando o tamanho da matriz
    board = (int**) malloc(sizeof(int*)*(nLin));

    if (board == NULL){
        printf("Falha na re-alocacao!\n");
        exit(0);
    }else{
        for(l = 0; l < nLin; l++){
            board[l] = (int *) malloc(sizeof(int)*(nCol));
            if(board[l] == NULL){
                printf("Falha na re-alocacao!\n");
                exit(0);
            }
        }
    }

    //Movendo o centro do tabuleiro
    *centerL += revL;
    *centerC += revC;

    for(l = 0; l < nLin; l++){
        for(c = 0; c < nCol; c++){
            if((l >= revL) && (c >= revC) && (l <= *lin) && (c <= *col)){
                board[l][c] = auxboard[l-revL][c-revC]; //inserindo valores que já existiam
            }else{
                board[l][c] = -1;   //marcando novas posições vazias
            }
            printf(" %d", board[l][c]);
        }
        printf("\n");
    }
    *lin = nLin;
    *col = nCol;

    return board;
}

int main(){
    int boardLine = 1;
    int boardCol = 1;
    int centerL = 0, centerC = 0;
    int **board;

    board = makeBoard();
    printBoard(board, boardLine, boardCol);
    board[0][0] = 11;
    printf("%d %d\n", boardCol, boardLine);

    board = expandBoard(board, &boardLine, &boardCol, &centerL, &centerC);
    board[0][0] = 12;
    board = expandBoard(board, &boardLine, &boardCol, &centerL, &centerC);

    printf("%d %d\n", boardCol, boardLine);
    printBoard(board, boardLine, boardCol);

    freeBoard(board, boardLine);
    return 0;
}