
void freeBoard(int** board, int lines);
int** makeBoard();
void printBoard(int **board, int lins, int cols, int cLin, int cCol);
int** expandBoard(int **board, int* lin, int* col, int* centerL, int* centerC);

int validateMove(int** board, int pc, int l, int c, int centerL, int centerC, int lins, int cols);
void makeMove(int** board, char *pc, int l, int c, int centerL, int centerC, int lins, int cols);

int* makePile();
int getPiece(int *pile);
int tradePiece(int *pile, int pc);

int* getHand(int *pile);
void printHand(int *hand);