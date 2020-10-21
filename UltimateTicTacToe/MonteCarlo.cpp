#include "MonteCarlo.h"

void FlatMonteCarlo::reset(){};

Move FlatMonteCarlo::findMove(const UltimateTicTacToe &game,int timeLimit){
    vector <Move> potentialMoves;
    game.generateAllPossibleMoves(potentialMoves);
    int numberOfMoves = potentialMoves.size();
    pair <int,int> scores[numberOfMoves];
    for(int i=0;i<numberOfMoves;i++){ scores[i] = mp(0,0); }
    time_t startTime = time(0);
    while(time(0) - startTime < timeLimit){
        for(int i=0;i<numberOfMoves;i++){
            UltimateTicTacToe fakeGame = UltimateTicTacToe(game);
            int result = fakeGame.makeMove(potentialMoves[i]);
            if(result == game.who){ scores[i].f++;scores[i].s++;continue;}
            if(result != 0){scores[i].s++;continue;}
            result = fakeGame.doRandomGame();
            if(result == game.who){ scores[i].f++;scores[i].s++;continue;}
            if(result != 0){scores[i].s++;continue;}
        }
    }
    int best;
    float highscore = -1.0;
    //number of simulations when board is clean is 81*135, when game is near end then 7*8192 per second
    for(int i=0;i<numberOfMoves;i++){
        float ithScore = float(scores[i].f)/scores[i].s;
        if(ithScore > highscore){ highscore = ithScore; best = i; }
    }
    return potentialMoves[best];
}


void MCTS::proneTree(const UltimateTicTacToe &game){
    // printf("Actual game state\n\n");
    // game.print();
    // printf("Actual tree state\n\n");
    // actualTree->state.print();
    for(int i=0;i<actualTree->sonsStates.size();i++){
        if(actualTree->sonsStates[i]->state == game){
            actualTree = actualTree->sonsStates[i];
            return;
        }
    }
    actualTree = make_shared<State>(game);
    actualTree->generate_moves();
}

MCTS::MCTS(float er){ exploreRate = er; actualTree = make_shared<State>(); }

Move MCTS::findMove(const UltimateTicTacToe& game,int timeLimit){
    //printf("Before mcts move\n\n");
    if(!(actualTree->state == game)){
        proneTree(game);
    }
    //printf("After prone state\n\n");
    //actualTree->state.print();
    int startTime = time(0);
    int iterations = 0;
    while(time(0) - startTime < timeLimit){
        iterations++;
        int result = mcts(actualTree);
        if(result == this->actualTree->state.who){
            actualTree->wins++;
        }
        actualTree->allGames++;
    }
    printf("ITERATIONS: %d\n",iterations);
    float highscore = 0.0;
    int chosenIndex;
    for(int i=0;i<actualTree->sonsStates.size();i++){
        if(highscore < float(actualTree->sonsStates[i]->wins)/actualTree->sonsStates[i]->allGames){
            highscore = float(actualTree->sonsStates[i]->wins)/actualTree->sonsStates[i]->allGames;
            chosenIndex = i;
        }
    }
    Move return_val = Move(actualTree->possibleMoves[chosenIndex]);
    UltimateTicTacToe g (actualTree->sonsStates[chosenIndex]->state);
    //printf("After mcts move\n\n");
    proneTree(g);
    return return_val;
}

int MCTS::bestChild(shared_ptr <State> game){
    float highscore = 0.0;
    int choosed;
    for(int i=0;i<game->sonsStates.size();i++){
        float firstPart = float(game->sonsStates[i]->wins)/game->sonsStates[i]->allGames;
        float secondPart = this->exploreRate * sqrt((2 * log(game->allGames))/game->sonsStates[i]->allGames);
        if(highscore <= firstPart + secondPart){
            highscore = firstPart + secondPart;
            choosed = i;
        }
    }
    return choosed;
}
int MCTS::mcts(shared_ptr <State> game){
    if(game->ended != 0){
        return game->ended;
    }
    if(game->act != game->possibleMoves.size()){
        //printf("Exploring\n");
        UltimateTicTacToe newGame (game->state);
        //printf("Creating new State from game in recurent mcts\n");
        shared_ptr <State> newState = make_shared<State>(newGame);
        newState->state.makeMove(game->possibleMoves[game->act]);
        //printf("Pushing new State to vector\n");
        game->sonsStates.pb(newState);
        int ended = game->sonsStates[game->act]->state.isTerminated();
        if(ended != 0){
            if(ended == this->actualTree->state.who){
                ended = 100000;
                game->sonsStates[game->act]->wins += ended;
            }
            game->sonsStates[game->act]->allGames++;
            game->sonsStates[game->act]->ended = ended;
            game->act++;
            return ended;
        }
        game->sonsStates[game->act]->generate_moves();
        //printf("Pushed to vector\n");
        //game.sonsStates[game.act]->state.print();
        int result = game->sonsStates[game->act]->state.doRandomGame(0,this->actualTree->state.who);
        Move descendingMove = game->moveToDescend(game->sonsStates[game->act]);
        if(result == this->actualTree->state.who){
            Mast[descendingMove.hash()].f++;
            game->sonsStates[game->act]->wins++;
        }
        Mast[descendingMove.hash()].s++;
        game->sonsStates[game->act]->allGames++;
        game->act++;
        return result;
    } else {
        //printf("Exploiting\n");
        int sonToExplore = bestChild(game);
        //game.sonsStates[sonToExplore]->state.print();
        //printf("Calling reccurent mcts\n");
        int result = mcts(game->sonsStates[sonToExplore]);
        Move descendingMove = game->moveToDescend(game->sonsStates[sonToExplore]);
        if(result == this->actualTree->state.who){
            Mast[descendingMove.hash()].f++;
            game->sonsStates[sonToExplore]->wins++;
        }
        Mast[descendingMove.hash()].s++;
        game->sonsStates[sonToExplore]->allGames++;
        return result;
    }
}

void MCTS::reset(){
    actualTree = make_shared<State>();
}