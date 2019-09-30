///////////////////////////////////////////
////Type your name and student ID here/////
////Name: Wong Sin Yi
////Student ID: 1155110677

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;

float x_delta = 0.1f;
int x_press_num = 0;

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void keyboard(unsigned char key, int x, int y)
{
	//TODO:
}

GLuint groundVaoID;
GLuint groundVboID;

void sendDataToOpenGL()
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs
	const GLfloat ground[] =
	{
		-1.0f, +0.0f, -1.0f,      +0.0f, +0.0f, +1.0f,
		-1.0f, +0.0f, +1.0f,      +0.0f, +0.0f, +1.0f,
		+1.0f, +0.0f, -1.0f,      +0.0f, +0.0f, +1.0f,
		-1.0f, +0.0f, +1.0f,      +0.0f, +0.0f, +1.0f,
		+1.0f, +0.0f, +1.0f,      +0.0f, +0.0f, +1.0f,
		+1.0f, +0.0f, -1.0f,      +0.0f, +0.0f, +1.0f,
	};

	glGenVertexArrays(1, &groundVaoID);
	glBindVertexArray(groundVaoID);
	glGenBuffers(1, &groundVboID);
	glBindBuffer(GL_ARRAY_BUFFER, groundVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));



}

void matrix(string obj) {


	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	glm::mat4 modelRotationMatrix = glm::mat4(1.0f);
	glm::mat4 modelScalingMatrix = glm::mat4(1.0f);

	if (obj == "ground") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -1.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(4.0f, 1.0f, 4.0f));
	}

	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	GLint modelRotateMatrixUniformLocation = glGetUniformLocation(programID, "modelRotationMatrix");
	GLint modelScalingMatrixUniformLocation = glGetUniformLocation(programID, "modelScalingMatrix");

	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glUniformMatrix4fv(modelRotateMatrixUniformLocation, 1, GL_FALSE, &modelRotationMatrix[0][0]);
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1, GL_FALSE, &modelScalingMatrix[0][0]);

	glm::mat4 Projection = glm::perspective(30.0f, 1.0f, 2.0f, 20.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, -2.0, 0),
		glm::vec3(0, 0, -10),
		glm::vec3(0, 1, 0)
	);
	glm::mat4 Model = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.5f, -5.0f));;
	glm::mat4 mvp = Projection * View * Model;
	GLuint MatrixID = glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
}


void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here
	//specify the background color RGBA
	glClearColor(0.0f, 0.0f, 0.1f, 0.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Deph test
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	matrix("ground");
	glBindVertexArray(groundVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char *argv[])
{
	/*Initialization*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Assignment 1");
	glewInit();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

	return 0;
}