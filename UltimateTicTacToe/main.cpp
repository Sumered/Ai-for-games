#include "MonteCarlo.h"

float playGameWithAI(AI* firstAI,AI* secondAI,int numberOfGames,int timeLimit,bool circleStarts,bool debug){
    int wins = 0,draws = 0;
    for(int i=0;i<numberOfGames;i++){
        UltimateTicTacToe game = UltimateTicTacToe();
        printf("Start round %d:\n\n",i);
        while(true){
            int result;
            if(circleStarts){
                Move choosed = firstAI->findMove(game,timeLimit);
                printf("After AI\n");
                printf("place: %d x: %d y: %d\n",choosed.place,choosed.x,choosed.y);
                result = game.makeMove(choosed);
                if(debug){ game.print(); }
                if(result != 0 && result != -2) { wins++; break; }
                if(result == -2){ draws++; break; }

                if(secondAI == NULL){
                    vector <Move> possibleMoves; game.generateAllPossibleMoves(possibleMoves);
                    result = game.makeRandomMove(possibleMoves);
                } else {
                    Move chosen = secondAI->findMove(game,timeLimit);
                    result = game.makeMove(chosen);
                }
                if(debug){ game.print(); }
                if(result == -2){ draws++; }
                if(result != 0){break;}
            }else{
                if(secondAI == NULL){
                    vector <Move> possibleMoves; game.generateAllPossibleMoves(possibleMoves);
                    result = game.makeRandomMove(possibleMoves);
                } else {
                    Move chosen = secondAI->findMove(game,timeLimit);
                    result =game.makeMove(chosen);
                }
                if(debug){ game.print(); }
                if(result == -2){ draws++; }
                if(result != 0){ break;}
                Move choosed = firstAI->findMove(game,timeLimit);
                result = game.makeMove(choosed);
                if(debug){ game.print(); }
                if(result != 0 && result != -2){wins++; break;}
                if(result == -2){draws++; break;}
            }
        }
        firstAI->reset();
        Mast.clear();
        if(secondAI != NULL){ secondAI->reset(); }
        printf("WINS: %d\nDRAWS: %d\n",wins,draws);
    }
    return wins/numberOfGames;
}


int main(){
    //firstAI,secondAI,numberOfGames,timeLimitPerMove,whostarts - first or second,debug mode
    MCTS firstAI (0.1);
    FlatMonteCarlo secondAI;
    playGameWithAI((&firstAI),(&secondAI),10,1,true,true);
    //Mcts outperforms flat mcts 7/8 wins per 10 games for mcts
    //Mcts vs random 8 wins
    //after implementing MAST MCTS performance against random: 9 wins
    //after implementing MAST MCTS performance against flat: 7wins 1 draw/8wins not really helping  
}