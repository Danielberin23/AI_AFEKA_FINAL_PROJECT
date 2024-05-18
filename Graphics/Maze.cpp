#include "Maze.h"
#include "Character.h"




Maze::Maze() {
	InitMaze();

	while (IsFairGame())
		fixMaze();
}
Maze::~Maze(){};

// -------------------------------------------------------------------


void Maze::InitMaze()
{
	int i, j;
	srand(static_cast<unsigned int>(time(0)));  // Explicit cast to unsigned int
	//srand(time(0));
	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			MAZE[i][j] = new Cell(i,j);
			/*MAZE[i][j]->SetRow(i);
			MAZE[i][j]->SetColumn(j);*/

		}
	}
	for (i = 0; i < MSZ; i++)
	{
		MAZE[0][i]->SetIdentity(WALL);//frame Wall
		MAZE[MSZ - 1][i]->SetIdentity(WALL);
		MAZE[i][0]->SetIdentity(WALL);
		MAZE[i][MSZ - 1]->SetIdentity(WALL);
	}

	for (i = 2; i < MSZ - 2; i++)
	{
		for (j = 2; j < MSZ - 2; j++)
		{
			if (i % 2 == 1) // Odd row
			{
				if (rand() % 10 > 5)
					MAZE[i][j]->SetIdentity(WALL);
			}
			else // Even row
			{
				if (rand() % 10 > 8)
					MAZE[i][j]->SetIdentity(WALL);
			}
		}
	}

	// mark out the pacman start cell
	do
	{
		i = rand() % MSZ;
		j = rand() % MSZ;
	} while (MAZE[i][j]->GetIdentity() != SPACE);
	MAZE[i][j]->SetIdentity(PACMAN);
	pacman =new Cell(MAZE[i][j]);
	pacmanQueue.push(pacman);//cell* pacman
}

void Maze::ChangeColorMaze()
{
	int i;
	int j;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (MAZE[i][j]->GetIdentity())
			{
			case BLACK:
				MAZE[i][j]->SetIdentity(SPACE);
				break;
			//case SPACE:
			//	MAZE[i][j]->SetIdentity(WALL);
			//	break;
			}
		}
}
// A function to add the Pacman food to the maze
void Maze::AddFood()
{
	int i, j, k;
	for (k = 0; k < NUM_OF_FOOD; k++)
	{
		do
		{
			i = rand() % MSZ;
			j = rand() % MSZ;
		} while (MAZE[i][j]->GetIdentity() != SPACE);
		MAZE[i][j]->SetIdentity(FOOD);
	}
}
void Maze::AddGhosts()
{
	int i, j, k;
	int identity;
	for (k = 0; k < NUM_OF_GHOSTS; k++)
	{
		do
		{
			i = rand() % MSZ;
			j = rand() % MSZ;
			identity = MAZE[i][j]->GetIdentity();
		} while (identity != SPACE);
		if (k == 0)
			MAZE[i][j]->SetIdentity(GHOST_1);
		else if (k == 1)
			MAZE[i][j]->SetIdentity(GHOST_2);
		else if (k == 2)
			MAZE[i][j]->SetIdentity(GHOST_3);
		ghosts[k]=new Cell(MAZE[i][j]);
	}
}
int Maze::numOfSpaces()
{
	int i, j;
	int counter = 0;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
			if (MAZE[i][j]->GetIdentity() == SPACE)
				counter++;
	return counter;
}
// This functions cleans the Maze and sets all the Cells to SPACE
void Maze::cleanMaze()
{
	int i, j;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
			MAZE[i][j]->SetIdentity(SPACE);
}

void Maze::fixMaze()
{
	Cell* current;

	if (pacmanVector.empty())
	{
		if (numOfSpaces() < NUM_OF_FOOD + NUM_OF_GHOSTS + 40)
		{
			cout << "An incorrect maze was created. Drawing another maze." << numOfSpaces() << endl;
			cleanMaze();
			InitMaze();
		}
		fairGame = false;
		ChangeColorMaze();
		AddGhosts();
		AddFood();
	}
	else
	{
		current = pacmanVector[0];
		pacmanVector.erase(pacmanVector.begin());

		if (MAZE[current->GetRow()][current->GetColumn()]->GetIdentity() != PACMAN)
			MAZE[current->GetRow()][current->GetColumn()]->SetIdentity(BLACK);

		if (MAZE[current->GetRow() - 1][current->GetColumn()]->GetIdentity() == SPACE)
		{
			MAZE[current->GetRow() - 1][current->GetColumn()]->SetIdentity(GRAY);
			pacmanVector.push_back(MAZE[current->GetRow() - 1][current->GetColumn()]);
		}
		if (MAZE[current->GetRow() + 1][current->GetColumn()]->GetIdentity() == SPACE)
		{
			MAZE[current->GetRow() + 1][current->GetColumn()]->SetIdentity(GRAY);
			pacmanVector.push_back(MAZE[current->GetRow() + 1][current->GetColumn()]);
		}

		if (MAZE[current->GetRow()][current->GetColumn() - 1]->GetIdentity() == SPACE)
		{
			MAZE[current->GetRow()][current->GetColumn() - 1]->SetIdentity(GRAY);
			pacmanVector.push_back(MAZE[current->GetRow()][current->GetColumn() - 1]);
		}

		if (MAZE[current->GetRow()][current->GetColumn() + 1]->GetIdentity() == SPACE)
		{
			MAZE[current->GetRow()][current->GetColumn() + 1]->SetIdentity(GRAY);
			pacmanVector.push_back(MAZE[current->GetRow()][current->GetColumn() + 1]);
		}
	}
}
void Maze::DrawMaze()
{

	int i, j;
	int xsz = WIDTH / MSZ;
	int ysz = HEIGHT / MSZ;
	double x, y;
	for (i = 0; i < MSZ; i++)
		for (j = 0; j < MSZ; j++)
		{
			switch (MAZE[i][j]->GetIdentity())   // set color
			{
			case SPACE:
					glColor3d(0, 0, 0); // Black
				break;
			case WALL:
				glColor3d(0.5, 0.5, 0.5); // Grey
				break;
			case PACMAN:
				glColor3f(1.0, 1.0, 0.0); // Yellow
				break;
			case TARGET:
				glColor3d(1, 0, 0); // Red
				break;
			case GHOST_1:
				glColor3d(0.5, 0.2, 1); // Purple
				break;
			case GHOST_2:
				glColor3d(0.5, 0.2, 1); // Purple
				break;
			case GHOST_3:
				glColor3d(0.5, 0.2, 1); // Purple
				break;
			case FOOD:
				glColor3d(1, 1, 1); // White
			}
			// Draw square -> maze[i][j]
			x = 2.0 * (j * xsz) / WIDTH - 1; // value in range [-1,1)
			y = 2.0 * (i * ysz) / HEIGHT - 1; // value in range [-1,1)
			// Fill the sqaure 
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x, y + (2.0 * ysz) / HEIGHT);
			glVertex2d(x + 2.0 * xsz / WIDTH, y + (2.0 * ysz) / HEIGHT);
			glVertex2d(x + 2.0 * xsz / WIDTH, y);
			glEnd();
			// Draw the square borders (cleaner look)
			glColor3d(0, 0, 0);
			glBegin(GL_LINE_LOOP);
			glVertex2d(x, y);
			glVertex2d(x, y + (2.0 * ysz) / HEIGHT);
			glVertex2d(x + 2.0 * xsz / WIDTH, y + (2.0 * ysz) / HEIGHT);
			glVertex2d(x + 2.0 * xsz / WIDTH, y);
			glEnd();
		}
}

bool Maze::IsGhost(int ghostValue)
{
	if (ghostValue == GHOST_1 || ghostValue == GHOST_2 || ghostValue == GHOST_3)
		return true;
	return false;
}