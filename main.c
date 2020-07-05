#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h> 
#include "functions.h"

int main(){
    srand(time(0));
    int boardLine = 1;
    int boardCol = 1;
    int centerL = 0, centerC = 0;
    int **board, *pile;
    int i = 0, cont = 0, end = 1;

    board = makeBoard(); //criando o tabuleiro
    pile = makePile();   //Criando o "saco" de peças restantes

    int nJog;
    char **nomeJog = menu(&nJog);   //Menu usado para configurações de jogadores
    fflush(stdin);

    //armazenado a primeira mão de cada jogador
    int *hand[nJog];
    for(i = 0; i < nJog; i++){
        hand[i] = getHand(pile);
    }

    int lastMove[4] = {-1, -1, -1, -1};  //linha1 coluna1 liha2 coluna2
    
    //iniciando o jogo
    int vez = 0;
    while (end){
        printBoard(board, boardLine, boardCol, centerL, centerC);

        //imprimindo dados da vez
        printf("Vez de %s \n", nomeJog[vez]);
        printHand(hand[vez]);

        //Armazenando o comando digitado
        char op[7][7];
        char str[60];
        int l, c;

        gets(str);
        printf("\n");

        //Dividindo o comando digitado
        char * token = strtok(str, " ");
        i = 0;
        while( token != NULL ) {
            strcpy(op[i], token);
            token = strtok(NULL, " ");
            i++;
        }

        //Controle de ação a ser realizada
        if(!strcmp(op[0], "jogar")){
            l = atoi(op[2]);
            c = atoi(op[3]);

            int pc = codePiece(op[1]);
            
            if(checkHand(hand[vez], pc)){
                if(makeMove(board, pc, l, c, centerL, centerC, boardLine, boardCol, lastMove)){
                    removeFromHand(pc, hand[vez]);
                    board = expandBoard(board, &boardLine, &boardCol, &centerL, &centerC);
                }
            }else{
                printf("Peca Invalida!\n");
            }
            
        }else{
            if(!strcmp(op[0], "trocar")){
                int pc = codePiece(op[1]);
                tradePiece(pile, pc, hand[vez]);
            }
            if(!strcmp(op[0], "sair"))
                end = 0;
            if(!strcmp(op[0], "passar")){
                reloadHand(hand[vez], pile);
                for(i = 0; i < 4; i++)
                    lastMove[i] = -1; 
                vez++;
                if(vez >= nJog){
                    vez = 0;
                }
                system("clear || cls"); //limpando a tela
            }
        }
    }

    freeAll(board, boardLine, pile, hand, nJog, nomeJog);
    
    return 0;
}