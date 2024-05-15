#include "AttackState.h"
#include "Game.h"


void AttackState::OnEnter(Character* pCharacter)
{
	pCharacter->SetIsAttacking(true); // fighting for 2 iterations

}
void AttackState::Transition(Character* pCharacter)
{
	
}
void AttackState::OnExit(Character* pCharacter)
{
	// if ghost won
	if(1)
	{

	}

	else
		pCharacter->SetIsAttacking(false);
		
}