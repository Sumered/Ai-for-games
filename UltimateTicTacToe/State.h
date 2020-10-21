#include "common.h"
#include "UltimateTicTacToe.h"

class State{
public:
    UltimateTicTacToe state;
    int wins,allGames,ended;
    vector <shared_ptr <State> >sonsStates;
    int act;
    vector <Move> possibleMoves;
    Move moveToDescend(shared_ptr <State> son);
    State& operator=(const State& rhs);
    State();
    State(const State &s);
    State(const UltimateTicTacToe &state);
    void generate_moves();
};