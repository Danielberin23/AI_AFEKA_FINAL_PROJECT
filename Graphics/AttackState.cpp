#include "AttackState.h"
#include "ChaseState.h"
#include "Game.h"


void AttackState::OnEnter(Character* pCharacter)
{
	pCharacter->SetIsAttacking(true); // fighting for 2 iterations
	// found pacman 
	pCharacter->SetIsChasing(false); 
}
void AttackState::Transition(Character* pCharacter)
{
	OnExit(pCharacter);
	pCharacter->SetCurrentState(new ChaseState());
	pCharacter->GetCurrentState()->OnEnter(pCharacter);
}
void AttackState::OnExit(Character* pCharacter)
{
	pCharacter->SetIsAttacking(false);
	pCharacter->SetIsChasing(true);
		
}