
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

bool Character::PlayPacman(Maze* gameInstance)
{
	Cell* pacmanTarget;
	Cell* previousTarget = nullptr;
	
	gameInstance->pacmanVector.push_back(this->getPosition());

	//if pacman won return true to game(no more coins to find)
	if(CoinsRisk(gameInstance))
		return true;
	pacmanTarget = gameInstance->safeDistancePQ.top();
	//if Pacman is moving, target is food, change the target to the safest one
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
bool Character::PlayGhost(Maze* mazeInstance, int ghostNumber)
{
	int ghostValue;
	int numOfGhosts = 0;

	//PICK WHICH GHOST IS THE PLAYER
	if (ghostNumber == 0)
		ghostValue = GHOST_1;
	else if (ghostNumber == 1)
		ghostValue = GHOST_2;
	else
		ghostValue = GHOST_3;

	
	double distance = Distance(mazeInstance->pacman, mazeInstance->ghosts[ghostNumber]);

	if (isChasing)
	{
		// check distance between ghost and pacman
		if ( distance >= 1 && distance < CLOSE_DISTANCE)
		{
			//move to attack mode 
			this->GetCurrentState()->Transition(this); 
			MoveGhost(mazeInstance, ghostNumber, ghostValue);
		}
		
		// else change position of the ghost 
		else
		{
			mazeInstance->ghosts[ghostNumber]->SetH(distance);
			mazeInstance->ghostsPQ.push(mazeInstance->ghosts[ghostNumber]);
			MoveGhost(mazeInstance, ghostNumber, ghostValue);
			while (!mazeInstance->ghostsPQ.empty())
				mazeInstance->ghostsPQ.pop();
		}
	}

	// in Attack state
	else
	{
		if (isAttacking)
		{
			// if ghost is near the pacman we first check number of ghosts
			getNumOfGhosts(&numOfGhosts, mazeInstance);

			// check if all ghosts are in range of 1 of the ghost which attacking
			// Three check required
			int attackers = ghostsAttacking(mazeInstance);
			if (attackers > 1)
			{
				// if there are at least 1 more ghost in range, than pacman looses
				return true;
			}

			else
			{
				return false;
			}
		}
	}
}

int Character::ghostsAttacking(Maze* mazeInstance)
{
	int numAttackers = 0;
	for (int i = 0; i < NUM_OF_GHOSTS; i++)
	{
		// the distance between ghosts themselves, and distance between each ghost and pacman
		if (Distance(mazeInstance->ghosts[i], mazeInstance->pacman) <= CLOSE_DISTANCE)
			numAttackers += Distance(this->getPosition(), mazeInstance->ghosts[i]);
	}

	if (numAttackers > 1)
		return numAttackers;

	return 0;
}


void Character::getNumOfGhosts(int* num, Maze* mazeInstance)
{
	int temp = 0;
	for (int i = 0; i < MSZ; i++)
	{
		for (int j = 0; j < MSZ; j++)
		{
			if (mazeInstance->MAZE[i][j]->GetIdentity() != WALL && mazeInstance->MAZE[i][j]->GetIdentity() != PACMAN
				&& mazeInstance->MAZE[i][j]->GetIdentity() != FOOD)
				temp++;
		}
	}

	*num = temp;
}

bool Character::MovePacman(Maze* gameInstance, Cell* target)
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
	currentCell->SetIdentity(PACMAN);
	
	this->SetPosition(currentCell);
	currentCell = currentCell->GetParent();
	// "Paint" the Pacman's previous cell with SPACE
	gameInstance->MAZE[currentCell->GetRow()][currentCell->GetColumn()]->SetIdentity(SPACE);

	if (Distance(this->getPosition(), target))
	{
		this->SetPacmanPoints(this->GetPacmanPoints() + 10);
		cout << "Pacman found food and got " << this->GetPacmanPoints() * 100 << " points" << endl;
		if (pacmanPoints == NUM_OF_FOOD)
			return true;
		else
			return false;
	}





}


void Character::MoveGhost(Maze* gameInstance, int ghostNumber, int ghostValue)
{
	Cell* temp = nullptr;
	int row, column;
	
	// A*
	while (!gameInstance->ghostsPQ.empty())
	{
		// getting the priority position and popping it out of the queue
		temp = gameInstance->ghostsPQ.top();
		gameInstance->ghostsPQ.pop();

		row = temp->GetRow();
		column = temp->GetColumn();

		// if pacman position is found...
		if (row == gameInstance->pacman->GetRow() && column == gameInstance->pacman->GetColumn())
			break;

		// Return an iterator to the first element in the range given that compares equal
		// to the temp Cell. If no such element is found, the function returns the last element.
		gameInstance->graysIterator = find(gameInstance->graysVector.begin()
			, gameInstance->graysVector.end(), temp);

		// If the element returned is not the last element, erase it from the vector
		if (gameInstance->graysIterator != gameInstance->graysVector.end())
			gameInstance->graysVector.erase(gameInstance->graysIterator);

		// Push the temp Cell to the blacks Vector
		gameInstance->blacksVector.push_back(temp);

		// up
		checkGhostNeighbors(0, -1, temp, ghostNumber, ghostValue, gameInstance, PACMAN);
		// down
		checkGhostNeighbors(0,  1, temp, ghostNumber, ghostValue, gameInstance, PACMAN);
		//left
		checkGhostNeighbors(-1, 0, temp, ghostNumber, ghostValue, gameInstance, PACMAN);
		// right
		checkGhostNeighbors(1,  0, temp, ghostNumber, ghostValue, gameInstance, PACMAN);
	}

	// Restore the best path found and get the next cell to move to
	while (true)
	{
		temp = temp->GetParent();
		if (temp->GetParent()->GetIdentity() == ghostValue)
			break;

		// temp pointing to the next cell position of ghost. 
		// which was calculated
	}
	// setting identity of new cell to be the ghost[number]
	temp->SetIdentity(ghostValue);
	// inserting new ghost position
	gameInstance->ghosts[ghostNumber] = temp;
	// getting old ghost's position
	temp = temp->GetParent();
	// put SPACE in that old position
	gameInstance->MAZE[temp->GetRow()][temp->GetColumn()]->SetIdentity(SPACE);

	//clear vectors
	gameInstance->blacksVector.clear();
	gameInstance->graysVector.clear();
}

// A function to check Ghost's neighbor cells
bool Character::checkGhostNeighbors(int rowOffset, int columnOffset, 
	Cell* pCurr, int ghostNumber, int ghostValue, Maze* gameInstance, int cellIdentity)
{
	Cell* temp;
	// coping pCurrent to temp
	temp = gameInstance->MAZE[pCurr->GetRow() + rowOffset][pCurr->GetColumn() + columnOffset];
	
	// Find that Cell in the blacks Vector
	gameInstance->blacksIterator = find(gameInstance->blacksVector.begin(),
		gameInstance->blacksVector.end(), temp);

	//Find that Cell in the grays Vector
	gameInstance->graysIterator = find(gameInstance->graysVector.begin(),
		gameInstance->graysVector.end(), temp);

	// Check if the Cell is not a WALL, and if it was the last element on both blacks and grays Vector
	// Also, verify the Cell is not a Ghost
	if (temp->GetIdentity() != WALL && !gameInstance->IsGhost(temp->GetIdentity())
		&& gameInstance->graysIterator == gameInstance->graysVector.end() 
		&& gameInstance->blacksIterator == gameInstance->blacksVector.end())
	{
		// set neighbor Cell's parent
		temp->SetParent(pCurr);
		// Push the Neighboring Cell to the grays Vector
		gameInstance->graysVector.push_back(temp);
		// Set the Neighboring Cell G to the Ghost's G + some step penalty
		temp->SetG(temp->GetG() + STEP_PENALTY);

	}
	
	return true;
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

