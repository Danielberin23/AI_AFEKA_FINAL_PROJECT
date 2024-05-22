#pragma once
#include <iostream>
#include <math.h>

const int SPACE = 0; // Represents a blank space where we can "travel" in the maze
const int WALL = 1; // Represents the Walls of the Maze
const int PACMAN = 2; 
const int TARGET = 3; 
const int BLACK = 4; // visited
const int GRAY = 5; // Neighbors to visit
const int PATH = 6; // The path found from PACMAN to TARGET
const int FOOD = 7;
const int GHOST_1 = 8;
const int GHOST_2 = 9;
const int GHOST_3 = 10;
const int			WIDTH = 1000;
const int			HEIGHT = 1000;
const int			NUM_OF_GHOSTS = 2;
const int			NUM_OF_FOOD = 4;

class Cell {
private:
	int row, column;
	int h;
	int g;
	int f;//A* f=g+h
	Cell* parent;
	int identity;
	bool isFood;
	double safeDistance;

public:
	//Constructors
	Cell();
	Cell(Cell* other);
	Cell(int row, int column);

	//Destructor
	~Cell();

	//Getters
	int			GetH() { return h; }
	int			GetG() { return g; }
	int			GetF() { return f; }

	int			GetRow() { return row; }
	int			GetColumn() { return column; }
	Cell*		GetParent() { return parent; }
	bool		IsFood() { return isFood; }
	int			GetIdentity() { return identity; }
	double		GetSafeDistance() { return safeDistance; }

	//Setters
	void SetH(int h) { this->h = h; SetF(); }
	void SetG(int g) { this->g = g; SetF(); }
	void SetF() { this->f = GetG() + GetH(); }

	void SetParent(Cell* parent) { this->parent = parent; }
	void SetRow(int r) { this->row = r; }
	void SetColumn(int c) { this->column = c; }
	void SetFood(bool flag) { this->isFood = flag; }
	void SetIdentity(int identity) { this->identity = identity; }
	void SetSafeDistance(double safeDistance) { this->safeDistance = safeDistance; }
	
	//calculate weight and Heuristic distance
	int CalculateH(int targetRow, int targetCol);
	int CalculateG();
};
