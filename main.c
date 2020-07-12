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

    int nJog, mode;
    char **nomeJog = menu(&nJog, &mode);   //Menu usado para configurações de jogadores
    fflush(stdin);
    int pontos[nJog];

    //armazenado a primeira mão de cada jogador
    int *hand[nJog];
    for(i = 0; i < nJog; i++){
        hand[i] = getHand(pile);
        pontos[i] = 0;  //inicializa os pontos com 0
    }

    int lastMove[4] = {999, 999, 999, 999}; //Variavel para armazenar os mov da rodada (linha1 coluna1 liha2 coluna2)  
    int vTrade = 1;                         //Variavel para verificar se a troca da rodada já fo feita
    int nCompras = 6 * nJog;                //Variável para armazenar quantas peças já foram tiradas do "saco"

    //iniciando o jogo
    int vez = 0;
    while (end){
        printBoard(board, boardLine, boardCol, centerL, centerC);

        //imprimindo dados da vez
        printf("Vez de %s | Pontos: %d\n", nomeJog[vez], pontos[vez]);
        printHand(hand[vez]);

        //Armazenando o comando digitado
        char op[7][7];
        char str[60];
        int l, c;

        gets(str);
        printf("\n");

        //Dividindo o comando digitado
        int nTk = 0;
        char * token = strtok(str, " ");
        i = 0;
        while( token != NULL ) {
            strcpy(op[i], token);
            token = strtok(NULL, " ");
            i++;
        }
        nTk = i;

        //Controle de ação a ser realizada
        if(!strcmp(op[0], "jogar")){
            l = atoi(op[2]);
            c = atoi(op[3]);

            int pc = codePiece(op[1]);
            
            if(checkHand(hand[vez], pc)){
                if(makeMove(board, pc, l, c, centerL, centerC, boardLine, boardCol, lastMove)){
                    removeFromHand(pc, hand[vez]);
                    board = expandBoard(board, &boardLine, &boardCol, &centerL, &centerC);
                    if(nCompras == 108){
                        if(isHandEmpty(hand[vez])){
                            pontos[vez] += 6;
                            end = 0;
                        }
                    }
                }
            }else{
                printf("Peca Invalida!\n");
            }
            
        }else{
            if(!strcmp(op[0], "trocar")){
                if(vTrade){
                    int pc;
                    for(i = 1; i < nTk; i++){
                        pc = codePiece(op[i]);
                        tradePiece(pile, pc, hand[vez]);
                    }
                    vTrade = 0;
                }else{
                    printf("Voce ja realizou suas trocas esta rodada\n");
                }
            }
            if(!strcmp(op[0], "sair"))
                end = 0;
            if(!strcmp(op[0], "passar")){
                pontos[vez] += countPoints(board, centerL, centerC, lastMove);
                if(nCompras <= 108)
                    nCompras += reloadHand(hand[vez], pile, nCompras);

                vTrade = 1;

                for(i = 0; i < 4; i++)
                    lastMove[i] = 999; 
                vez++;
                if(vez >= nJog){
                    vez = 0;
                }
    
            }
        }
    }
    printf("Fim de jogo!! \n Pontuacoes finais:\n");
    for(i = 0; i < nJog; i++){
        printf("Jogador %s: %d Pontos \n", nomeJog[i], pontos[i]);
    }
    freeAll(board, boardLine, pile, hand, nJog, nomeJog);
    
    return 0;
}