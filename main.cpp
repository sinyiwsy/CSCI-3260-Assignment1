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
#include <time.h>

using namespace std;
using glm::vec3;
using glm::mat4;

clock_t now_t;
int st_time = 0;
clock_t now_t2;
int ck_time = 0;
GLint programID;

float scale_delta = 0.01f;
int scale_press_num = 0;
float x_delta = 0.1f;
int x_press_num = 0;
int ah_animated = 0; 
int ready = 0;
int isClockwise = 1;

int season = 1; //0,1,2,3

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
	if (key == 's' && ah_animated == 0) {
		now_t = clock();
		ah_animated = 1;
	}
	if (key == 'a')
	{
		now_t2 = clock();
		isClockwise = 1;
	}
	if (key == 'd')
	{
		now_t2 = clock();
		isClockwise = -1;
	}
	if (key == 'q')
	{
		season = 0;
	}
	if (key == 'w')
	{
		season = 1;
	}
	if (key == 'e')
	{
		season = 2;
	}
	if (key == 'r')
	{
		season = 3;
	}
	if (key == 'm')
	{
		x_press_num++;
	}
	if (key == 'n')
	{
		x_press_num--;
	}
	if (key == 'x')
	{
		scale_press_num++;
	}
	if (key == 'z')
	{
		scale_press_num--;
	}
}



GLuint groundVaoID, groundVboID;
GLuint moonVaoID, moonVboID;
GLuint ahVaoID, ahVboID;
GLuint treeVaoID, treeVboID, treeIndicesVboID;
GLuint tree1VaoID, tree1VboID, tree1IndicesVboID;
GLuint tree2VaoID, tree2VboID, tree2IndicesVboID;
GLuint tree3VaoID, tree3VboID, tree3IndicesVboID;
GLuint starVaoID, starVboID, starIndicesVboID;
GLuint platformVaoID, platformVboID, platformIndicesVboID;
GLuint gateVaoID, gateVboID, gateIndicesVboID;
GLuint peopleVaoID, peopleVboID, peopleIndicesVboID;
GLuint nightVaoID, nightVboID;

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

	const GLfloat moon[] =
	{
		1.000f, 0.000f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.839f, -0.544f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.408f, 0.913f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.154f, -0.988f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.667f, 0.745f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.965f, -0.262f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.952f, -0.305f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.633f, 0.774f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.110f, -0.994f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.448f, 0.894f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.862f, -0.506f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.999f, -0.044f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.814f, 0.581f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.367f, -0.930f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.198f, 0.980f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.699f, -0.715f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.976f, 0.219f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.938f, 0.347f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.598f, -0.801f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.066f, 0.998f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.487f, -0.873f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.884f, 0.468f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.996f, 0.088f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.788f, -0.616f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.326f, 0.945f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.241f, -0.971f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.730f, 0.683f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.984f, -0.176f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.922f, -0.388f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.562f, 0.827f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.022f, -1.000f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.525f, 0.851f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.904f, -0.428f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.991f, -0.132f, 0.000f,  1.0f, 1.0f, 0.0f,
		0.760f, 0.650f, 0.000f,  1.0f, 1.0f, 0.0f,
		-0.284f, -0.959f, 0.000f,  1.0f, 1.0f, 0.0f,






	};

	glGenVertexArrays(1, &moonVaoID);
	glBindVertexArray(moonVaoID);
	glGenBuffers(1, &moonVboID);
	glBindBuffer(GL_ARRAY_BUFFER, moonVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(moon), moon, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	const GLfloat ah[] =
	{
		-0.7f, -0.2f, -0.0f, 0.7f, 0.5f, 0.5f,
		-0.65f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.55f, 0.3f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.5f, -0.2f, 0.0f, 0.7f, 0.5f, 0.5f,
		-0.4f, -0.2f, 0.0f, 0.7f, 0.5f, 0.5f,
		-0.55f, 0.3f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.67f, 0.0f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.65f, 0.05f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.4f, -0.1f,0.0f,0.7f, 0.5f, 0.5f,

		-0.3f, 0.3f, 0.0f, 0.7f, 0.5f, 0.5f,
		-0.2f, 0.3f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.3f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.3f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.2f,0.05f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.1f, 0.05f,0.0f,0.7f, 0.5f, 0.5f,
		-0.2f,0.05f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.1f, 0.05f,0.0f,0.7f, 0.5f, 0.5f,
		-0.05f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,

		-0.3f + 0.3f, 0.3f , 0.0f, 0.7f, 0.5f, 0.5f,
		-0.2f + 0.3f, 0.3f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.3f + 0.3f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.3f + 0.3f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.2f + 0.3f,0.05f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.1f + 0.3f, 0.05f,0.0f,0.7f, 0.5f, 0.5f,
		-0.2f + 0.3f,0.05f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.1f + 0.3f, 0.05f,0.0f,0.7f, 0.5f, 0.5f,
		-0.05f + 0.3f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,

		-0.3f + 0.6f, 0.3f , 0.0f, 0.7f, 0.5f, 0.5f,
		-0.2f + 0.6f, 0.3f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.3f + 0.6f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.3f + 0.6f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.2f + 0.6f,0.05f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.1f + 0.6f, 0.05f,0.0f,0.7f, 0.5f, 0.5f,
		-0.2f + 0.6f,0.05f, 0.0f,0.7f, 0.5f, 0.5f,
		-0.1f + 0.6f, 0.05f,0.0f,0.7f, 0.5f, 0.5f,
		-0.05f + 0.6f, -0.2f, 0.0f,0.7f, 0.5f, 0.5f,
	};

	glGenVertexArrays(1, &ahVaoID);
	glBindVertexArray(ahVaoID);
	glGenBuffers(1, &ahVboID);
	glBindBuffer(GL_ARRAY_BUFFER, ahVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ah), ah, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	const float slight = 0.6f;
	const float woodSize = 0.3f;
	const GLfloat tree[] =
	{
		//green
		-slight, +1.0f, -slight,  +0.2f, +0.2f, +0.3f, //
		-1.0f, -1.0f, -1.0f,  0.56f, 0.73f, 0.56f,
		+slight, +1.0f, -slight,  +0.2f, +0.2f, +0.3f, //
		+1.0f, -1.0f, -1.0f,  0.56f, 0.73f, 0.56f,
		-1.0f, -1.0f, +1.0f,  0.56f, 0.73f, 0.56f,
		+1.0f, -1.0f, +1.0f,  0.56f, 0.73f, 0.56f,
		+slight, +1.0f, +slight, +0.2f, +0.2f, +0.3f, //
		-slight, +1.0f, +slight,  +0.2f, +0.2f, +0.3f, //
		//wood
		-woodSize, +1.0f  -2.0f, -woodSize,  +0.65, +0.5, +0.39f, //
		-woodSize, -1.0f - 2.0f, -woodSize,   +0.65, +0.5, +0.39f,
		+woodSize, +1.0f - 2.0f, -woodSize,  +0.65, +0.5, +0.39f,//
		+woodSize, -1.0f - 2.0f, -woodSize,   +0.65, +0.5, +0.39f,
		-woodSize, -1.0f - 2.0f, +woodSize,   +0.65, +0.5, +0.39f,
		+woodSize, -1.0f - 2.0f, +woodSize, +0.65, +0.5, +0.39f,
		+woodSize, +1.0f - 2.0f, +woodSize, +0.65, +0.5, +0.39f, //
		-woodSize, +1.0f - 2.0f, +woodSize,  +0.65, +0.5, +0.39f, //
	};
	GLushort treeIndices[] = {
	  0,2,3,
	  0,1,3,
	  0,2,6,
	  0,7,6,
	  0,7,4,
	  0,1,4,
	  5,6,2,
	  5,3,2,
	  5,3,1,
	  5,1,4,
	  5,6,7,
	  5,4,7,

	8,10,11,
	8,9,11,
	8,10,14,
	8,15,14,
	8,15,12,
	8,9,12,
	13,14,10,
	13,11,10,
	13,11,9,
	13,9,12,
	13,14,15,
	13,12,15,
	};
	glGenVertexArrays(1, &treeVaoID);
	glBindVertexArray(treeVaoID);
	glGenBuffers(1, &treeVboID);
	glBindBuffer(GL_ARRAY_BUFFER, treeVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tree), tree, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &treeIndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, treeIndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(treeIndices) * sizeof(GLushort), treeIndices, GL_STATIC_DRAW);

	//pink
	const GLfloat tree1[] =
	{
		//green
		-slight, +1.0f, -slight,  +0.2f, +0.2f, +0.3f, //
		-1.0f, -1.0f, -1.0f,  0.73f, 0.56f, 0.56f,
		+slight, +1.0f, -slight,  +0.2f, +0.2f, +0.3f, //
		+1.0f, -1.0f, -1.0f,   0.73f, 0.56f, 0.56f,
		-1.0f, -1.0f, +1.0f,  0.73f, 0.56f, 0.56f,
		+1.0f, -1.0f, +1.0f,  0.73f, 0.56f, 0.56f,
		+slight, +1.0f, +slight, +0.2f, +0.2f, +0.3f, //
		-slight, +1.0f, +slight,  +0.2f, +0.2f, +0.3f, //
		//wood
		-woodSize, +1.0f - 2.0f, -woodSize,  +0.65, +0.5, +0.39f, //
		-woodSize, -1.0f - 2.0f, -woodSize,   +0.65, +0.5, +0.39f,
		+woodSize, +1.0f - 2.0f, -woodSize,  +0.65, +0.5, +0.39f,//
		+woodSize, -1.0f - 2.0f, -woodSize,   +0.65, +0.5, +0.39f,
		-woodSize, -1.0f - 2.0f, +woodSize,   +0.65, +0.5, +0.39f,
		+woodSize, -1.0f - 2.0f, +woodSize, +0.65, +0.5, +0.39f,
		+woodSize, +1.0f - 2.0f, +woodSize, +0.65, +0.5, +0.39f, //
		-woodSize, +1.0f - 2.0f, +woodSize,  +0.65, +0.5, +0.39f, //
	};
	GLushort tree1Indices[] = {
	  0,2,3,
	  0,1,3,
	  0,2,6,
	  0,7,6,
	  0,7,4,
	  0,1,4,
	  5,6,2,
	  5,3,2,
	  5,3,1,
	  5,1,4,
	  5,6,7,
	  5,4,7,

	8,10,11,
	8,9,11,
	8,10,14,
	8,15,14,
	8,15,12,
	8,9,12,
	13,14,10,
	13,11,10,
	13,11,9,
	13,9,12,
	13,14,15,
	13,12,15,
	};
	glGenVertexArrays(1, &tree1VaoID);
	glBindVertexArray(tree1VaoID);
	glGenBuffers(1, &tree1VboID);
	glBindBuffer(GL_ARRAY_BUFFER, tree1VboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tree1), tree1, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &tree1IndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tree1IndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tree1Indices) * sizeof(GLushort), tree1Indices, GL_STATIC_DRAW);

	//orange
	const GLfloat tree2[] =
	{
		//green
		-slight, +1.0f, -slight,  +0.2f, +0.2f, +0.3f, //
		-1.0f, -1.0f, -1.0f,  0.76f, 0.45f, 0.20f,
		+slight, +1.0f, -slight,  +0.2f, +0.2f, +0.3f, //
		+1.0f, -1.0f, -1.0f, 0.76f, 0.45f, 0.20f,
		-1.0f, -1.0f, +1.0f,  0.76f, 0.45f, 0.20f,
		+1.0f, -1.0f, +1.0f, 0.76f, 0.45f, 0.20f,
		+slight, +1.0f, +slight, +0.2f, +0.2f, +0.3f, //
		-slight, +1.0f, +slight,  +0.2f, +0.2f, +0.3f, //
		//wood
		-woodSize, +1.0f - 2.0f, -woodSize,  +0.65, +0.5, +0.39f, //
		-woodSize, -1.0f - 2.0f, -woodSize,   +0.65, +0.5, +0.39f,
		+woodSize, +1.0f - 2.0f, -woodSize,  +0.65, +0.5, +0.39f,//
		+woodSize, -1.0f - 2.0f, -woodSize,   +0.65, +0.5, +0.39f,
		-woodSize, -1.0f - 2.0f, +woodSize,   +0.65, +0.5, +0.39f,
		+woodSize, -1.0f - 2.0f, +woodSize, +0.65, +0.5, +0.39f,
		+woodSize, +1.0f - 2.0f, +woodSize, +0.65, +0.5, +0.39f, //
		-woodSize, +1.0f - 2.0f, +woodSize,  +0.65, +0.5, +0.39f, //
	};
	GLushort tree2Indices[] = {
	  0,2,3,
	  0,1,3,
	  0,2,6,
	  0,7,6,
	  0,7,4,
	  0,1,4,
	  5,6,2,
	  5,3,2,
	  5,3,1,
	  5,1,4,
	  5,6,7,
	  5,4,7,

	8,10,11,
	8,9,11,
	8,10,14,
	8,15,14,
	8,15,12,
	8,9,12,
	13,14,10,
	13,11,10,
	13,11,9,
	13,9,12,
	13,14,15,
	13,12,15,
	};
	glGenVertexArrays(1, &tree2VaoID);
	glBindVertexArray(tree2VaoID);
	glGenBuffers(1, &tree2VboID);
	glBindBuffer(GL_ARRAY_BUFFER, tree2VboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tree2), tree2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &tree2IndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tree2IndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tree2Indices) * sizeof(GLushort), tree2Indices, GL_STATIC_DRAW);

	//white
	const GLfloat tree3[] =
	{
		//green
		-slight, +1.0f, -slight,  +0.2f, +0.2f, +0.3f, //
		-1.0f, -1.0f, -1.0f,  0.9f, 0.9f, 0.9f,
		+slight, +1.0f, -slight,  +0.2f, +0.2f, +0.3f, //
		+1.0f, -1.0f, -1.0f,  0.9f, 0.9f, 0.9f,
		-1.0f, -1.0f, +1.0f,  0.9f, 0.9f, 0.9f,
		+1.0f, -1.0f, +1.0f,  0.9f, 0.9f, 0.9f,
		+slight, +1.0f, +slight, +0.2f, +0.2f, +0.3f, //
		-slight, +1.0f, +slight,  +0.2f, +0.2f, +0.3f, //
		//wood
		-woodSize, +1.0f - 2.0f, -woodSize,  +0.65, +0.5, +0.39f, //
		-woodSize, -1.0f - 2.0f, -woodSize,   +0.65, +0.5, +0.39f,
		+woodSize, +1.0f - 2.0f, -woodSize,  +0.65, +0.5, +0.39f,//
		+woodSize, -1.0f - 2.0f, -woodSize,   +0.65, +0.5, +0.39f,
		-woodSize, -1.0f - 2.0f, +woodSize,   +0.65, +0.5, +0.39f,
		+woodSize, -1.0f - 2.0f, +woodSize, +0.65, +0.5, +0.39f,
		+woodSize, +1.0f - 2.0f, +woodSize, +0.65, +0.5, +0.39f, //
		-woodSize, +1.0f - 2.0f, +woodSize,  +0.65, +0.5, +0.39f, //
	};
	GLushort tree3Indices[] = {
	  0,2,3,
	  0,1,3,
	  0,2,6,
	  0,7,6,
	  0,7,4,
	  0,1,4,
	  5,6,2,
	  5,3,2,
	  5,3,1,
	  5,1,4,
	  5,6,7,
	  5,4,7,

	8,10,11,
	8,9,11,
	8,10,14,
	8,15,14,
	8,15,12,
	8,9,12,
	13,14,10,
	13,11,10,
	13,11,9,
	13,9,12,
	13,14,15,
	13,12,15,
	};
	glGenVertexArrays(1, &tree3VaoID);
	glBindVertexArray(tree3VaoID);
	glGenBuffers(1, &tree3VboID);
	glBindBuffer(GL_ARRAY_BUFFER, tree3VboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tree3), tree3, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glGenBuffers(1, &tree3IndicesVboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tree3IndicesVboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tree3Indices) * sizeof(GLushort), tree3Indices, GL_STATIC_DRAW);

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
		-1.0f, +1.0f, -1.0f,  +0.8f, +0.8f, +0.8f,
		-1.0f, -1.0f, -1.0f,  +0.5f, +0.5f, +0.5f,
		+1.0f, +1.0f, -1.0f, +0.8f, +0.8f, +0.8f,
		+1.0f, -1.0f, -1.0f,  +0.5f, +0.5f, +0.5f,
		-1.0f, -1.0f, +1.0f,  +0.5f, +0.5f, +0.5f,
		+1.0f, -1.0f, +1.0f,  +0.5f, +0.5f, +0.5f,
		+1.0f, +1.0f, +1.0f, +0.8f, +0.8f, +0.8f,
		-1.0f, +1.0f, +1.0f,  +0.8f, +0.8f, +0.8f,

	};
	GLushort platformIndices[] = {
	  0,2,3,
	  0,1,3,
	  0,2,6,
	  0,7,6,
	  0,7,4,
	  0,1,4,
	  5,6,2,
	  5,3,2,
	  5,3,1,
	  5,1,4,
	  5,6,7,
	  5,4,7,
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


	const GLfloat night[] =
	{
		0.680f, -0.211f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.566f, 0.597f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.823f, -0.605f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.330f, 0.536f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.444f, 0.108f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.045f, 0.258f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.270f, 0.027f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.904f, 0.832f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.271f, 0.435f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.717f, 0.214f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.967f, -0.514f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.726f, 0.608f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.687f, -0.198f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.740f, -0.782f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.998f, -0.563f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.026f, 0.678f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.225f, -0.408f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.275f, 0.049f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.013f, 0.946f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.415f, 0.543f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.053f, 0.540f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.200f, 0.783f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.433f, -0.295f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.615f, 0.838f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.860f, 0.899f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.052f, -0.828f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.616f, 0.326f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.780f, -0.302f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.872f, -0.960f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.085f, -0.874f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.523f, 0.941f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.804f, 0.702f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.467f, 0.080f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.250f, 0.520f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.025f, 0.335f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.063f, -0.921f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.125f, 0.864f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.862f, 0.442f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.431f, 0.477f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.280f, -0.292f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.376f, -0.668f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.120f, 0.760f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.658f, -0.339f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.542f, 0.787f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.299f, 0.373f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.913f, 0.177f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.315f, 0.717f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.121f, 0.848f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.203f, 0.630f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.368f, 0.822f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.035f, -0.568f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.901f, 0.840f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.705f, 0.762f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.282f, -0.136f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.239f, -0.438f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.572f, -0.385f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.106f, -0.548f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.625f, -0.448f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.113f, -0.167f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.661f, 0.814f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.794f, -0.748f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.009f, 0.521f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.970f, 0.870f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.369f, -0.234f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.500f, -0.263f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.412f, -0.535f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.169f, -0.511f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.695f, 0.464f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.749f, 0.587f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.672f, 0.490f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.851f, 0.900f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.895f, 0.043f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.648f, -0.520f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.596f, 0.465f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.313f, 0.935f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.279f, 0.519f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.813f, -0.730f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.040f, -0.844f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.860f, -0.591f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.077f, 0.639f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.147f, 0.511f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.896f, -0.684f, 0.000f, 0.90f, 0.91f, 0.98f,
		1.000f, -0.591f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.780f, -0.749f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.996f, -0.892f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.741f, -0.855f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.992f, 0.846f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.188f, -0.639f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.674f, -0.217f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.826f, 0.639f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.282f, 0.105f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.159f, -0.095f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.375f, -0.801f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.062f, 0.515f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.391f, 0.984f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.154f, 0.755f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.496f, 0.258f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.929f, 0.496f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.666f, 0.851f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.747f, 0.662f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.959f, 0.488f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.807f, 0.967f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.334f, -0.005f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.672f, 0.660f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.778f, -0.846f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.299f, -0.504f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.259f, -0.542f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.401f, -0.366f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.342f, -0.537f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.852f, 0.266f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.553f, 0.302f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.021f, 0.943f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.440f, 0.092f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.439f, -0.773f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.057f, 0.185f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.889f, -0.098f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.327f, 0.695f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.131f, -0.994f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.310f, 0.197f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.666f, -0.532f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.351f, -0.034f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.036f, -0.390f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.424f, -0.635f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.244f, -0.918f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.172f, 0.392f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.348f, 0.275f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.306f, -0.631f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.218f, 0.254f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.461f, -0.343f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.481f, -0.596f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.842f, 0.370f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.306f, -0.485f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.065f, -0.825f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.479f, 0.755f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.372f, -0.813f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.777f, -0.277f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.153f, 0.186f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.333f, -0.422f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.552f, -0.423f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.341f, -0.620f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.969f, -0.993f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.655f, -0.337f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.624f, -0.127f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.917f, 0.838f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.530f, 0.398f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.758f, 0.372f, 0.000f, 0.90f, 0.91f, 0.98f,
		-0.232f, 0.549f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.886f, 0.833f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.724f, -0.593f, 0.000f, 0.90f, 0.91f, 0.98f,
		0.587f, 0.096f, 0.000f, 0.90f, 0.91f, 0.98f,

	};

	glGenVertexArrays(1, &nightVaoID);
	glBindVertexArray(nightVaoID);
	glGenBuffers(1, &nightVboID);
	glPointSize(4.0f);
	glBindBuffer(GL_ARRAY_BUFFER, nightVboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(night), night, GL_STATIC_DRAW);
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
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 1.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(4.0f, 1.0f, 4.0f));
	}
	else if (obj == "moon") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(x_delta*x_press_num, 5.0f, -5.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));
		//modelRotationMatrix =  glm::rotate(glm::mat4(), 7.0f, glm::vec3(0, 1, 0));
	}
	else if (obj == "tree0") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(2.3f, 2.0f, 2.5f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.4f+scale_delta*scale_press_num, 0.4f + scale_delta * scale_press_num, 0.4f + scale_delta * scale_press_num));
		//modelRotationMatrix = glm::rotate(glm::mat4(), clock()*0.0005f*isClockwise, glm::vec3(0, 1, 0));
		//modelRotationMatrix = glm::rotate(glm::mat4(), 10.0f, glm::vec3(0, 1, 0));
	}
	else if (obj == "tree1") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(1.6f, 2.0f, 3.1f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.2f + scale_delta * scale_press_num, 0.3f + scale_delta * scale_press_num, 0.2f + scale_delta * scale_press_num));
		//modelRotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0, 1, 0));
	}
	else if (obj == "tree2") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(1.8f, 2.0f, 3.5f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.2f + scale_delta * scale_press_num, 0.5f + scale_delta * scale_press_num, 0.2f + scale_delta * scale_press_num));
		// = glm::rotate(glm::mat4(), 3.0f, glm::vec3(0, 1, 0));
	}
	else if (obj == "tree3") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(-1.8f, 2.0f, 3.3f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.2f + scale_delta * scale_press_num, 0.3f + scale_delta * scale_press_num, 0.2f + scale_delta * scale_press_num));
		//modelRotationMatrix = glm::rotate(glm::mat4(), 7.0f, glm::vec3(0, 1, 0));
	}
	else if (obj == "tree4") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(-2.0f, 2.0f, 2.55f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.3f + scale_delta * scale_press_num, 0.4f + scale_delta * scale_press_num, 0.3f + scale_delta * scale_press_num));
		//modelRotationMatrix = glm::rotate(glm::mat4(), 1.0f, glm::vec3(0, 1, 0));
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
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 1.5f, 0.001 *st_time-2.5f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.5f, 1.0f, 1.0f));
		if (ready == 0) {
			if (st_time >= 3000) {
				now_t = clock();
				ready = 1;
				
			}
			else {
				st_time = (clock() - now_t);
			}
		}
	}
	else if (obj == "night") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -1.0f, -4.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(8.0f, 8.0f, 1.0f));
		//ck_time = (clock() - now_t2)*isClockwise;
		modelRotationMatrix = glm::rotate(glm::mat4(), clock()*0.0005f*isClockwise, glm::vec3(0, 0, 1));
	}
	else if (obj == "ah") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 4.0f, 0.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.001 *ck_time, 0.001 *ck_time, 1.0f));
		if (ck_time >= 2000) {
			now_t = clock();
			ah_animated = 0;
			ready = 0;
			ck_time = 0;
			st_time = 0;
		}
		else {
			ck_time = (clock() - now_t);
		}
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
	//printf("%d\t%d\t%d\t%d\n", st_time, ck_time, now_t, ready);
	//printf("%d\n", scale_press_num);
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

	matrix("moon");
	glBindVertexArray(moonVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 150);

	matrix("night");
	glBindVertexArray(nightVaoID);
	glDrawArrays(GL_POINTS, 0, 150);

	switch (season) {
		case 0:
			matrix("tree0");
			glBindVertexArray(tree1VaoID);
			glDrawElements(GL_TRIANGLES, 20 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree1");
			glBindVertexArray(tree1VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree2");
			glBindVertexArray(tree1VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree3");
			glBindVertexArray(tree1VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree4");
			glBindVertexArray(tree1VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			break;
		case 1:
			matrix("tree0");
			glBindVertexArray(treeVaoID);
			glDrawElements(GL_TRIANGLES, 20 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree1");
			glBindVertexArray(treeVaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree2");
			glBindVertexArray(treeVaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree3");
			glBindVertexArray(treeVaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree4");
			glBindVertexArray(treeVaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			break;
		case 2:
			matrix("tree0");
			glBindVertexArray(tree2VaoID);
			glDrawElements(GL_TRIANGLES, 20 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree1");
			glBindVertexArray(tree2VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree2");
			glBindVertexArray(tree2VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree3");
			glBindVertexArray(tree2VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree4");
			glBindVertexArray(tree2VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			break;
		case 3:
			matrix("tree0");
			glBindVertexArray(tree3VaoID);
			glDrawElements(GL_TRIANGLES, 20 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree1");
			glBindVertexArray(tree3VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree2");
			glBindVertexArray(tree3VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree3");
			glBindVertexArray(tree3VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			matrix("tree4");
			glBindVertexArray(tree3VaoID);
			glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
			break;
	}

	if (ah_animated == 1) {
		matrix("people");
		glBindVertexArray(peopleVaoID);
		glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);
		if (ready == 1){
			
			matrix("ah");
			glBindVertexArray(ahVaoID);
			glDrawArrays(GL_TRIANGLES, 0, 50);
		}	

	}

	matrix("platform");
	glBindVertexArray(platformVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	matrix("platform2");
	glBindVertexArray(platformVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	matrix("gate");
	glBindVertexArray(gateVaoID);
	glDrawElements(GL_TRIANGLES, 40 * sizeof(float), GL_UNSIGNED_SHORT, nullptr);

	if (scale_press_num > 10)
		scale_press_num = 10;
	if (scale_press_num < -10)
		scale_press_num = -10;


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