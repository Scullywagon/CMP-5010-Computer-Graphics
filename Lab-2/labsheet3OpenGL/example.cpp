#include <iostream>
using namespace std;
//--- OpenGL ---
#include "GL/glew.h"
#include "GL/glfw.h"

//Shader.h has been created using common glsl structure
#include "Shader.h"   // include shader header file, this is not part of OpenGL

unsigned int m_vaoID;		//Vertex Array Object (VAO), identifier
unsigned int m_vboID[3];	// three VBOs

const int numOfVerts = 6;

Shader* myShader;  ///shader object 

#include <iostream>
using namespace std;

//Variables for the dimensions of the window in pixels.
int screenWidth=480, screenHeight=480;

//OPENGL FUNCTION PROTOTYPES
void display();				//called in winmain to draw everything to the screen
void init();				//called in winmain when the program starts.
void createGeometry();      //used to create Vertex Array Objects

/*************    START OF OPENGL FUNCTIONS   ****************/

void display()									
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(myShader->handle());  // use the shader
	
		glBindVertexArray(m_vaoID);		// select first VAO
		glDrawArrays(GL_LINES, 0, numOfVerts);	// draw first object

		glBindVertexArray(0); //unbind the vertex array object
	glUseProgram(0); //turn off the current shader

	glutSwapBuffers();
}

void createGeometry()
{
	// First simple object
	float vert[numOfVerts * 3];	// vertex array
  float col[numOfVerts * 3];	// color array

	int index = 0;
	vert[index++] = -0.25; vert[index++] = 0.0; vert[index++] =0.0;
	vert[index++] = 0.0; vert[index++] = 0.5; vert[index++] =0.0;
	vert[index++] = 0.25; vert[index++] = 0.0; vert[index++]= 0.0;
	vert[index++] = -1.25; vert[index++] = 0.0; vert[index++] =0.0;
	vert[index++] = 0.0; vert[index++] = 0.5; vert[index++] =0.0;
	vert[index++] = -0.25; vert[index++] = 0.0; vert[index++]= 0.0;

	
	index = 0;
	col[index++] = 1.0; col[index++] = 0.0; col[index++] = 0.0; //r,g,b values for each vertex
	col[index++] = 1.0; col[index++] = 0.0; col[index++] = 0.0;
	col[index++] = 1.0; col[index++] = 0.0; col[index++] = 0.0;
	col[index++] = 1.0; col[index++] = 0.0; col[index++] = 0.0; //r,g,b values for each vertex
	col[index++] = 1.0; col[index++] = 0.0; col[index++] = 0.0;
	col[index++] = 1.0; col[index++] = 0.0; col[index++] = 0.0;

	
	//VAO allocation
	glGenVertexArrays(1, &m_vaoID);

	// First VAO setup
	glBindVertexArray(m_vaoID);
	
	glGenBuffers(2, m_vboID); // we need two VBOs - one for the vertices and one for the colours
			//these are associated with the first vertex array object - m_vaoID
	
	//Lets set up the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[0]);

	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, numOfVerts * 3 *sizeof(GLfloat), vert, GL_STATIC_DRAW);

	//set the position - linked to the position shader input
	GLint vertexLocation = glGetAttribLocation(myShader->handle(), "in_Position");
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0); 
	
	//Now set up the colours
	glBindBuffer(GL_ARRAY_BUFFER, m_vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, numOfVerts * 3 *sizeof(GLfloat), col, GL_STATIC_DRAW);

	//set the colour - linked to the colour shader input.
	GLint colorLocation = glGetAttribLocation(myShader->handle(), "in_Color");
	glEnableVertexAttribArray(colorLocation);
	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void init()
{

	glClearColor(0.0,0.0,0.0,0.0);						//sets the clear colour to yellow
					//glClear(GL_COLOR_BUFFER_BIT) in the display function//will clear the buffer to this colour.

	// Shaders
	myShader = new Shader;
	if(!myShader->load("Basic", "./glslfiles/basic.vert", "./glslfiles/basic.frag"))
	{
		cout << "failed to load shader" << endl;
	}												

	createGeometry();

	
}


/**************** END OPENGL FUNCTIONS *************************/

// FREEGLUT WINDOW SET UP
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL FreeGLUT Example");
	
	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << "OpenGL Version: " << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	//initialise the objects for rendering
	init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}


