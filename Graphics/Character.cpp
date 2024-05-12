
#include "Character.h"
#include "EatingState.h"
#include "ChaseState.h"

using namespace std;


Character::~Character() {}

Character::Character(bool IsPacman)
{
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
	dirx = Target->GetRow() - Position->GetRow();
	diry = Target->GetColumn() - Position->GetColumn();
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

	//if pacman won return true to game
	if(CoinsRisk(gameInstance))
		return true;
	pacmanTarget = gameInstance->safeDistancePQ.top();
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
			//needs to move away from ghost
		}
		else if (isAttacking)
		{
			this->SetTarget(this->getPosition());
		}
	}
		gameInstance->pacmanVector.clear();
		while(!gameInstance->safeDistancePQ.empty())
			gameInstance->safeDistancePQ.pop();
	

		return false;
}
void Character::PlayGhost(Maze* mazeInstance, Cell* ghostTarget,Cell* pacman, int ghostNumber)
{
	int ghostValue;

	if (ghostNumber == 0)
		ghostValue = GHOST_1;
	else if (ghostNumber == 1)
		ghostValue = GHOST_2;
	else
		ghostValue = GHOST_3;

	
	double distance = Distance(ghostTarget, pacman);


	// if ghost is near the pacman we first check number of ghosts
	// if there are 3 ghost we have to create transition to attacking state
	// if there are 2 or less ghosts, ghosts won
	if (distance >= 1 && distance < CLOSE_DISTANCE)
	{
		this->GetCurrentState()->Transition(this);//move to attack mode
	}
	else
	{
		ghostTarget->SetH(distance);
		mazeInstance->ghostsPQ.push(ghostTarget);
		MoveGhost(ghostNumber, ghostValue);
		while (!mazeInstance->ghostsPQ.empty())
			mazeInstance->ghostsPQ.pop();
	}

}

void Character::MovePacman(Maze* gameInstance, Cell* target)
{
	//pacman find food with BFS
	queue<Cell*> queue;
	Cell* startCell = nullptr;
	int row, column;
	std::vector<std::vector<bool>> visited(MSZ, std::vector<bool>(MSZ, false));

	startCell = this->getPosition();
	queue.push(startCell);
	visited[startCell->GetRow()][startCell->GetColumn()] = true;
	int currentDepth = 0;
	

	while (!queue.empty() && currentDepth <= DEPTH)
	{
		int levelSize = queue.size();

		for (int i = 0; i < levelSize; i++)
		{
			Cell* currentCell = queue.front();
			queue.pop();
			if (gameInstance->IsGhost(currentCell->GetIdentity()))
			{

				//in case we find ghost
				//transition to chase state
				this->GetCurrentState()->Transition(this);
				queue.push(currentCell);
				break;

			}
			else
			{
				//in case we find coin
				if (currentCell->GetColumn() == target->GetColumn() && currentCell->GetRow() == target->GetRow())
				{	

					break;
				}
			}
			// Check the cells: up, down, left, right

			std::vector<Cell*> neighbors = getPacmanNeighbors(currentCell, *gameInstance);
			for (Cell* neighbor : neighbors) {
				int x = neighbor->GetRow();
				int y = neighbor->GetColumn();
				if (!visited[x][y]) {
					queue.push(neighbor);
					visited[x][y] = true;
				}

			}
			currentDepth++;
		}
	}
	if (!queue.empty() && isMoving) {
		Cell* lastCell = queue.back();
		this->SetPosition(lastCell);
		gameInstance->MAZE[lastCell->GetRow()][lastCell->GetColumn()]->SetIdentity(PACMAN);
		//paint previous cell space
		gameInstance->MAZE[startCell->GetRow()][startCell->GetColumn()]->SetIdentity(SPACE);
	}
	if (isChasing)
	{
		double newRisk = 0;
		int xSafe, ySafe;
		Cell* ghostCell = queue.front();
		std::vector<Cell*> neighbors = getPacmanNeighbors(this->getPosition(), *gameInstance);
		double risk = this->assertSafety(gameInstance,this->getPosition());
		for (Cell* neighbor : neighbors) {
			int x = neighbor->GetRow();
			int y = neighbor->GetColumn();
			newRisk = this->assertSafety(gameInstance,gameInstance->MAZE[x][y]);
			if (newRisk < risk) {
				xSafe = x;
				ySafe = y;
			}
		}
		this->SetPosition(gameInstance->MAZE[xSafe][ySafe]);
		gameInstance->MAZE[xSafe][ySafe]->SetIdentity(PACMAN);
		//paint previous cell space
		gameInstance->MAZE[this->getPosition()->GetRow()][this->getPosition()->GetColumn()]->SetIdentity(SPACE);

	}
	
}


void Character::MoveGhost(int ghostNumber, int ghostValue)
{

}

std::vector<Cell*> Character::getPacmanNeighbors(Cell* cell, const Maze& maze)
{
	std::vector<Cell*> neighbors;
	int x = cell->GetRow();
	int y = cell->GetColumn();

	// Check left neighbor
	if (x > 0 && !(maze.MAZE[y][x - 1]->GetIdentity() == WALL)) {
		neighbors.push_back(maze.MAZE[y][x - 1]);
	}

	// Check right neighbor
	if (x < MSZ - 1 && !(maze.MAZE[y][x + 1]->GetIdentity() == WALL)) {
		neighbors.push_back(maze.MAZE[y][x + 1]);
	}

	// Check top neighbor
	if (y > 0 && !(maze.MAZE[y - 1][x]->GetIdentity() == WALL)) {
		neighbors.push_back(maze.MAZE[y - 1][x]);
	}

	// Check bottom neighbor
	if (y < MSZ - 1 && !(maze.MAZE[y + 1][x]->GetIdentity() == WALL)) {
		neighbors.push_back(maze.MAZE[y + 1][x]);
	}

	return neighbors;
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

