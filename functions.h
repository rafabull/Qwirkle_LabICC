
char** menu(int *nJog, char *cMode);

void freeBoard(int** board, int lines);
int** makeBoard();
void printBoard(int **board, int lins, int cols, int cLin, int cCol);
int** expandBoard(int **board, int* lin, int* col, int* centerL, int* centerC);

int codePiece(char *pc);
int validateMove(int** board, int pc, int l, int c, int centerL, int centerC, int lins, int cols, int *lastMove);
int makeMove(int** board, int pc, int l, int c, int centerL, int centerC, int lins, int cols, int *lastMove);

int* makePile();
int getPiece(int *pile);
void tradePiece(int *pile, int pc, int *hand);

int* getHand(int *pile);
void printHand(int *hand);

void freeAll(int **board, int boardLine, int *pile, int **hand,int nJog, char **nomeJog);
int checkHand(int *h, int pc);
void removeFromHand(int pc, int *hand);
int reloadHand(int *hand, int *pile, int nCompras);

int countPoints(int **board, int centerL, int centerC, int *lastMove);
int isHandEmpty(int *hand);

int checkPile(int *pile, int pc);
int isAvaiable(int *hand, int *pile, int pc);
int removeFromPile(int pilePos, int *pile);

