#include "simulation.h"
#include "evolution.h"
//g++ -I. main.cpp -o main simulation.cpp evolution.cpp physics.cpp graphics/common/shader.cpp -lGLEW  -framework OpenGL -lglfw

int main(){
    float points[] = {
		0.0f, 1500.0f, 0.0f,
        1000.0f, 2000.0f, 0.0f,
        2000.0f, 500.0f, 0.0f,
        3500.0f, 500.0f, 0.0f,
        5000.0f, 1500.0f, 0.0f,
        6999.0f, 1000.0f, 0.0f
	};
    Constants::set_up(6,3);
    Constants::actualize_points(mp(0.0f,1500.0f),0);
    Constants::actualize_points(mp(1000.0f,2000.0f),1);
    Constants::actualize_points(mp(2000.0f,500.0f),2);
    Constants::actualize_points(mp(3500.0f,500.0f),3);
    Constants::actualize_points(mp(5000.0f,1500.0f),4);
    Constants::actualize_points(mp(6999.0f,1000.0f),5);
    Evolution::set_up();
    Lander racket = Lander(0,0,mp(0.0,0.0),4000);
    pair <float,float> act = mp(5000,2500);
    pair <float,float> prev = mp(act.f - 1.0, act.s - 1.0);
    vector <pair <float,float> > positions;
    for(int i=0;i<150;i++){
        positions.pb(act);
        pair <Move,float> results = Evolution::evolution(racket,act.f,act.s);
        printf("score after round %d: %f\n",i,results.s);
        float move_x = sin(-(racket.rotation+results.f.rotation)*3.14/180.0)*(float)(racket.power+results.f.power)/2.0 + racket.speed.f;
        float move_y = (cos(-(racket.rotation+results.f.rotation)*3.14/180.0)*(float)(racket.power+results.f.power) - Constants::mars_g)/2.0 + racket.speed.s;
        prev = act; act.f += move_x; act.s += move_y;
        if(collision(prev,act) == Constants::flat_idx){ printf("FUCK YEAH!!!\n"); }
        if(collision(prev,act) != -1){break;}
        printf("X pos: %f\n Y pos: %f\nX speed: %f\nY speed: %f\n",act.f,act.s,racket.speed.f,racket.speed.s);
        racket.update(results.f);
    }
    simulate(positions);
}