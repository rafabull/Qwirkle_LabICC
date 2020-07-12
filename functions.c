#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//Definindo constantes para ateração de cor
const char cCodes[6][9] = {"\e[0;31m", "\e[0;32m", "\e[0;33m", "\e[0;34m", "\e[0;35m", "\e[0;36m"};
const char rColor[9] = "\033[0m";

//Definindo constantes para armazenar os tipos (formas) das peças e suas cores
const char types[6] = {'A', 'B', 'C', 'D', 'E', 'F'};
const char colors[6] = {'1', '2', '3', '4', '5', '6'};


//Exibe o menu e coleta informações dos jogadores
char** menu(int *nJog, char *cMode){
    //Imprimndo Qwirkle personalizado
    printf(" ::::::::   :::       ::: ::::::::::: :::::::::  :::    ::: :::        ::::::::::\n");
    printf(":+:    :+:  :+:       :+:     :+:     :+:    :+: :+:   :+:  :+:        :+:\n");
    printf("+:+    +:+  +:+       +:+     +:+     +:+    +:+ +:+  +:+   +:+        +:+\n");
    printf("+#+    +:+  +#+  +:+  +#+     +#+     +#++:++#:  +#++:++    +#+        +#++:++#\n");
    printf("+#+  # +#+  +#+ +#+#+ +#+     +#+     +#+    +#+ +#+  +#+   +#+        +#+\n");
    printf("#+#   +#+    #+#+# #+#+#      #+#     #+#    #+# #+#   #+#  #+#        #+#\n");
    printf(" ###### ###   ###   ###   ########### ###    ### ###    ### ########## ##########\n\n");

    int val = 0;
    while(!val){
        printf("Digite o numero de jogadores (2-4): ");
        scanf("%d", nJog);
        printf("\n");

        if((*nJog < 2)||(*nJog > 4)){
            printf("Numero invalido!\n");
        }else{
            val = 1;
        }
    }

    char **jog = (char**) malloc(sizeof(char*) * (*nJog));
    if (jog == NULL){
        printf("Falha na alocacao!\n");
        exit(0);
    }else{
        for(int l = 0; l < *nJog; l++){
            jog[l] = (char *) malloc(sizeof(char)*(21));
            if(jog[l] == NULL){
                printf("Falha na alocacao!\n");
                exit(0);
            }
        }
    }

    char nome[21];
    for(int l = 0; l < *nJog; l++){
            printf("Digite um nome para o jogador #%d (max = 20 caracteres): ", l+1);
            scanf(" %s", nome);
            strcpy(jog[l], nome);
    }

    val = 0;
    while(!val){
        printf("Cheat Mode? (S/N) ");
        scanf(" %c", cMode);
        printf("\n");
        *cMode = toupper(*cMode);

        if((*cMode != 'S') && (*cMode != 'N')){
            printf("Resposta invalida!\n");
        }else{
            val = 1;
        }
    }

    return jog;
}

//Função para liberar o tabuleiro
void freeBoard(int** board, int lines){
    int i;
    for(i = 0; i < lines; i++){
        free(board[i]); //liberando as linhas
    }
    free(board);    //liberando o ponteiro inicial
}

//Função para alocar o primeiro tabuleiro (1x1)
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
            board[0][0] = -1;   //Inserindo -1 na posição vazia
            return board;
        }
    }
}

//Função para imprimir o tabuleiro
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

        //Imprimindo primeira divisória de linha
        printf("     -"); //para a coluna indice
        for(c = 0; c < cols; c++)
            printf("-----");    //para as demais colunas
        printf("\n");

        for(c = 0; c < cols; c++){
            //imprimindo a numeração da linha
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
            //Para peças inseridas
            if (board[l][c]  != -1){
                ty = board[l][c] / 10;      //encontrando o tipo da peça
                cor = board[l][c] - ty*10;  //encontrando a cor da peça
                printf(" %s%c%c%s |", cCodes[cor-1], types[ty-1], colors[cor-1], rColor);   //imprimnido no formato esperado
            }else{
                //para peças não inseridas
                printf("    |");    // imprimindo espaço vazio do tamanho necessário
            }
        }
        printf("\n");
    }

    //imprimindo demais divisórias de linhas
    printf("     -"); //para a coluna indice
    for(c = 0; c < cols; c++)
        printf("-----");    //para as demais colunas
    printf("\n\n");
}

//Função para expandir o tabuleiro
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

    //definindo noas dimensões
    int nLin = *lin + revL + fowL;
    int nCol = *col + revC + fowC;

    //liberando a memória anterior
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

    //preenchendo as posições
    for(l = 0; l < nLin; l++){
        for(c = 0; c < nCol; c++){
            if((l >= revL) && (c >= revC) && (l < *lin + revL) && (c < *col + revC)){
                board[l][c] = auxboard[l-revL][c-revC]; //inserindo valores que já existiam
            }else{
                board[l][c] = -1;   //marcando novas posições vazias
            }
        }
    }
    //alterando valores armazenados  

    *lin = nLin;
    *col = nCol;

    return board;
}

//Função para codificar a peça
int codePiece(char *pc){
    int i, p = 0, val = 0;
    pc[0] = toupper(pc[0]);

    for(i = 0; i < 6; i++){
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
        return p;
    }else{
        return 0;
    }
}

//Função para verificar se o movimento é válido
int validateMove(int** board, int pc, int l, int c, int centerL, int centerC, int lins, int cols, int *lastMove){
    int response = 1;
    int i, dirL = 0, dirC = 0;

    //Verificando se a linha é valida
    if((centerL + l < 0) || (centerL + l >= lins)){
        printf("Linha inválida\n");
        return 0;
    }
    //verificando se a coluna é valida
    if((centerC + c < 0) || (centerC + c >= cols)){
        printf("Coluna inválida\n");
        return 0;
    }
    //verfcando se a posição está vazia
    if(board[centerL + l][centerC + c] != -1){
        printf("Posicao preenchida\n");
        return 0;
    }
    //verificando se esta grudado a uma peça caso não seja o movimento inicial
    int v1 = 1;
    if((lins != 1) || (cols != 1)){
        if(centerL + l - 1 >= 0)
            if(board[centerL + l - 1][centerC + c] != -1) v1 = 0;
        if(centerL + l + 1 < lins)
            if(board[centerL + l + 1][centerC + c] != -1) v1 = 0;
        if(centerC + c - 1 >= 0)
            if(board[centerL + l][centerC + c - 1] != -1) v1 = 0;
        if(centerC + c + 1 < cols)
            if(board[centerL + l][centerC + c + 1] != -1) v1 = 0;
    }else{
        v1 = 0;
    }
    if(v1){
        printf("Deveria haver pecas conectadas\n");
        return 0;
    }

    //verificando se existe uma peça semelhante a volta e não conflita com as demais
    int side, elL = -1, elC = -1;
    //verificando em cima
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
                else{
                    printf("Conflita com uma peca ao lado\n");
                    return 0;
                }
            }
        }
    }
    //verificando embaixo
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
                else{
                    printf("Conflita com uma peca ao lado\n");
                    return 0;
                }
            }
        }
    }
    //verificando esquerda
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
                else{
                    printf("Conflita com uma peca ao lado\n");
                    return 0;
                }
            }
        }
    }
    //verificando direita
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
                else{
                    printf("Conflita com uma peca ao lado\n");
                    return 0;
                }
            }
        }
    }

    //verificando se existe uma peça igual no grupo e se é compativel com td grupo
    int li = centerL + l, co = centerC + c;
    //grupo na vertical
    if(dirL != 0){
        while ((li + dirL >= 0)&&(li + dirL < lins)){
            li += dirL;
            side = board[li][co];
            if(side == -1)
                break;
            else{
                if (side == pc){
                    printf("Ha uma peca igual na linha\n");
                    return 0;
                }
                if ((elL == 0) && (side/10 != pc/10)){
                    printf("Incompativel com a linha\n");
                    return 0;
                }
                if ((elL == 1) && (side - (side/10)*10 != pc - (pc/10)*10)){
                    printf("Incompativel com a linha\n");
                    return 0;
                }
            }
        }
    }
    li = centerL + l, co = centerC + c;
    //Grupo na horizontal
    if(dirC != 0){
        while ((co + dirC >= 0)&&(co + dirC < cols)){
            co += dirC;
            side = board[li][co];
            if(side == -1)
                break;
            else{
                if (side == pc){
                    printf("Ha uma peca igual na linha\n");
                    return 0;
                }
                if ((elC == 0) && (side/10 != pc/10)){
                    printf("Incompativel com a linha\n");
                    return 0;
                }
                if ((elC == 1) && (side - (side/10)*10 != pc - (pc/10)*10)){
                    printf("Incompativel com a linha\n");
                    return 0;
                }
            }
        }
    }

    //verificando de compõe uma linha com os movimentos ateriores
    v1 = 0;
    int v2 = 0;
    if(lastMove[0] != 999){
        v1++;
        if(lastMove[2] == 999){
            //Verificando se esta conectado
            if((l == lastMove[0]+1) && (c == lastMove[1]))
                v2++;
            if((l == lastMove[0]-1) && (c == lastMove[1]))
                v2++;
            if((l == lastMove[0]) && (c == lastMove[1]+1))
                v2++;
            if((l == lastMove[0]) && (c == lastMove[1]-1))
                v2++;
        }else{
            v1++;
            if((l == lastMove[0]) && (l == lastMove[2])){ //verifica se tdo esta na mesma linha
                v2++;
                //verificando se esta conectado
                if((c == lastMove[1]+1) || (c == lastMove[1]-1))
                    v2++;
                if((c == lastMove[3]+1) || (c == lastMove[3]-1))
                    v2++;
            }
            if((c == lastMove[1]) && (c == lastMove[3])){   //verifica se tudo esta na mesma coluna
                v2++;
                //verificando se esta conectado
                if((l == lastMove[0]+1) || (l == lastMove[0]-1))
                    v2++;
                if((l == lastMove[2]+1) || (l == lastMove[2]-1))
                    v2++;
            }
        }
    }
    
    if(v1 != v2){
        printf("V1 %d, v2 %d\n", v1, v2);
        printf("Incompativel com os movimentos anteriores\n");
        return 0;
    }

    return response;
}

//Função para realizar o movimento
int makeMove(int** board, int pc, int l, int c, int centerL, int centerC, int lins, int cols, int *lastMove){
    int i, val = 0;

    if(pc){
        if(validateMove(board, pc, l, c, centerL, centerC, lins, cols, lastMove)){ //verificar se o movimento é válido
            //adicionar a ultima jogada
            if(lastMove[0] == 999){
                lastMove[0] = l;
                lastMove[1] = c;
            }else{
                if(lastMove[2] == 999){
                    lastMove[2] = l;
                    lastMove[3] = c;
                }else{
                    //manter as fronteiras corretas
                    //fileiras verticais
                    if(lastMove[0] > lastMove[2]){
                        if(l > lastMove[0]){
                            lastMove[0] = l;
                            lastMove[1] = c;
                        }else{
                            lastMove[2] = l;
                            lastMove[3] = c;
                        }
                    }
                    if(lastMove[0] < lastMove[2]){
                        if(l < lastMove[0]){
                            lastMove[0] = l;
                            lastMove[1] = c;
                        }else{
                            lastMove[2] = l;
                            lastMove[3] = c;
                        }
                    }
                    //fileiras horizontais
                    if(lastMove[1] > lastMove[3]){
                        if(l > lastMove[0]){
                            lastMove[0] = l;
                            lastMove[1] = c;
                        }else{
                            lastMove[2] = l;
                            lastMove[3] = c;
                        }
                    }
                    if(lastMove[1] < lastMove[3]){
                        if(l < lastMove[1]){
                            lastMove[0] = l;
                            lastMove[1] = c;
                        }else{
                            lastMove[2] = l;
                            lastMove[3] = c;
                        }
                    }
                }
            }
            //inserindo no tabuleiro
            board[centerL + l][centerC + c] = pc;
            return 1;
        }else{
            printf("Jogada invalida\n");
        }
    }else{
        printf("Peca invalida\n");
    }
    return 0;
}

//Função para criar as peças do "saco"
int* makePile(){
    int *vet = (int*) malloc(sizeof(int)*108);
    if(vet == NULL){
        printf("Falha na alocação!\n");
        exit(0);
    }
    int i = 6, e = 6, cont;
    for(cont = 0; cont < 36; cont++){
        //alterando variáveis para que a incerção seja na ordem
        if (e < 5){
            e++;
        }else{
            e = 0;
            i++;
        }
        if(i > 5){
            i = 0;
        }
        //inserindo as 3 peças iguais em cada ciclo de peças iguais
        vet[cont] = (i+1)*10 + e+1;
        vet[cont + 36] = (i+1)*10 + e+1;
        vet[cont + 72] = (i+1)*10 + e+1;
    }
    return vet;
}

//Função para "comprar uma peça" do "saco"
int getPiece(int *pile){
    int control = 0;
    //gerando numero aleatório até 108     
    int pos = rand() % 107;

    //encontando a próxima posição que contenha uma peça (casa a escolhida não possua)
    while(pile[pos] == -1){
        if (pos >= 107){
            pos = 0;
        }else{
            pos+= 1;
        }
        control ++;
        if (control == 108){
            return 0;  //caso não exista nenhuma peça no vetor
        }
    }

    int pc = pile[pos]; //armazenando a peça
    pile[pos] = -1;     //esvaziando a posição

    return pc;
}

//função para trocar uma peça
void tradePiece(int *pile, int pc, int*hand){
    int nPc = getPiece(pile);   //pega uma nova peça aleatória
    int pos = 0;
    //Adicionando nova peça a mão do jogador
    int v = 1;
    while (pos < 7){
        if(hand[pos] == pc){
            hand[pos] = nPc;
            v = 0;
            break;
        }else{
            pos ++;
        }
    }
    if(v){
        printf("Voce nao possui essa peca! \n");
    }else{
        pos = 0;
        //insere a peça a ser trocada na pimeira posição vazia encontrada
        while (pos < 108){
            if (pile[pos] == -1){
                break;
            }else{
                pos++;
            }
        }
        pile[pos] = pc;
    }   
}

//recolhe as 7 peças para niciar uma "mão"
int* getHand(int *pile){
    int i;
    int *h = (int*) malloc(sizeof(int)*6);
    if(h == NULL){
        printf("Falha na Alocação!\n");
        exit(0);
    }
    for(i = 0; i < 6; i++){
        h[i] = getPiece(pile);  //inserindo nova peça na mão
    }
    return h;
}

//imprime a "mão"
void printHand(int *hand){
    int i;
    for(i = 0; i < 6; i++){
        if(hand[i] != -1){
            int ty = hand[i] / 10;
            int cor = hand[i] - ty*10;
            printf(" %s%c%c%s |", cCodes[cor-1], types[ty-1], colors[cor-1], rColor);   //imprimnido no formato esperado
        }
    }
    printf("\n");
}

//libera td memória alocada donâmicamente
void freeAll(int **board, int boardLine, int *pile, int **hand,int nJog, char **nomeJog){
    freeBoard(board, boardLine);
    free(pile);
    for(int i = 0; i < nJog; i++){
        free(hand[i]);
        free(nomeJog[i]);
    }
    free(nomeJog);
}

int checkHand(int *h, int pc){
    //verificando se é uma peca valida
    if(!pc){
        return 0;
    }

    //verificando se a peca esta na mão do jogador
    for(int i = 0; i < 6; i++){
        if(pc == h[i]){
            return 1;
        }
    }

    return 0;
}

void removeFromHand(int pc, int *hand){
    for(int i = 0; i < 6; i++){
        if(hand[i] == pc){
            hand[i] = -1;
            break;
        }
    }
}

int reloadHand(int *hand, int *pile, int nCompras){
    int cont = 0;
    for(int i = 0; i < 6; i++){
        if(hand[i] == -1){
            cont ++;
            if (nCompras + cont <= 108)
                hand[i] = getPiece(pile);
            else 
                break;
        }
    }
    return cont;
}

//recebe cordenadas direto no formato da matriz
int countConnect(int **board, int l, int c, int side){
    if( board[l][c] == -1){
        return 0;
    }
    if(side == 1){  //cima
        return 1 + countConnect(board, l+1, c, side);
    }
    if(side == 2){  //Direita
        return 1 + countConnect(board, l, c+1, side);
    }
    if(side == -1){  //Baixo
        return 1 + countConnect(board, l-1, c, side);
    }
    if(side == -2){  //Esquerda
        return 1 + countConnect(board, l, c-1, side);
    }
}

//Função conta os pontos daquela jogada
int countPoints(int **board, int centerL, int centerC, int *lastMove){
    int pontos = 0, l = 0, c = 0;
    int pt = 0;
    int v = 1,  s = 1;
    int primeiraL, ultimaL;
    int primeiraC, ultimaC;

    if((lastMove[0] == 999) && (lastMove[2] == 999)){ //caso nenhuma jogada tenha sido feita
        printf("Pontos dessa rodada: %d\n", pontos);
        return 0;
    }
    if (lastMove[2] == 999){   //caso tenha sido inserida apenas uma peça
        l = centerL + lastMove[0];
        c = centerC + lastMove[1];

        //Análise Vertical
        pt += countConnect(board, l+1, c, 1);
        pt += countConnect(board, l-1, c, -1);
        if (pt != 0) pt++; //se houver uma linha a peca deve ser contada
        pontos += pt;
        //Analise de qwirkle
        if (pt == 6){
            pontos += 6;
        }
        pt = 0;

        //Análise horizontal
        pt += countConnect(board, l, c+1, 2);
        pt += countConnect(board, l, c-1, -2);
        if (pt != 0) pt++; //se houver uma linha a peca deve ser contada
        pontos += pt;
        //Analise de qwirkle
        if (pt == 6){
            pontos += 6;
        }
        pt = 0;

        if(pontos == 0)
            pontos++; //Caso não exista nenhuma conexão com a peça
    
        printf("Pontos dessa rodada: %d\n", pontos);
        return pontos;
    }

    //Se foi inserida uma linha (mais de uma peça)
    //Caso a linha colocada seja na horizontal
    if(lastMove[0] == lastMove[2]){ 
        //Análise perpendicular
        //Definindo o sentido a ser percorrido
        if(lastMove[1] <= lastMove[3]){
            s = 1;
        }else{
            s = -1;
        }
        //percorrendo todas as peças jogadas
        int i = lastMove[1];
        int cont = 1;
        l = centerL + lastMove[0];
        while(cont){
            c = centerC + i;

            pt += countConnect(board, l+1, c, 1);
            pt += countConnect(board, l-1, c, -1);
            if (pt != 0) pt++; //se houver uma linha a peca deve ser contada
        
            pontos += pt;
            //Analise de qwirkle
            if (pt == 6){
                pontos += 6;
            }
            pt = 0;

            //caminhando no sentido correto
            i += s;

            //codicioes de saida
            if((i > lastMove[1]) && (i > lastMove[3]))
                cont = 0;
            if((i < lastMove[1]) && (i < lastMove[3]))
                cont = 0;
        }
        //Análise da linha jogada
        //Análise horizontal
        l = centerL + lastMove[0];
        c = centerC + lastMove[1];
        pt += countConnect(board, l, c+1, 2);
        pt += countConnect(board, l, c-1, -2);
        if (pt != 0) pt++; //se houver uma linha a peca deve ser contada

        pontos += pt;
        //Analise de qwirkle
        if (pt == 6){
            pontos += 6;
        }
        pt = 0;

        printf("Pontos dessa rodada: %d\n", pontos);
        return pontos;
    }
    //Caso a linha colocada seja na vertical
    if(lastMove[1] == lastMove[3]){ 
        //Análise perpendicular
        //Definindo o sentido a ser percorrido
        if(lastMove[0] <= lastMove[2]){
            s = 1;
        }else{
            s = -1;
        }
        //percorrendo todas as peças jogadas
        int i = lastMove[0];
        int cont = 1;
        c = centerC + lastMove[1];
        while(cont){
            l = centerL + i;

            pt += countConnect(board, l, c+1, 2);
            pt += countConnect(board, l, c-1, -2);
            if (pt != 0) pt++; //se houver uma linha a peca deve ser contada
        
            pontos += pt;
            //Analise de qwirkle
            if (pt == 6){
                pontos += 6;
            }
            pt = 0;

            //caminhando no sentido correto
            i += s;

            //codicioes de saida
            if((i > lastMove[0]) && (i > lastMove[2]))
                cont = 0;
            if((i < lastMove[0]) && (i < lastMove[2]))
                cont = 0;
        }
        //Análise da linha jogada
        //Análise horizontal
        l = centerL + lastMove[2];
        c = centerC + lastMove[3];
        pt += countConnect(board, l+1, c, 1);
        pt += countConnect(board, l-1, c, -1);
        if (pt != 0) pt++; //se houver uma linha a peca deve ser contada

        pontos += pt;
        //Analise de qwirkle
        if (pt == 6){
            pontos += 6;
        }
        pt = 0;

        printf("Pontos dessa rodada: %d\n", pontos);
        return pontos;
    }
    
    return pontos;
}

//Verificando se o jogador esvaziou sua mão
int isHandEmpty(int *hand){
    int res = 1;
    for(int i = 0; i < 6; i++){
        if(hand[i] != -1){
            return 0;
        }
    }
    return res;
}

//Procura a peça na pilha e retorna sua posicao, de 1 a 108
int checkPile(int *pile, int pc){
    int cont, e, i;
    for(cont = 0; cont < 36; cont++){
        //Comparando 3 posições no mesmo ciclo para maior velocidado
        if(pile[cont] == pc){
            return cont + 1;
        }
        if(pile[cont + 36] == pc){
            return cont + 36 + 1 ;
        }
        if(pile[cont + 72] == pc){
            return cont + 72 + 1 ;
        }
    }
    return 0;
}

//Verifica se uma peça está disponível no cheat mode
int isAvaiable(int *hand, int *pile, int pc){
    if(!pc){                    //Verifica se a peça digtada é válida
        printf("Peca Invalida!\n");
        return 0;
    }
    if(checkHand(hand, pc)){    //Verifica se esta na mão do jogador
        return -1;
    }
    int pilePos = checkPile(pile, pc);
    if(pilePos){    //Verifica se a peça esta na pilha
        return pilePos;           
    }
    printf("Peca nao disponvel!\n");
    return 0;                   //Caso não esteja em nenhuma
}

//Remove a peça na posição pilePos-1 da pilha
void removeFromPile(int pilePos, int *pile){
    if((pilePos <= 108) && (pilePos > 0)){
        pile[pilePos - 1] = -1;
    }
}