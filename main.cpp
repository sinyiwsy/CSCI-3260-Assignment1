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

float y_delta = 0.1f;
int y_press_num = 0;

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
	if (key == 'w')
	{
		y_press_num -= 1;
	}
	if (key == 's')
	{
		y_press_num += 1;
	}
}



GLuint groundVaoID, groundVboID;
GLuint starVaoID, starVboID, starIndicesVboID;
GLuint platformVaoID, platformVboID, platformIndicesVboID;
GLuint gateVaoID, gateVboID, gateIndicesVboID;
GLuint peopleVaoID, peopleVboID, peopleIndicesVboID;

void sendDataToOpenGL()
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs
	const GLfloat ground[] =
	{
		-1.0f, +0.0f, -1.0f,      +0.2f, +0.2f, +0.3f,
		-1.0f, +0.0f, +1.0f,      +0.52f, +0.37f, +0.26f,
		+1.0f, +0.0f, -1.0f,      +0.2f, +0.2f, +0.3f,
		-1.0f, +0.0f, +1.0f,      +0.52f, +0.37f, +0.26f,
		+1.0f, +0.0f, +1.0f,      +0.52f, +0.37f, +0.26f,
		+1.0f, +0.0f, -1.0f,      +0.2f, +0.2f, +0.3f,
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

	const GLfloat star[] =
	{
		-1.0f, -1.0f, -1.0f,      +0.3f, +0.3f, +0.3f,
		+0.0f, +1.0f, +0.0f,      +0.91f, +0.91f, +0.98f,
		+1.0f, -1.0f, -1.0f,      +0.7f, +0.7f, +0.7f,
		+1.0f, -1.0f, +1.0f,      +0.3f, +0.3f, +0.3f,
		-1.0f, -1.0f, +1.0f,      +0.7f, +0.7f, +0.7f,
		+0.0f, -3.0f, +0.0f,      +0.91f, +0.91f, +0.98f,
	};
	GLushort starIndices[] = {
	  0, 1, 2,
	  1, 2, 3,	
	  0, 1, 4, 
	  1, 4, 3,
	  0, 5, 2,
	  5, 2, 3,
	  0, 5, 4,
	  5, 4, 3,
	};

	glGenVertexArrays(1, &starVaoID);
	glBindVertexArray(starVaoID);
	glGenBuffers(1, &starVboID);
	glBindBuffer(GL_ARRAY_BUFFER, starVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(star), star, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &starIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, starIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(starIndices) * sizeof(GLushort), starIndices, GL_STATIC_DRAW);

	//platform
	const GLfloat platform[] =
	{
		-1.0f, +1.0f, -1.0f,  +0.5f, +0.5f, +0.5f,
		-1.0f, -1.0f, -1.0f,  +0.5f, +0.5f, +0.5f,
		+1.0f, +1.0f, -1.0f,  +0.8f, +0.8f, +0.8f,
		-1.0f, +1.0f, -1.0f,  +0.8f, +0.8f, +0.8f,
		-1.0f, -1.0f, +1.0f,  +0.5f, +0.5f, +0.5f,
		+1.0f, -1.0f, +1.0f,  +0.5f, +0.5f, +0.5f,
		+1.0f, +1.0f, +1.0f,  +0.8f, +0.8f, +0.8f,
		-1.0f, +1.0f, +1.0f,  +0.8f, +0.8f, +0.8f,
	};
	GLushort platformIndices[] = {
	  0,1,2,
	  0,2,3,
	  0,3,7,
	  0,4,7,
	  0,4,5,
	  0,5,1,
	  6,3,2,
	  6,3,7,
	  6,7,4,
	  6,5,4,
	  6,2,1,
	  6,5,1,

	};

	glGenVertexArrays(1, &platformVaoID);
	glBindVertexArray(platformVaoID);
	glGenBuffers(1, &platformVboID);
	glBindBuffer(GL_ARRAY_BUFFER, platformVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(platform), platform, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &platformIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, platformIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(platformIndices) * sizeof(GLushort), platformIndices, GL_STATIC_DRAW);

	//Gate
	const GLfloat gate[] =
	{
		//first part 0-12
		-0.9f, -0.6f, 0.0f,     +0.3f, +0.3f, +0.3f,
		-0.5f, -0.6f, 0.0f,     +0.3f, +0.3f, +0.3f,
		-0.95f, -0.2f, 0.0f,    +0.3f, +0.3f, +0.3f,
		-0.8f, -0.1f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-0.5f, -0.1f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-1.0f, 0.0f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-0.6f, 0.0f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-0.9f, 0.3f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-0.6f, 0.15f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-0.8f, 0.3f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-0.8f, 0.4f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-0.5f, 0.45f, 0.0f,		+0.3f, +0.3f, +0.3f,
		-0.5f, 0.15f, 0.0f,		+0.3f, +0.3f, +0.3f,
		//second part
		-0.42f, 0.47f, 0.0f, +0.3f, +0.3f, +0.3f,
		-0.45f, 0.15f, 0.0f, +0.3f, +0.3f, +0.3f,
		0.35f, 0.35f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.2f, 0.0f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.48f, 0.3f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.25f, -0.6f, 0.0f, +0.3f, +0.3f, +0.3f,
		0.67f, -0.6f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.62f, -0.15f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.8f, -0.1f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.75f, 0.05f, 0.0f, +0.3f, +0.3f, +0.3f,
		0.8f, 0.1f, 0.0f, +0.3f, +0.3f, +0.3f,
		1.0f, 0.35f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.9f, 0.4f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.8f, 0.6f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.65f,  0.4f, 0.0f, +0.3f, +0.3f, +0.3f,
		0.6f, 0.45f, 0.0f,+0.3f, +0.3f, +0.3f,
		0.5f, 0.4f, 0.0f, +0.3f, +0.3f, +0.3f,
	};
	GLushort gateIndices[] = {
		0, 1, 2,
		2, 1, 3,
		3, 1, 4, 
		3, 4, 6, 
		3, 6, 5, 
		5, 6, 7, 
		6, 7, 8, 
		7, 8, 9,
		8, 9, 10,
		8, 10, 12, 
		10, 12, 11,

		//second
		13,14,15,
		14,15,16,
		15,16,17,
		16,17,18,
		17,18,19,
		17,19,20,
		17,20,21,
		17,21,22,
		17,22,23,
		17,29,23,
		29,23,27,
		29,27,28,
		27,23,24,
		27,24,25,
		27,26,25,
		26,27,28,

	};

	glGenVertexArrays(1, &gateVaoID);
	glBindVertexArray(gateVaoID);
	glGenBuffers(1, &gateVboID);
	glBindBuffer(GL_ARRAY_BUFFER, gateVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gate), gate, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &gateIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gateIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gateIndices) * sizeof(GLushort), gateIndices, GL_STATIC_DRAW);

	const GLfloat people[] =
	{
		//head
		-0.2f, 0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		0.2f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.2f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.2f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f,
		//body and leg
		0.0f, 0.05f,0.0f, 0.0f, 0.0f, 0.0f,
		-0.3f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.3f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,

		0.3f, -0.55f,0.0f, 0.0f, 0.0f, 0.0f,
		0.09f, -0.55f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.2f, -1.0f,0.0f, 0.0f, 0.0f, 0.0f,

		-0.3f, -0.55f,0.0f, 0.0f, 0.0f, 0.0f,
		-0.09f, -0.55f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.2f, -1.0f,0.0f, 0.0f, 0.0f, 0.0f,

	};
	GLushort peopleIndices[] = {
	  //head
		0,1,2,
		1,2,3,
		//body and leg
		4,5,6,
		7,8,9,
		10,11,12,
	};

	glGenVertexArrays(1, &peopleVaoID);
	glBindVertexArray(peopleVaoID);
	glGenBuffers(1, &peopleVboID);
	glBindBuffer(GL_ARRAY_BUFFER, peopleVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(people), people, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &peopleIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, peopleIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(peopleIndices) * sizeof(GLushort), peopleIndices, GL_STATIC_DRAW);

}

void matrix(string obj) {


	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	glm::mat4 modelRotationMatrix = glm::mat4(1.0f);
	glm::mat4 modelScalingMatrix = glm::mat4(1.0f);

	if (obj == "ground") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 1.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(4.0f, 1.0f, 4.0f));
	}
	else if (obj == "star") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 5.0f, -2.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
		modelRotationMatrix =  glm::rotate(glm::mat4(), 7.0f, glm::vec3(0, 1, 0));
	}
	else if (obj == "gate") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 2.0f, -1.2f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(2.0f, 2.5f, 1.0f));
	}
	else if (obj == "platform") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -1.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(3.5f, 0.2f, 2.0f));
	}
	else if (obj == "platform2") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.2f, -1.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(3.25f, 0.2f, 1.75f));
	}
	else if (obj == "people") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 1.5f, y_delta * y_press_num));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.5f, 1.0f, 1.0f));
	}

	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	GLint modelRotateMatrixUniformLocation = glGetUniformLocation(programID, "modelRotationMatrix");
	GLint modelScalingMatrixUniformLocation = glGetUniformLocation(programID, "modelScalingMatrix");

	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glUniformMatrix4fv(modelRotateMatrixUniformLocation, 1, GL_FALSE, &modelRotationMatrix[0][0]);
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1, GL_FALSE, &modelScalingMatrix[0][0]);

	glm::mat4 Projection = glm::perspective(30.0f, 1.0f, 2.0f, 20.0f);
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 5.0, 0),
		glm::vec3(0, 0, -10),
		glm::vec3(0, -1, 0)
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
	glClearColor(0.0f, 0.0f, 0.15f, 0.0f); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Deph test
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	matrix("ground");
	glBindVertexArray(groundVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	matrix("platform");
	glBindVertexArray(platformVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	matrix("platform2");
	glBindVertexArray(platformVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	matrix("gate");
	glBindVertexArray(gateVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	matrix("people");
	glBindVertexArray(peopleVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	/*matrix("star");
	glBindVertexArray(starVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);*/

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