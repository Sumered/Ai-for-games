default: tutorial02

%: %.cpp
	g++ -I. $< -o $@ common/shader.cpp -lGLEW  -framework OpenGL -lglfw 
clean:
	rm a.out *.o *~ tutorial02
