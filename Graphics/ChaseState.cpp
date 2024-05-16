#include "ChaseState.h"
#include "EatingState.h"
#include "AttackState.h"
void ChaseState::OnEnter(Character* pCharacter)
{
	if(pCharacter->GetIsPacman())
		pCharacter->SetIsMoving(false);
	pCharacter->SetIsChasing(true);
	
}

void ChaseState::Transition(Character* pCharacter)
{
	OnExit(pCharacter);
	pCharacter->SetCurrentState(new AttackState());
	pCharacter->GetCurrentState()->OnEnter(pCharacter);
		
}
void ChaseState::OnExit(Character* pCharacter)
{
	pCharacter->SetIsChasing(false);
}