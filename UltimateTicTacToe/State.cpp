#include "State.h"

State::State(const UltimateTicTacToe &ut){ 
    wins = 0; allGames = 0; act = 0; ended = 0;
    state = UltimateTicTacToe(ut);
    state.generateAllPossibleMoves(possibleMoves);
}

State::State(){ 
    wins = 0; allGames = 0; act = 0;ended = 0;
    state = UltimateTicTacToe();
    state.generateAllPossibleMoves(possibleMoves);
}

State::State(const State &s){
    state = UltimateTicTacToe(s.state); wins = 0; allGames = 0; act = 0; ended = 0;
}

State& State::operator=(const State& rhs){
    this->state = UltimateTicTacToe(rhs.state); wins = 0; allGames = 0; act = 0; ended = 0;
    for(int i=0;i<rhs.sonsStates.size();i++){
        this->sonsStates.pb(rhs.sonsStates[i]);
    }
    for(int i=0;i<rhs.possibleMoves.size();i++){
        this->possibleMoves.pb(rhs.possibleMoves[i]);
    }
    return *this;
}

void State::generate_moves(){
    act = 0;
    state.generateAllPossibleMoves(possibleMoves);
}

Move State::moveToDescend(shared_ptr <State> son){
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            if(state.board[i][j/3][j%3] != son->state.board[i][j/3][j%3] && state.board[i][j/3][j%3] != 0){
                return Move(i,j/3,j%3);
            }
        }
    }
}
