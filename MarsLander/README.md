# Mars Lander
To compile go to directory and type in terminal:\
g++ -I. main.cpp -o main simulation.cpp evolution.cpp physics.cpp graphics/common/shader.cpp -lGLEW  -framework OpenGL -lglfw \

After simulation end there will be simple openGL window showing how mars lander traveled.\
If You want to see another example then You need to set up code a little bit.\
First of all call Constants::set_up(x,y) where x is number of points and y is flat surface id (if flat is beetwen fourth and fifth then y will be 4 (counting from 0 this is fifth element).\
Then you need to call Constants::actualize_points(x,y) where x is pair of floats describing position of point in 2D space, and y is index where you want to put that point.\
