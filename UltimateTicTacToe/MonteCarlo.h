#include "State.h"

class AI {
public:
    virtual Move findMove(const UltimateTicTacToe& game,int timeLimit) = 0;
    virtual void reset() = 0;
};

class FlatMonteCarlo: public AI {
public:
    Move findMove(const UltimateTicTacToe &game,int timeLimit);
    void reset();
};

class MCTS: public AI {
private:
    int bestChild(shared_ptr <State> game);
public:
    void reset();
    shared_ptr <State> actualTree;
    float exploreRate;
    MCTS(float er);
    Move findMove(const UltimateTicTacToe &game,int timeLimit);
    int mcts(shared_ptr <State> game);
    void proneTree(const UltimateTicTacToe &game); //this will be pretty crazy
};