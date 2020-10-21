#include "common.h"

class Move{
public:
    int place,x,y;
    Move(int p,int x,int y);
    bool operator <(const Move& rhs) const;
    bool operator ==(const Move& rhs) const;
    int hash() const;
};

extern map<int,pair <int,int> > Mast;

extern map <int,double> Pap;

extern double alfa;

extern float greed;

class State;

class UltimateTicTacToe {
public:
    int board[9][3][3];
    int state[3][3];
    int full[3][3];
    int where;
    int who;
    //class funcs
    bool operator==(const UltimateTicTacToe& other);
    UltimateTicTacToe& operator=(const UltimateTicTacToe& rhs);
    UltimateTicTacToe();
    UltimateTicTacToe(const UltimateTicTacToe &UT);
    void print() const;
    int checkBoard(int pos);
    int isTerminated();
    int makeMove(Move m);
    void generateAllPossibleMoves(vector <Move> &res) const;
    int doRandomGame(int mast = 0,int pap = 0) const;
    int makeRandomMove(const vector <Move> &possibleMoves);
    pair <int,Move> makeRandomMastMove(const vector <Move> &possibleMoves);
    pair <int,Move> makeRandomPapMove(const vector <Move> &possibleMoves);
};