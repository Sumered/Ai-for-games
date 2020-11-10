#ifndef PHYSICS_H
#define PHYSICS_H
#include "common.h"

class Constants {
public:
    static const float mars_g;
    static const float max_tilt;
	static const int indiv_size = 50;
    static const int population_size = 100;
	static const int mutation_chance = 5; //in %
	static const int number_of_randoms_injected = 5;
	static const int number_of_combatants = 10;
	static default_random_engine gen;
    static int n_points;
    static int flat_idx;
    static pair<float, float> points[1000];
    static void set_up(int np,int flatid);
	static void actualize_points(pair <float,float> p ,int idx); 
};

class Move{
public:
	int power;
	int rotation;

	Move(int rotation, int power);
	Move();
	static Move get_random_move();
	void print();
};

class Lander{
public:
	int power;
	int rotation;
	int fuel;
	pair<float, float> speed;

	Lander(int rotation, int power, pair<float, float> speed, int fuel);
	Lander();
	void update(Move move);
	void print();
	static bool is_inside(float pos_x,float pos_y);
};

float dist(pair<float, float> A, pair<float, float> B);

bool lines_touching(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

int collision(pair<float, float> A, pair<float, float> B);

bool collision_on_flat(pair<float, float> A, pair<float, float> B);

pair<float, float> contact_point(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

pair<float, float> get_contact_point(pair<float, float> A, pair<float, float>B);

#endif