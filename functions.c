#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

const char cCodes[6][9] = {"\e[0;31m", "\e[0;32m", "\e[0;33m", "\e[0;34m", "\e[0;35m", "\e[0;36m"};
const char rColor[9] = "\033[0m";

const char types[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
const char colors[6] = {'1', '2', '3', '4', '5', '6'};

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

void printBoard(int **board, int lins, int cols, int cLin, int cCol){
    int l,c, ty, cor;
    for(l = 0; l < lins; l++){
        //imprimir a numeração das colunas
        if (l == 0){
            printf("     ");
            for(c = 0; c < cols; c++){
                //Corrigir alinhamento
                if((c-cCol >= 0) && (c-cCol < 10))
                    printf("   %d ", c-cCol);
                else{
                    if (c-cLin > -10)
                        printf("  %d ", c-cCol);
                    else
                        printf(" %d ", c-cCol);
                }
            }
            printf("\n");

        }

        printf("     -"); //para a coluna indice
        for(c = 0; c < cols; c++)
            printf("-----");    //para as demais colunas
        printf("\n");

        for(c = 0; c < cols; c++){
            if (c == 0){
                //Corrigir alinhamento
                if((l-cLin >= 0) && (l-cLin < 10))
                    printf("   %d |", l-cLin);
                else{
                    if (l-cLin > -10)
                        printf("  %d |", l-cLin);
                    else
                        printf(" %d |", l-cLin);
                }
            }
            if (board[l][c]  != -1){
                ty = board[l][c] / 10;
                cor = board[l][c] - ty*10;
                printf(" %s%c%c%s |", cCodes[cor-1], types[ty-1], colors[cor-1], rColor);
            }else{
                printf("    |");
            }
        }
        printf("\n");
    }
    printf("     -"); //para a coluna indice
    for(c = 0; c < cols; c++)
        printf("-----");    //para as demais colunas
    printf("\n\n");
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
            if((l >= revL) && (c >= revC) && (l < *lin + revL) && (c < *col + revC)){
                board[l][c] = auxboard[l-revL][c-revC]; //inserindo valores que já existiam
            }else{
                board[l][c] = -1;   //marcando novas posições vazias
            }
        }
    }
    *lin = nLin;
    *col = nCol;

    return board;
}

int validateMove(int** board, int pc, int l, int c, int centerL, int centerC, int lins, int cols){
    int response = 1;
    int i, dirL = 0, dirC = 0;
    //Verificando se a linha é valida
    if((centerL + l < 0) || (centerL + l >= lins)){
        return 0;
    }
    //verificando se a coluna é valida
    if((centerC + c < 0) || (centerC + c >= cols)){
        return 0;
    }
    //verfcando se a posição está vazia
    if(board[centerL + l][centerC + c] != -1){
        return 0;
    }
    //verificando se existe uma peça semelhante a volta e não conflita com as demais
    int side, elL = -1, elC = -1;
    if(centerL + l - 1 >= 0){
        puts("1");
        side = board[centerL + l - 1][centerC + c];
        if(side != -1){
            if(side/10 == pc/10){
                elL = 0 ;    //compatvel com o tipo
                dirL = -1;
            }
            else{
                if(side - (side/10)*10 == pc - (pc/10)*10){
                    elL = 1;    //compativel com a cor
                    dirL = -1;
                }
                else
                    return 0;
            }
        }
    }
    if(centerL + l + 1 < lins){
        side = board[centerL + l + 1][centerC + c];
        if(side != -1){
            if(side/10 == pc/10){
                elL = 0 ;    //compatvel com o tipo
                dirL = 1;
            }
            else{
                if(side - (side/10)*10 == pc - (pc/10)*10){
                    elL = 1;    //compativel com a cor
                    dirL = 1;
                }
                else
                    return 0;
            }
        }
    }
    if(centerC + c - 1 >= 0){
        side = board[centerL + l][centerC + c - 1];
        if(side != -1){
            if(side/10 == pc/10){
                elC = 0 ;    //compatvel com o tipo
                dirC = -1;
            }
            else{
                if(side - (side/10)*10 == pc - (pc/10)*10){
                    elC = 1;    //compativel com a cor
                    dirC = -1;
                }
                else
                    return 0;
            }
        }
    }
    if(centerC + c + 1 < cols){
        side = board[centerL + l][centerC + c + 1];
        if(side != -1){
            if(side/10 == pc/10){
                elC = 0 ;    //compatvel com o tipo
                dirC = 1;
            }
            else{
                if(side - (side/10)*10 == pc - (pc/10)*10){
                    elC = 1;    //compativel com a cor
                    dirC = 1;
                }
                else
                    return 0;
            }
        }
    }

    //verificando se existe uma peça igual no grupo e se é compativel com td grupo
    int li = centerL + l, co = centerC + c;
    if(dirL != 0){
        while ((li + dirL >= 0)&&(li + dirL < lins)){
            li += dirL;
            side = board[li][co];
            if(side == -1)
                break;
            else{
                if (side == pc)
                    return 0;
                if ((elL == 0) && (side/10 != pc/10))
                    return 0;
                if ((elL == 1) && (side - (side/10)*10 != pc - (pc/10)*10))
                    return 0;
            }
        }
    }
    li = centerL + l, co = centerC + c;
    if(dirC != 0){
        while ((co + dirC >= 0)&&(co + dirC < cols)){
            co += dirC;
            side = board[li][co];
            if(side == -1)
                break;
            else{
                if (side == pc)
                    return 0;
                if ((elC == 0) && (side/10 != pc/10))
                    return 0;
                if ((elC == 1) && (side - (side/10)*10 != pc - (pc/10)*10))
                    return 0;
            }
        }
    }

    return response;
}

void makeMove(int** board, char *pc, int l, int c, int centerL, int centerC, int lins, int cols){
    int i, p = 0, val = 0;
    pc[0] = toupper(pc[0]);

    for(i = 0; i < 6; i++){
        //Codificando a peça
        if(pc[0] == types[i]){
            p += (i+1)*10;
            val += 1;
        }
        if(pc[1] == colors[i]){
            p += i+1;
            val += 1;
        }
    }

    if(val == 2){
        if(validateMove(board, p, l, c, centerL, centerC, lins, cols)){
            //inserindo no tabuleiro
            board[centerL + l][centerC + c] = p;
        }else{
            printf("Jogada invalida\n");
        }
    }else{
        printf("Peca invalida\n");
    }
}

int* makePile(){
    int *vet = (int*) malloc(sizeof(int)*108);
    int i = 6, e = 6, cont;
    for(cont = 0; cont < 108; cont++){
        if (e < 5){
            e++;
        }else{
            e = 0;
            i++;
        }
        if(i > 5){
            i = 0;
        }
        vet[cont] = (i+1)*10 + e+1;
    }
    return vet;
}

int getPiece(int *pile){
    srand(time(0));
    int pos = rand() % 108;
    while(pile[pos] == -1){
        if (pos >= 108){
            pos = 0;
        }else{
            pos+= 1;
        }
    }
    int pc = pile[pos];
    pile[pos] = -1;

    return pc;
}

int tradePiece(int *pile, int pc){
    int nPc = getPiece(pile);
    int pos = 0;
    while (pos < 108){
        if (pile[pos] == -1){
            break;
        }else{
            pos++;
        }
    }
    pile[pos] = pc;
    return nPc;
}

int* getHand(int *pile){
    int i;
    int *h = (int*) malloc(sizeof(int)*7);
    for(i = 0; i < 7; i++){
        h[i] = getPiece(pile);
    }
    return h;
}

void printHand(int *hand){
    int i;
    for(i = 0; i < 7; i++){
        int ty = hand[i] / 10;
        int cor = hand[i] - ty*10;
        printf(" %s%c%c%s |", cCodes[cor-1], types[ty-1], colors[cor-1], rColor);
    }
    printf("\n");
}