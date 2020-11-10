#include "physics.h"
class Individual {
public:
    vector <Move> moves;
    Individual();
    float simulate_playthrough(float start_x,float start_y,float speed_x,float speed_y,int fuel,int rotation,int power);
    static pair <Individual,Individual> cross_over(const Individual &first_indi,const Individual &second_indi);
    void mutate();
};

class Evolution {
public:
    static float start_x,start_y,speed_x,speed_y;
    static int fuel,rotation,power;
    static vector <pair <Individual,float> > population;
    static void set_up();
    static void set_up_params(float pos_x,float pos_y,float lander_speed_x,float lander_speed_y,int lander_fuel,int lander_rotation,int lander_power);
    static void merge_generations(const vector <pair <Individual,float> > &children);
    static void prep_for_next_round();
    static bool cmp(const pair<Individual,float> &lhs, const pair<Individual,float> &rhs);
    static pair <Move,float> evolution(Lander racket,float pos_x,float pos_y);
    static int wheel_of_fortune();
};