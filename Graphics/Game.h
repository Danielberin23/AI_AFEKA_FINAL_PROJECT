#pragma once

#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <queue>
#include <vector>

#include "glut.h"
#include "Character.h"






class Game {
private:
		//define characters
		Character* Pacman;
		Character* Ghosts[NUM_OF_GHOSTS];
		
public:
	//Queues and Vectors
	//int pacmanScore = 0;
	int steps_num = 0;
	bool pacmanWon = false;
	bool ghostsWon = false;
	


	//constructor
	Game();

	// Getters
	int				GetSteps() { return steps_num; }
	
	bool			IsPacmanWon() { return pacmanWon; }
	bool			IsGhostWon() { return ghostsWon; }
	

	// Setters
	void SetSteps(int steps) { this->steps_num = steps; }
	void SetPacmanWon(bool flag) { this->pacmanWon = flag; }
	void SetGhostsWon(bool flag) { this->ghostsWon = flag; }
	
	
	void DrawMaze();
	// Functions
	void	PlayGame();
	void	Play(Character* character);
	

};