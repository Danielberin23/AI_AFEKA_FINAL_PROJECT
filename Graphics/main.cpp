
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "glut.h"

#include "Game.h"

using namespace std;

Game* game;

void init()
{
	srand(time(NULL));

	game = new Game();
	
	//srand(static_cast<unsigned int>(time(NULL)));  // Explicit cast to unsigned int
	glClearColor(0, 0.5, 0.8, 0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // set the coordinates system
	
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	game->DrawMaze();
	glutSwapBuffers(); // show all

}

void idle() 
{
	glutPostRedisplay();
}
void menu(int choice)
{

	switch (choice)
	{
	case 0:
		if (!game->IsGhostWon() && !game->IsPacmanWon())
		{
			game->PlayGame();
			display();
		}
		break;

	case 1:
		while (!game->IsGhostWon() && !game->IsPacmanWon())
		{
			game->PlayGame();
			display();
		}
		break;

	case 2:
		srand(static_cast<unsigned int>(time(0)));
		game=new Game();
		break;

	}
	
}

void main(int argc, char* argv[]) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Pacman Game");


	//menu
	glutCreateMenu(menu);
	glutAddMenuEntry("One Step", 0);
	glutAddMenuEntry("Run", 1);
	glutAddMenuEntry("Restart Game", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	
	init();

	glutMainLoop();
}