#include "simulation.h"
#include <unistd.h>
GLFWwindow* window;
using namespace glm;
int simulate(vector <pair <float,float> > positions){
    // Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "graphics/SimpleVertexShader.vertexshader", "graphics/SimpleFragmentShader.fragmentshader" );

    // {
	// 	0.0f * 2.0f/maxx - 1.0f, 1500.0f * 2.0f/maxy - 1.0f, 0.0f,
    //     1000.0f * 2.0f/maxx - 1.0f, 2000.0f * 2.0f/maxy - 1.0f, 0.0f,
    //     2000.0f * 2.0f/maxx - 1.0f, 500.0f * 2.0f/maxy - 1.0f, 0.0f,
    //     3500.0f * 2.0f/maxx - 1.0f, 500.0f * 2.0f/maxy - 1.0f, 0.0f,
    //     5000.0f * 2.0f/maxx - 1.0f, 1500.0f * 2.0f/maxy - 1.0f, 0.0f,
    //     6999.0f * 2.0f/maxx - 1.0f, 1000.0f * 2.0f/maxy - 1.0f, 0.0f
	// };
    
    GLfloat maxx = 7000.0;
    GLfloat maxy = 3000.0;

	//build drawing
	GLfloat g_vertex_buffer_data[Constants::n_points * 3];
	int helper = 0;

	for(int i=0;i<Constants::n_points;i++){
		printf("points[%d]: %f %f\n",i,Constants::points[i].f,Constants::points[i].s);
		g_vertex_buffer_data[helper] = Constants::points[i].f * 2.0f/maxx;
		g_vertex_buffer_data[helper] -= 1.0f;
		g_vertex_buffer_data[helper+1] = Constants::points[i].s * 2.0f/maxy;
		g_vertex_buffer_data[helper+1] -= 1.0f;
		g_vertex_buffer_data[helper+2] = 0.0f;
		helper += 3;
		printf("g_vertex[%d]: %f\ng_vertex[%d]: %f\n",i,g_vertex_buffer_data[i],i+1,g_vertex_buffer_data[i+1]);
	}
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//build racket drawing
    static GLfloat g_vertex_buffer_racket[] = {
        positions[0].f * 2.0f/maxx - 1.0f, positions[0].s * 2.0f/maxy - 1.0f, 0.0f,
        positions[0].f * 2.0f/maxx - 1.0f, (positions[0].s + 40.0f) * 2.0f/maxy - 1.0f, 0.0f
    };
	Lander racket = Lander(0,0,mp(0.0,0.0),1000);
    GLuint vertexbufferP;

	int iter = 1;

    glGenBuffers(1, &vertexbufferP);
	do{

		// Clear the screen
		glClear( GL_COLOR_BUFFER_BIT );

		// Use our shader
		glUseProgram(programID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_LINE_STRIP, 0, 6); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);

        // Point drawing
        glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbufferP);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		//Draw Point
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferP);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_racket), g_vertex_buffer_racket, GL_STATIC_DRAW);

		glDrawArrays(GL_LINE_STRIP, 0, 2);
        g_vertex_buffer_racket[0] = positions[iter].f * 2.0f/maxx - 1.0f;
        g_vertex_buffer_racket[1] = positions[iter].s * 2.0f/maxy - 1.0f;
		g_vertex_buffer_racket[3] = positions[iter].f * 2.0f/maxx - 1.0f;
		g_vertex_buffer_racket[4] = (positions[iter].s + 40.0f) * 2.0f/maxy - 1.0f;

		glDisableVertexAttribArray(0);
        
		iter++;
        usleep(1000000.0);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}