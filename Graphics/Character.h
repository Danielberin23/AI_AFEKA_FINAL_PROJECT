#pragma once

#include "Cell.h"
#include "State.h"
#include "Maze.h"


using namespace		std;


const int			WIDTH = 800;
const int			HEIGHT = 600;
const int			NUM_OF_GHOSTS = 3;
const int			NUM_OF_FOOD = 4;
const double		STEP_PENALTY = 0.4;
const double		CLOSE_DISTANCE = 1.4;
const int			DEPTH = 5;// BFS DEPTH
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
	

	// bool states
	bool isMoving;
	bool isChasing;
	bool isAttacking;

	int pacmanScore = 0;
	State* pCurrentState;
	
public:
	
	//bool fairGame = false;
	// Constructor
	Character(bool IsPacman);
	//Destructor
	~Character();

	//Getters
	int		GetPacmanPoints() { return pacmanPoints; }
	int		IsSafeDistance() { return safeDistance; }
	Cell*   getPosition() { return Position; }
	Cell*   getTarget() { return Target; }
	bool	GetIsPacman() { return IsPacman; }
	int		GetPacmanScore() { return pacmanScore; }

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
	bool	isNearFood();//not used
	int		StartMove(Cell* pCurrPlayer, Character* pCh, int distance);
	
	
	bool	PlayPacman(Maze* gameInstance);
	void	PlayGhost(Maze* gameInstance, Cell* ghostTarget, Cell* pacman, int ghostNumber);

	void	MovePacman(Maze* gameInstance,Cell* target);
	void	MoveGhost(int ghostNumber, int ghostValue);
	std::vector<Cell*>	getPacmanNeighbors(Cell* cell, const Maze &maze);

	double	assertSafety(Maze* gameInstance, Cell* coinCell);
	bool	CoinsRisk(Maze* gameInstance);

};