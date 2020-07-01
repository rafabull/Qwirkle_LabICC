#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include "functions.h"

int main(){
    int boardLine = 1;
    int boardCol = 1;
    int centerL = 0, centerC = 0;
    int **board, *pile, *hand;
    int i = 0, cont = 1;

    pile = makePile();

    board = makeBoard();
    printBoard(board, boardLine, boardCol, centerL, centerC);

    hand = getHand(pile);
    printHand(hand);

    while (cont){

        char op[7][7];
        char str[60];
        int l, c;

        gets(str);

        char * token = strtok(str, " ");

        i = 0;
        while( token != NULL ) {
            strcpy(op[i], token);
            token = strtok(NULL, " ");
            i++;
        }

        if(!strcmp(op[0], "jogar")){
            l = atoi(op[2]);
            c = atoi(op[3]);
            makeMove(board, op[1], l, c, centerL, centerC, boardLine, boardCol);
            board = expandBoard(board, &boardLine, &boardCol, &centerL, &centerC);
            printBoard(board, boardLine, boardCol, centerL, centerC);
        }else{
            if(!strcmp(op[0], "sair"))
                cont = 0;
            if(!strcmp(op[0], "trocar")){
                printf("trade\n");
            }
        }
    }

    freeBoard(board, boardLine);
    free(pile);
    free(hand);
    return 0;
}