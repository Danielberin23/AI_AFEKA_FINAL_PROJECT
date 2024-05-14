
#include "Character.h"
#include "EatingState.h"
#include "ChaseState.h"

using namespace std;


Character::~Character() {}

Character::Character(bool IsPacman, Cell* Position)
{
	this->SetPosition(Position);
	this->SetIsPacman(IsPacman);
	//Pacman is eating food and Ghost is chasing pacman 
	if (IsPacman)
		pCurrentState = new EatingState();
	else
		pCurrentState = new ChaseState();
	pCurrentState->OnEnter(this);
	this->SetIsMoving(false);
}

void Character::SetDestination(double dx, double dy)
{
	double length;
	Target->SetRow(dx);
	Target->SetColumn(dy);
	dirx = Target->GetRow() - (*Position)->GetRow();
	diry = Target->GetColumn() - (*Position)->GetColumn();
	// normalize vector dir
	length = sqrt(dirx * dirx + diry * diry);
	dirx /= length;
	diry /= length;

}
//euclidian distance
double Character::Distance(Cell* n1, Cell* n2)
{
	int x1 = n1->GetRow();
	int x2 = n2->GetRow();
	int y1 = n1->GetColumn();
	int y2 = n2->GetColumn();
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


bool Character::isNearFood()
{

	return true;
}


bool Character::PlayPacman(Maze* gameInstance)
{
	Cell* pacmanTarget;
	Cell* previousTarget = nullptr;
	
	gameInstance->pacmanVector.push_back(this->getPosition());

	//if pacman won return true to game(no more coins to find)
	if(CoinsRisk(gameInstance))
		return true;
	pacmanTarget = gameInstance->safeDistancePQ.top();
	//if Pacman is moving target is food, change the target to the safest one
	if (isMoving)
	{
		if (previousTarget != nullptr)
		{
			if (pacmanTarget->GetSafeDistance() < previousTarget->GetSafeDistance())
			{
				this->SetTarget(previousTarget);
				MovePacman(gameInstance, this->getTarget());
			}
		}
		else
		{
			this->SetTarget(pacmanTarget);
			MovePacman(gameInstance, this->getTarget());
		}
		previousTarget = pacmanTarget;
	}
	else
	{
		if (isChasing)
		{
			//checks if the current position is safer than the neighboring cells
			int risk=assertSafety(gameInstance, pacmanTarget);

			previousTarget = gameInstance->MAZE[pacmanTarget->GetRow() + 1][pacmanTarget->GetColumn()];

			int newRisk = assertSafety(gameInstance, previousTarget);
			if (newRisk < risk)
				pacmanTarget = previousTarget;
			
			previousTarget = gameInstance->MAZE[pacmanTarget->GetRow() - 1][pacmanTarget->GetColumn()];

			newRisk = assertSafety(gameInstance, previousTarget);
			if (newRisk < risk)
				pacmanTarget = previousTarget;

			previousTarget = gameInstance->MAZE[pacmanTarget->GetRow()][pacmanTarget->GetColumn() + 1];

			newRisk = assertSafety(gameInstance, previousTarget);
			if (newRisk < risk)
				pacmanTarget = previousTarget;
			
			previousTarget = gameInstance->MAZE[pacmanTarget->GetRow()][pacmanTarget->GetColumn() - 1];

			newRisk = assertSafety(gameInstance, previousTarget);
			if (newRisk < risk)
				pacmanTarget = previousTarget;

			this->SetTarget(pacmanTarget);
			MovePacman(gameInstance, this->getTarget());
		}
		else if (isAttacking)
		{
			//staying in place need to check turn logic when pacman is attacked
		}
	}
		gameInstance->pacmanVector.clear();
		while(!gameInstance->safeDistancePQ.empty())
			gameInstance->safeDistancePQ.pop();
	
		//pacman didn't won yet
		return false;
}
void Character::PlayGhost(Maze* mazeInstance, int ghostNumber)
{
	int ghostValue;
	//PICK WHICH GHOST IS THE PLAYER
	if (ghostNumber == 0)
		ghostValue = GHOST_1;
	else if (ghostNumber == 1)
		ghostValue = GHOST_2;
	else
		ghostValue = GHOST_3;

	
	double distance = Distance(mazeInstance->pacman, mazeInstance->ghosts[ghostNumber]);


	// if ghost is near the pacman we first check number of ghosts
	if (distance >= 1 && distance < CLOSE_DISTANCE)
	{
		this->GetCurrentState()->Transition(this);//move to attack mode
	}
	else
	{
		/*ghostTarget->SetH(distance);
		mazeInstance->ghostsPQ.push(ghostTarget);
		MoveGhost(ghostNumber, ghostValue);
		while (!mazeInstance->ghostsPQ.empty())
			mazeInstance->ghostsPQ.pop();*/
	}

}

void Character::MovePacman(Maze* gameInstance, Cell* target)
{
	Cell* currentCell=nullptr;
	int row, column;
	bool go_on = true;
	int currentDepth = 0;
	while (!gameInstance->pacmanQueue.empty()&&currentDepth<=DEPTH)
	{
		currentCell = gameInstance->pacmanQueue.front();
		gameInstance->pacmanQueue.pop();
		row = currentCell->GetRow();
		column = currentCell->GetColumn();
		if (row == target->GetRow() && column == target->GetColumn())
			break;
		// up
			go_on = checkPacmanNeighbors(currentCell,gameInstance->MAZE[row + 1][column], gameInstance);
		// down
		if (go_on)
			go_on = checkPacmanNeighbors(currentCell, gameInstance->MAZE[row - 1][column], gameInstance);
		// left
		if (go_on)
			go_on = checkPacmanNeighbors(currentCell, gameInstance->MAZE[row][column - 1], gameInstance);
		// right
		if (go_on)
			go_on = checkPacmanNeighbors(currentCell, gameInstance->MAZE[row ][column+ 1], gameInstance);

		currentDepth++;
	}
	////restore path
	while (true)
	{
		currentCell = currentCell->GetParent();
		if (currentCell->GetParent()->GetIdentity() == PACMAN)
			break;
	}




}


void Character::MoveGhost(int ghostNumber, int ghostValue)
{

}

bool Character::checkPacmanNeighbors(Cell* previousCell, Cell* cell, Maze* maze)
{
	if (maze->MAZE[cell->GetRow()][cell->GetColumn()]->GetIdentity() == SPACE)
	{
		cell->SetParent(previousCell);
		maze->pacmanQueue.push(cell);
		return true;
	}
	else
		return false;
}


double Character::assertSafety(Maze * gameInstance, Cell* coinCell)
{
	int i;
	double risk = 0;
	// Sum the distances between the coins and the ghosts
	for (i = 0; i < NUM_OF_GHOSTS; i++)
	{
		risk += Distance(coinCell, gameInstance->ghosts[i]);
	}
	// If the Distance between the coin and Pacman is smaller then DISTANCE_THRESHOLD
	// The DISTANCE_THRESHOLD can be any value bigger than 1, which means there is some risk.
	// Else, reduce the distance between pacman and the coin, multiplied by the number 
	// of Ghosts (3) for safety
	if (Distance(coinCell, gameInstance->pacman) < CLOSE_DISTANCE)
		risk = -DBL_MAX;
	else
		risk -= 3 * Distance(coinCell, gameInstance->pacman);
	return risk;
}

bool Character::CoinsRisk(Maze* gameInstance)
{
	int i, j;
	double safeDistance;
	bool foundCoin = false;
	for (i = 0; i < MSZ; i++)
	{
		for (j = 0; j < MSZ; j++)
		{
			if (gameInstance->MAZE[i][j]->IsFood())
			{
				foundCoin = true;
				// Calculate the safe distance for the coin Cell
				safeDistance = assertSafety(gameInstance,gameInstance->MAZE[i][j]);
				// Set the coin Cell's Safe Distance
				gameInstance->MAZE[i][j]->SetSafeDistance(safeDistance);
				gameInstance->safeDistancePQ.push(gameInstance->MAZE[i][j]);
				
			}
		}
	}

	// If no coins are found, then Pacman ate them all 
	if (!foundCoin)
	{
		return true;
		//cout << "PACMAN WON THE GAME! Yey! Numbers of Steps: " << this->GetSteps() << endl;
	}
	return false;
}

