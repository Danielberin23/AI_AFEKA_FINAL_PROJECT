#include "AttackState.h"
#include "EatingState.h"
#include "Game.h"


void AttackState::OnEnter(Character* pCharacter)
{
	pCharacter->SetIsChasing(false);
	pCharacter->SetIsAttacking(true); // fighting for X iterations
	 
}
//only pacman can move from attack state
void AttackState::Transition(Character* pCharacter)
{
	OnExit(pCharacter);
	pCharacter->SetCurrentState(new EatingState());
	pCharacter->GetCurrentState()->OnEnter(pCharacter);
}
void AttackState::OnExit(Character* pCharacter)
{
	pCharacter->SetIsAttacking(false);
	pCharacter->SetIsMoving(true);
		
}