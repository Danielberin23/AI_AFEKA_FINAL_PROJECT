#pragma once

#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <queue>
#include <vector>

#include "glut.h"
#include "Cell.h"
#include "CellComparator.h"
#include "SafeDistanceComparator.h"
using namespace std;

const int MSZ = 50;
const int PacmanFightDelay = 2;

class Maze {
public:
    priority_queue<Cell*, vector<Cell*>, CellComparator> ghostsPQ;
    priority_queue<Cell*, vector<Cell*>, SafeDistanceComparator> safeDistancePQ;

    vector<Cell*> pacmanVector;
    vector<Cell*> blacksVector;
    vector<Cell*> graysVector;
    queue <Cell*> pacmanQueue; // queue of gray cells

    vector<Cell*>::iterator graysIterator;
    vector<Cell*>::iterator blacksIterator;
    vector<Cell*>::iterator pacmanIterator;

    Cell* pacman;
    Cell* ghosts[3];  // initialize ghost arr
    Cell* MAZE[MSZ][MSZ] = { 0 }; // Initialize the maze with SPACES 
    
    //check if food can be reached
    bool fairGame=true;


    Maze();
    ~Maze();

    void	InitMaze();
    void	ChangeColorMaze();
    void	AddFood();
    void	AddGhosts();
    int		numOfSpaces();
    void	cleanMaze();
    void	fixMaze();
    void	DrawMaze();
    bool	IsGhost(int ghostValue);
   
    //void	turn();

    bool			IsFairGame() { return fairGame; }
    void SetFairGame(bool flag) { this->fairGame = flag; }
  
};