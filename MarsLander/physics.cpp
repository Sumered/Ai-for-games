#include "physics.h"

int Constants::n_points;
int Constants::flat_idx;
const float Constants::mars_g = 3.711;
const float Constants::max_tilt = 15.0;
default_random_engine Constants::gen;

pair<float, float> Constants::points[1000];

void Constants::set_up(int np,int flatid){ n_points = np; flat_idx = flatid; gen.seed(time(NULL));}

void Constants::actualize_points(pair <float,float> p,int idx){ points[idx] = p; }

Move::Move(int rotation, int power){
    this->rotation = rotation;
    this->power = power;
}
Move::Move(){
    this->rotation = 0;
    this->power = 0;
}
Move Move::get_random_move(){
    uniform_int_distribution<int> distribution_angle(-15,15);
	uniform_int_distribution<int> distribution_power(-1,1);
    int rotate = distribution_angle(Constants::gen);
    int power = distribution_power(Constants::gen);
    return Move(rotate,power);
}
void Move::print(){
    printf("Rotation: %d Power: %d\n",this->rotation,this->power);
}

Lander::Lander(){}

Lander::Lander(int rotation, int power, pair<float, float> speed, int fuel){
    this->rotation = rotation;
    this->power = power;
    this->speed = speed;
    this->fuel = fuel;
}

void Lander::update(Move move){
    this->rotation += move.rotation;
    this->rotation = max(-90,min(90,this->rotation));
	if(this->fuel < this->power + move.power){ this->power = this->fuel; }
    else { this->power += move.power; }
    this->fuel -= this->power;
    this->power = min(4,max(0,this->power));
    this->speed.f += sin(-rotation*3.14/180.0)*(float)this->power;
    this->speed.s += cos(-rotation*3.14/180.0)*(float)this->power - Constants::mars_g;
}

void Lander::print(){
    printf("Power: %d Rotation: %d\nSpeed_x: %f Speed_y: %f\n",this->power,this->rotation,this->speed.f,this->speed.s);
}

bool Lander::is_inside(float pos_x,float pos_y){
	return pos_x >= 0.0 && pos_x <= 7000 && pos_y >= 0.0 && pos_y <= 3000;
}

float dist(pair<float, float> A, pair<float, float> B){ return sqrt((B.f-A.f)*(B.f-A.f) + (B.s-A.s)*(B.s-A.s)); }

bool lines_touching(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	float denominator = ((x2 - x1) * (y4 - y3)) - ((y2 - y1) * (x4 - x3));
	float numerator1 = ((y1 - y3) * (x4 - x3)) - ((x1 - x3) * (y4 - y3));
	float numerator2 = ((y1 - y3) * (x2 - x1)) - ((x1 - x3) * (y2 - y1));

	// Detect coincident lines (has a problem, read below)
	if (denominator == 0){ return numerator1 == 0 && numerator2 == 0; }

	float r = numerator1 / denominator;
	float s = numerator2 / denominator;

	return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
}

int collision(pair<float, float> A, pair<float, float> B){
	for(int i = 1; i<Constants::n_points; i++){
		if(lines_touching(A.f, A.s, B.f, B.s, Constants::points[i-1].f, Constants::points[i-1].s, Constants::points[i].f, Constants::points[i].s))
			return i;
	}
	return -1;
}

bool collision_on_flat(pair<float, float> A, pair<float, float> B){
	if(lines_touching(A.f, A.s, B.f, B.s, Constants::points[Constants::flat_idx-1].f, Constants::points[Constants::flat_idx-1].s,
     Constants::points[Constants::flat_idx].f, Constants::points[Constants::flat_idx].s))
		return true;
	return false;
}

pair<float, float> contact_point(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {

  // calculate the distance to intersection point
	float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
	float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));

	// if uA and uB are between 0-1, lines are colliding
	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) { return pair<float, float>(x1 + (uA * (x2-x1)), y1 + (uA * (y2-y1))); }
	else {
		printf("ERROR");
		return pair<float, float>(0.0,0.0);
	}
}

pair<float, float> get_contact_point(pair<float, float> A, pair<float, float>B){
	for(int i = 1; i<Constants::n_points; i++){
		if(lines_touching(A.f, A.s, B.f, B.s, Constants::points[i-1].f, Constants::points[i-1].s, Constants::points[i].f, Constants::points[i].s))
			return contact_point(A.f, A.s, B.f, B.s, Constants::points[i-1].f, Constants::points[i-1].s, Constants::points[i].f, Constants::points[i].s);
	}
	return pair<float, float>(0.0,0.0);
}