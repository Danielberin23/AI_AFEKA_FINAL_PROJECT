#include "EatingState.h"
#include "ChaseState.h"


void EatingState::OnEnter(Character* pCharacter)
{
	pCharacter->SetIsMoving(true);
}
void EatingState::Transition(Character* pCharacter)
{

	//ghost is found, run away
	OnExit(pCharacter);
	pCharacter->SetCurrentState(new ChaseState());
	pCharacter->GetCurrentState()->OnEnter(pCharacter);
}
void EatingState::OnExit(Character* pCharacter)
{
	
	pCharacter->SetIsMoving(false);
	pCharacter->SetIsChasing(true);//ghost is chasing pacman
}