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
			 Play(Ghosts[k],k);

		if (!ghostsWon && !pacmanWon)
			 Play(Pacman,-1);
	}
	else
		if(ghostsWon)
			cout << "Ghosts Won!" << endl;
		else
			cout << "Pacman Won!" << endl;
}
void Game::DrawMaze()
{
	BoardInstance->DrawMaze();
}

void Game::Play(Character* character,int ghostPlayed)
{
	if (character->GetIsPacman() && ghostPlayed == -1)
		this->pacmanWon = Pacman->PlayPacman(*BoardInstance);
	else
		this->ghostsWon = Ghosts[ghostPlayed]->PlayGhost(*BoardInstance, ghostPlayed);
	BoardInstance->cleanParents();
}