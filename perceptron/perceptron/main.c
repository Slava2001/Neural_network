#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#pragma comment( lib, "glew32.lib" )
#pragma comment (lib, "OpenGL32.lib")

#include "neuron.h"

#define WIDTH  400
#define HEIGHT  400
#define DOT_SIZE 0.02

int countOfTD = 0;
double truedot[100][2];
int countOfFD = 0;
double falseedot[100][2];

char FlagDrawMode=0;
char FlagMouseButt = 0;

double CursorX = 0;
double CursorY = 0;

void draw();
void learn();
int main(int argc, char* argv[])
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "perceptron", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewInit();
	glEnable(GL_DEPTH_TEST);


	srand(time(NULL));
	initNeuron(3, 2,2, 1);

	char FlagKeyPrs = 0;

	while (!glfwWindowShouldClose(window))
	{

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw();

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!FlagKeyPrs)
			{
				delNeuron();
				initNeuron(3, 2, 3, 2);
			}
			FlagKeyPrs = 1;
		}else
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		{
			if (!FlagKeyPrs)
			{
				if(FlagDrawMode)
				FlagDrawMode=0;
				else
				FlagDrawMode = 1;
			}
			FlagKeyPrs = 1;
		}
		else
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			if (!FlagKeyPrs)
			{
				countOfFD = countOfTD = 0;
			}
			FlagKeyPrs = 1;
		}
		else FlagKeyPrs = 0;
		


		
		glfwGetCursorPos(window, &CursorX, &CursorY);

		CursorX=(CursorX / (float)WIDTH) * 2 - 1;
		CursorY= -((CursorY / (float)HEIGHT) * 2 - 1);


		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			if (!FlagMouseButt)
			{
				truedot[countOfTD][0] = CursorX;
				truedot[countOfTD][1] = CursorY;
				countOfTD++;
			}
			FlagMouseButt = 1;
		}
		else
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
		{
			if (!FlagMouseButt)
			{
				falseedot[countOfFD][0] = CursorX;
				falseedot[countOfFD][1] = CursorY;
				countOfFD++;
			}
			FlagMouseButt = 1;
		}
		else
		FlagMouseButt = 0;



		






		
		learn();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

void draw()
{
	glPushMatrix();
	
	for (int i = 0; i < countOfTD; i++)
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0, 0, 1);
		glVertex2d(truedot[i][0] - DOT_SIZE, truedot[i][1] + DOT_SIZE);
		glVertex2d(truedot[i][0] + DOT_SIZE, truedot[i][1] - DOT_SIZE);
		glVertex2d(truedot[i][0] + DOT_SIZE, truedot[i][1] + DOT_SIZE);
		glEnd();


	}
	for (int i = 0; i < countOfFD; i++)
	{
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0, 0, 1);
		glVertex2d(falseedot[i][0] - DOT_SIZE, falseedot[i][1] + DOT_SIZE);
		glVertex2d(falseedot[i][0] - DOT_SIZE, falseedot[i][1] - DOT_SIZE);
		glVertex2d(falseedot[i][0] + DOT_SIZE, falseedot[i][1] + DOT_SIZE);
		glEnd();

	}
	
	glPopMatrix();


	//не самый быстрый спрособ,но простой
	glPushMatrix();
	glBegin(GL_POINTS);
	for (float y = -1; y < 1; y += 2. / HEIGHT)
	{
		for (float x = -1; x < 1; x+=2./WIDTH)
		{
			if(FlagDrawMode)
			glColor3f(0, (int)(queryNeuron(x, y)[0] + 0.2),0);
			else
			glColor3f(0, queryNeuron(x, y)[0], 0);
			
			
			glVertex2f(x, y);
		}
	}
	glEnd();
	glPopMatrix();




}
void learn()
{
	int errCountT = 0;
	int errCountF = 0;

	for (int i = 0; i < countOfTD; i++) {
		double* a = queryNeuron(truedot[i][0], truedot[i][1]);
		if (a[0] < 0.8)
		{
			learningNeuron(1.f);
			errCountT++;
		}
	}
	for (int i = 0; i < countOfFD; i++) {
		double* a = queryNeuron(falseedot[i][0], falseedot[i][0]);
		if (a[0]>0.5)
		{
			learningNeuron(-1.f);
			errCountF++;
		}
	}

	printf("\rErr: %2d %2d", errCountT, errCountF);

}