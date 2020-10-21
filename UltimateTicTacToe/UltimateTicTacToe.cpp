#include "UltimateTicTacToe.h"

map <int,pair <int,int> > Mast;

map <int,double> Pap;

double alfa = 0.1;

float greed = 0.8;

Move::Move(int p,int xa,int ya){ place = p;x = xa;y = ya; }

bool Move::operator <(const Move& rhs) const{
    if(place != rhs.place){ return place < rhs.place; }
    else if(x != rhs.x){ return x < rhs.x; }
    else if(y != rhs.y){ return y < rhs.y; }
    else{ return true; }
}
bool Move::operator ==(const Move& rhs) const{
    return place == rhs.place && x == rhs.x && y == rhs.y;
}
int Move::hash() const{
    return place*9+x*3+y*3;
}

UltimateTicTacToe::UltimateTicTacToe(){
    where = -1;who = 1;
    for(int i=0;i<81;i++){ board[i/9][(i%9)/3][i%3] = 0; }
    for(int i=0;i<9;i++){ state[i/3][i%3] = 0; full[i/3][i%3] = 0; }
}
UltimateTicTacToe::UltimateTicTacToe(const UltimateTicTacToe &UT){
    for(int i=0;i<81;i++){
        board[i/9][(i%9)/3][i%3] = UT.board[i/9][(i%9)/3][i%3];
    }
    for(int i=0;i<9;i++){
        state[i/3][i%3] = UT.state[i/3][i%3];
        full[i/3][i%3] = UT.full[i/3][i%3];
    }
    where = UT.where; who = UT.who;
}
void UltimateTicTacToe::print() const{
    int place=0,x=0,y=0,bon=0;
    for(int i=0;i<81;i++){
        if(i%3==0 && i!=0){ place++; y=0; printf(" ");}
        if(i%9==0 && i!=0){ x++; place=0+bon; printf("\n");}
        if(i%27==0 && i!=0){ x=0; bon+=3; place = bon;  printf("\n");}
        if(board[place][x][y] == 1){printf("O");}
        else if(board[place][x][y] == -1){printf("X");}
        else {printf("-");}
        y++;
    }
    printf("\n\n");
}
int UltimateTicTacToe::checkBoard(int pos){
    for(int i=0;i<3;i++){
        if(board[pos][i][0] != 0 && board[pos][i][0] == board[pos][i][1] && board[pos][i][1] == board[pos][i][2]){ return board[pos][i][0];}
        if(board[pos][0][i] != 0 && board[pos][0][i] == board[pos][1][i] && board[pos][1][i] == board[pos][2][i]){ return board[pos][0][i];}
    }
    if(board[pos][0][0] != 0 && board[pos][0][0] == board[pos][1][1] && board[pos][1][1] == board[pos][2][2]){ return board[pos][0][0]; }
    if(board[pos][0][2] != 0 && board[pos][0][2] == board[pos][1][1] && board[pos][1][1] == board[pos][2][0]){ return board[pos][0][2]; }
    return 0;
}
int UltimateTicTacToe::isTerminated(){
    for(int i=0;i<3;i++){
        if(state[0][i] != 0 && state[0][i] == state[1][i] && state[1][i] == state[2][i]){ return state[0][i]; }
        if(state[i][0] != 0 && state[i][0] == state[i][1] && state[i][1] == state[i][2]){ return state[i][0]; }
    }
    if(state[0][0] != 0 && state[0][0] == state[1][1] && state[1][1] == state[2][2]){ return state[0][0]; }
    if(state[0][2] != 0 && state[0][2] == state[1][1] && state[1][1] == state[2][0]){ return state[0][2]; }
    
    int passCounter = 0;
    for(int i=0;i<9;i++){
        if(full[i/3][i%3]==9){
            passCounter++;
            continue;
        }
        if(state[i/3][i%3]!=0){
            passCounter++;
        }
    }
    if(passCounter == 9){return -2;}
    return 0;
}
int UltimateTicTacToe::makeMove(Move m){
    board[m.place][m.x][m.y] = who;
    full[m.place/3][m.place%3]++;
    who *= -1;
    state[m.place/3][m.place%3] = checkBoard(m.place);
    if(state[m.x][m.y] != 0 || full[m.x][m.y] == 9){ where = -1; } //changed
    else{ where = m.x*3 + m.y; }
    return isTerminated();
}
void UltimateTicTacToe::generateAllPossibleMoves(vector <Move> &res) const {
    if(where != -1){
        for(int i=0;i<9;i++){
            if(board[where][i/3][i%3] == 0){ 
                Move mv = Move(where,i/3,i%3);
                res.pb(mv);
            }
        }
    } else {
        for(int i=0;i<9;i++){
            for(int j=0;j<9;j++){
                if(state[i/3][i%3] == 0 && board[i][j/3][j%3] == 0){
                    Move mv = Move(i,j/3,j%3);
                    res.pb(mv);
                }
            }
        }
    }
}
int UltimateTicTacToe::doRandomGame(int mast,int pap) const{
    UltimateTicTacToe fakeGame = UltimateTicTacToe((*this));
    int winner;
    vector <int> movesDone;
    while(true){
        vector <Move> possibleMoves;
        fakeGame.generateAllPossibleMoves(possibleMoves);
        if(mast == fakeGame.who){
            pair<int,Move> p = fakeGame.makeRandomMastMove(possibleMoves);
            movesDone.pb(p.s.hash());
            if(p.f != 0){winner = p.f;break;}
            continue;
        } else if(pap == fakeGame.who){
            pair<int,Move> p = fakeGame.makeRandomPapMove(possibleMoves);
            movesDone.pb(p.s.hash());
            if(p.f != 0){winner = p.f;break;}
            continue;
        }else {
            winner = fakeGame.makeRandomMove(possibleMoves);
            if(winner != 0){break;}
            continue;
        }
    }
    if(mast != 0){
        for(int i=0;i<movesDone.size();i++){
            if(Mast.find(movesDone[i]) == Mast.end()){
                Mast[movesDone[i]] = mp(0,0);
            }
            if(winner == mast){
                Mast[movesDone[i]].f++;
            }
            Mast[movesDone[i]].s++;
        }
    }
    if(pap != 0){
        for(int i=0;i<movesDone.size();i++){
            if(Pap.find(movesDone[i]) == Pap.end()){
                Pap[movesDone[i]] = 0.0;
            }
            if(winner == mast){
                Pap[movesDone[i]] += alfa;
            } else {
                Pap[movesDone[i]] -= alfa;
            }
        }
    }
    return winner;
}
int UltimateTicTacToe::makeRandomMove(const vector <Move> &possibleMoves){
    srand (time(NULL));
    if(possibleMoves.size() == 0){
        printf("BUMMMMMM\n");
        this->print();
        for(int i=0;i<9;i++){
            if(i%3 == 0){ printf("\n"); }
            printf("%d ",full[i/3][i%3]);
        }
        printf("\n");
        for(int i=0;i<9;i++){
            if(i%3 == 0){ printf("\n"); }
            printf("%d ",state[i/3][i%3]);
        }
        printf("\n");
        printf("who: %d where: %d\n",who,where);
    }
    int i = rand()%possibleMoves.size();
    return makeMove(possibleMoves[i]);
}

pair <int,Move> UltimateTicTacToe::makeRandomMastMove(const vector <Move> &possibleMoves){
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0.0, 1.0);
    double x = dis(gen);
    if(x < greed){ //here is e greedy param
        float highscore = 0.0;
        int choosed = 0;
        for(int i=0;i<possibleMoves.size();i++){
            float score = float(Mast[possibleMoves[i].hash()].f)/Mast[possibleMoves[i].hash()].s;
            if(highscore < score){
                highscore = score;
                choosed = i;
            }
        }
        return mp(makeMove(possibleMoves[choosed]),possibleMoves[choosed]);
    } else {
        int i = rand()%possibleMoves.size();
        return mp(makeMove(possibleMoves[i]),possibleMoves[i]);
    }
}

pair <int,Move> UltimateTicTacToe::makeRandomPapMove(const vector <Move> &possibleMoves){
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(0.0, 1.0);
    int probabilities[possibleMoves.size()];
    double divider = 0.0;
    for(int i=0;i<possibleMoves.size();i++){
        divider += exp(Pap[possibleMoves[i].hash()]);
    }
    for(int i=0;i<possibleMoves.size();i++){
        probabilities[i] = exp(Pap[possibleMoves[i].hash()])/divider;
    }
    double X= dis(gen);
    double sum = 0;
    for(int i=0;i<possibleMoves.size();i++){
        if(X >= sum && X < sum+probabilities[i]){
            return mp(makeMove(possibleMoves[i]),possibleMoves[i]);
        }
        sum += probabilities[i];
    }
    return mp(makeMove(possibleMoves[0]),possibleMoves[0]);
}

bool UltimateTicTacToe::operator==(const UltimateTicTacToe& other){
    if(who != other.who){ return false; }
    if(where != other.where){ return false; }
    for(int i=0;i<81;i++){
        if(board[i/9][(i%9)/3][i%3] != other.board[i/9][(i%9)/3][i%3]){ return false; }
    }
    return true;
}

UltimateTicTacToe& UltimateTicTacToe::operator=(const UltimateTicTacToe& rhs){
    this->who = rhs.who; this->where = rhs.where;
    for(int i=0;i<81;i++){
        this->board[i/9][(i%9)/3][i%3] = rhs.board[i/9][(i%9)/3][i%3];
    }
    for(int i=0;i<9;i++){
        this->state[i/3][i%3] = rhs.state[i/3][i%3];
        this->full[i/3][i%3] = rhs.full[i/3][i%3];
    }
    return *this;
}