#include "Game.h"


Maze* BoardInstance;//global game instance for game

Game::Game()
{
	BoardInstance = new Maze();

	bool isGhost = false, isPacman = true;
	Pacman = new Character(isPacman,BoardInstance->pacman);
	for (int i = 0; i < NUM_OF_GHOSTS; i++)
	{
		Ghosts[i] = new Character(isGhost,BoardInstance->ghosts[i]);
	}

}



void Game::PlayGame()
{
	int k;  // for each ghost
	if (!ghostsWon && !pacmanWon)
	{
		for (k = 0; k < NUM_OF_GHOSTS && (!ghostsWon && !pacmanWon); k++)
			 Play(Ghosts[k]);

		if (!ghostsWon && !pacmanWon)
			 Play(Pacman);
	}

}
void Game::DrawMaze()
{
	BoardInstance->DrawMaze();
}

void Game::Play(Character* character)
{
	if (character->GetIsPacman())
		this->pacmanWon = Pacman->PlayPacman(*BoardInstance);
	else
		for (int i = 0; i < NUM_OF_GHOSTS; i++)
			this->ghostsWon = Ghosts[i]->PlayGhost(*BoardInstance, i);
		
}