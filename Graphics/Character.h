#pragma once

#include "State.h"
#include "Maze.h"


using namespace		std;



const double		STEP_PENALTY = 0.4;
const double		CLOSE_DISTANCE = 1.4;
const int			DEPTH = 20;// BFS DEPTH
class State;

class Character
{
private:
	Cell* Position;
	
	Cell* Target;
	double dirx, diry;
	int pacmanPoints = 0;
	bool safeDistance = true;
	bool IsPacman;
	int Moves = 0;

	// bool states
	bool isMoving;
	bool isChasing;
	bool isAttacking;

	int pacmanScore = 0;
	State* pCurrentState;
	
public:
	
	//bool fairGame = false;
	// Constructor
	Character(bool IsPacman,Cell* Position);
	//Destructor
	~Character();

	//Getters
	 int	GetPacmanPoints() { return pacmanPoints; }
	 int	IsSafeDistance() { return safeDistance; }
	Cell*   getPosition() { return Position; }
	Cell*   getTarget() { return Target; }
	bool	GetIsPacman() { return IsPacman; }
	int		GetPacmanScore() { return pacmanScore; }
	int GetMoves() { return Moves; }
	// STATE Getters
	
	bool	GetIsAttacking() { return isAttacking; }
	bool	GetIsChasing() { return isChasing; }
	State*	GetCurrentState() { return pCurrentState; }

	// Setters
	void SetPacmanPoints(int points) { this->pacmanPoints = points; }
	
	void SetIsMoving(bool value) { isMoving = value; }
	void SetDestination(double dx, double dy);
	void SetIsPacman(bool isPacman) { this->IsPacman = isPacman; };
	void SetPosition(Cell* Position) { this->Position = Position; }
	void SetTarget(Cell* Target) { this->Target = Target; }

	// STATE - setters
	
	void SetIsAttacking(bool attacking) { this->isAttacking = attacking; }
	void SetIsChasing(bool chasing) { this->isChasing = chasing; }
	
	
	void	SetCurrentState(State* ps) { pCurrentState = ps; }
	double	Distance(Cell* n1, Cell* n2);
	
	
	bool	PlayPacman(Maze& gameInstance);
	bool	PlayGhost(Maze& gameInstance,int ghostNumber);

	bool	MovePacman(Maze& gameInstance,Cell* target);
	void	MoveGhost(Maze& gameInstance, int ghostNumber, int ghostValue);
	bool	checkPacmanNeighbors(Cell* previousCell, Cell* cell, Maze& maze);

	bool	checkGhostNeighbors(int rowOffset, int columnOffset,
		Cell* pCurrentCell, int ghostNumber, int ghostValue, Maze& gameInstance, int cellIdentity);

	double	assertSafety(Maze& gameInstance, Cell* coinCell);
	bool	CoinsRisk(Maze& gameInstance); 

	void getNumOfGhosts(int& num, Maze& mazeInstance);

	int ghostsAttacking(Maze& mazeInstance);

	Cell* getAttackingGhost(Maze& mazeInstance);
};