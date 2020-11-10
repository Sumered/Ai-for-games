#include "evolution.h"

vector <pair <Individual,float> > Evolution::population;
float Evolution::start_x,Evolution::start_y,Evolution::speed_x,Evolution::speed_y;
int Evolution::fuel,Evolution::rotation,Evolution::power;

Individual::Individual(){
    for(int i=0;i<Constants::indiv_size;i++){
        moves.pb(Move::get_random_move());
    }
}

float Individual::simulate_playthrough(float start_x,float start_y,float speed_x,float speed_y,int fuel,int rotation,int power){
    Lander racket = Lander(rotation,power,mp(speed_x,speed_y),fuel);
    pair <float,float> prev = mp(start_x - 1.0, start_y - 1.0);
    pair <float,float> act = mp(start_x, start_y);
    int iter = 0;
    while(iter < this->moves.size() && collision(prev,act) == -1){
        if(!Lander::is_inside(act.f,act.s)){ return INF; }
        Move move = this->moves[iter];
        if(racket.fuel == 0){ move.power = 0; }
        float move_x = sin(-(rotation+move.rotation)*3.14/180.0)*(float)(racket.power+move.power)/2.0 + racket.speed.f;
        float move_y = (cos(-(rotation+move.rotation)*3.14/180.0)*(float)(racket.power+move.power) - Constants::mars_g)/2.0 + racket.speed.s;
        prev = act; act.f += move_x; act.s += move_y;
        racket.update(move);
        iter++;
    }
    int collision_idx = collision(prev,act);
    if(collision_idx == -1){
        while(collision_idx == -1){
            if(!Lander::is_inside(act.f,act.s)){ return INF; }
            float move_x = (sin(-rotation*3.14/180.0)*(float)racket.power)/2.0 + racket.speed.f;
            float move_y = (cos(-rotation*3.14/180.0)*(float)racket.power - Constants::mars_g)/2.0 + racket.speed.s;
            racket.fuel -= racket.power;
            if(racket.fuel < racket.power){ racket.power = racket.fuel; }
            prev = act; act.f += move_x; act.s += move_y;
            collision_idx = collision(prev,act);
        }
    }
    if(collision_idx == Constants::flat_idx){
        if(abs(racket.speed.f) <= 20.0 && abs(racket.speed.s) <= 40 && racket.rotation == 0){ return 0.0; }
        else { return abs(racket.speed.f) + abs(racket.speed.s) + abs(racket.rotation); }
    } else{
        if(collision_idx < Constants::flat_idx){
            pair <float,float> point_of_contact = contact_point(prev.f, prev.s, act.f, act.s, Constants::points[collision_idx-1].f, 
                Constants::points[collision_idx-1].s, Constants::points[collision_idx].f, Constants::points[collision_idx].s);
            float result = dist(point_of_contact,Constants::points[collision_idx]);
            point_of_contact = Constants::points[collision_idx];
            collision_idx++;
            while(collision_idx != Constants::flat_idx){
                result += dist(point_of_contact,Constants::points[collision_idx]);
                point_of_contact = Constants::points[collision_idx];
                collision_idx++;
            }
            return result + abs(racket.speed.f) + abs(racket.speed.s) + abs(racket.rotation);
        } else {
            pair <float,float> point_of_contact = contact_point(prev.f, prev.s, act.f, act.s, Constants::points[collision_idx-1].f,
                Constants::points[collision_idx-1].s, Constants::points[collision_idx].f, Constants::points[collision_idx].s);
            float result = dist(point_of_contact,Constants::points[collision_idx-1]);
            point_of_contact = Constants::points[collision_idx-1];
            collision_idx--;
            while(collision_idx != Constants::flat_idx){
                result += dist(point_of_contact,Constants::points[collision_idx-1]);
                point_of_contact = Constants::points[collision_idx-1];
                collision_idx--;
            }
            return result + abs(racket.speed.f) + abs(racket.speed.s) + abs(racket.rotation);
        }
    }
    return 0.0;
}

pair <Individual,Individual> Individual::cross_over(const Individual &first_indi,const Individual &second_indi){
    uniform_int_distribution<int> distribution_point(0,Constants::indiv_size-1);
    int cross_point = distribution_point(Constants::gen);
    Individual new_first = Individual();
    Individual new_second = Individual();
    for(int i=0;i<Constants::indiv_size;i++){
        if(i <= cross_point){
            new_first.moves[i] = first_indi.moves[i];
            new_second.moves[i] = second_indi.moves[i];
        } else {
            new_first.moves[i] = second_indi.moves[i];
            new_second.moves[i] = first_indi.moves[i];
        }
    }
    return mp(new_first,new_second);
}

void Individual::mutate(){
    uniform_int_distribution<int> distribution_mutation(0,100);
    for(int i=0;i<Constants::indiv_size;i++){
        int roll = distribution_mutation(Constants::gen);
        if(roll <= Constants::mutation_chance){
            this->moves[i] = Move::get_random_move(); // can be other way maybe should try it
        }
    }
}

bool Evolution::cmp(const pair<Individual,float> &lhs, const pair<Individual,float> &rhs){
    return lhs.s < rhs.s;
}

void Evolution::set_up(){
    for(int i=0;i<Constants::population_size;i++){
        population.pb(mp(Individual(),INF));
    }
}

void Evolution::set_up_params(float pos_x,float pos_y,float lander_speed_x,float lander_speed_y,int lander_fuel,int lander_rotation,int lander_power){
    start_x = pos_x; start_y = pos_y; speed_x = lander_speed_x; speed_y = lander_speed_y;
    fuel = lander_fuel; rotation = lander_rotation; power = lander_power;
}

int Evolution::wheel_of_fortune(){
    uniform_int_distribution<int> distribution_combatants(0,Constants::population_size-1);
    vector <int> choosen_combatants;
    for(int i=0;i<Constants::number_of_combatants;i++){
        choosen_combatants.pb(distribution_combatants(Constants::gen));
    }
    float highscore = INF;
    int winner;
    for(int i=0;i<Constants::number_of_combatants;i++){
        if(highscore > population[choosen_combatants[i]].s){
            highscore = population[choosen_combatants[i]].s;
            winner = i;
        }
    }
    return winner;
}

void Evolution::merge_generations(const vector <pair <Individual,float> > &children){
    int parents_idx = 0, children_idx = 0;
    vector <pair <Individual,float> > chosen_ones;
    while(parents_idx + children_idx < Constants::population_size){
        if(population[parents_idx].s <= children[children_idx].s){
            chosen_ones.pb(population[parents_idx]);
            parents_idx++;
        } else {
            chosen_ones.pb(children[children_idx]);
            children_idx++;
        }
    }
    // population.clear();
    // for(int i=0;i<Constants::population_size;i++){
    //     population.pb(chosen_ones[i]);
    // }
    population = chosen_ones; //may be not working should check
}

void Evolution::prep_for_next_round(){
    //printf("Deleting first move in individuals\n");
    for(int i=0;i<Constants::population_size - Constants::number_of_randoms_injected;i++){
        population[i].f.moves.erase(population[i].f.moves.begin()+0);
        population[i].f.moves.pb(Move::get_random_move());
    }
    //printf("Deleting individuals\n");
    for(int i=0;i<Constants::number_of_randoms_injected;i++){
        population.pop_back();
    }
    //printf("Adding random individuals\n");
    for(int i=0;i<Constants::number_of_randoms_injected;i++){
        population.pb(mp(Individual(),INF));
    }
}

pair <Move,float> Evolution::evolution(Lander racket,float pos_x,float pos_y){
    time_t start_time = time(NULL);
    set_up_params(pos_x,pos_y,racket.speed.f,racket.speed.s,racket.fuel,racket.rotation,racket.power);
    //eval population
    for(int i=0;i<Constants::population_size;i++){
        population[i].s = population[i].f.simulate_playthrough(start_x,start_y,speed_x,speed_y,fuel,rotation,power);
    }
    printf("Evolution loop started\n");
    while(time(NULL) - start_time <= 0.999){
        //sort
        sort(population.begin(),population.end(),cmp);
        //do some funny selection, crossover and mutation
        vector <pair <Individual,float> > children;
        for(int i=0;i<Constants::population_size;i++){
            int father = wheel_of_fortune();
            int mother = wheel_of_fortune();
            pair <Individual,Individual> offspring = Individual::cross_over(population[father].f,population[mother].f);
            offspring.f.mutate(); offspring.s.mutate();
            children.pb(mp(offspring.f,INF)); children.pb(mp(offspring.s,INF));
        }
        //printf("Children created\n");
        //evaluate children
        for(int i=0;i<Constants::population_size;i++){
            children[i].s = children[i].f.simulate_playthrough(start_x,start_y,speed_x,speed_y,fuel,rotation,power);
        }
        //printf("Children evaluated\n");
        //sort children
        sort(children.begin(),children.end(),cmp);
        //merge population
        //printf("Merging generations\n");
        merge_generations(children);
        //printf("Generations merged\n");
    }
    printf("Evolution loop ended\n");
    sort(population.begin(),population.end(),cmp);
    //printf("Obtaining result\n");
    pair <Move,float> result = mp(population[0].f.moves[0],population[0].s);
    //printf("Preparation for next round\n");
    prep_for_next_round();
    return result;
}
