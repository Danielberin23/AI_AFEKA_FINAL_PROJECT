#include "ChaseState.h"
#include "EatingState.h"
#include "AttackState.h"
void ChaseState::OnEnter(Character* pCharacter)
{
	pCharacter->SetIsChasing(true);
	
}

void ChaseState::Transition(Character* pCharacter)
{
	OnExit(pCharacter);
	if (pCharacter->GetIsPacman())
	{
		if (pCharacter)
		{
			pCharacter->SetCurrentState(new EatingState());
			pCharacter->GetCurrentState()->OnEnter(pCharacter);
		}
		else
		{
			pCharacter->SetCurrentState(new AttackState());
			pCharacter->GetCurrentState()->OnEnter(pCharacter);
		}

	}
	else
	{
		pCharacter->SetCurrentState(new AttackState());
		pCharacter->GetCurrentState()->OnEnter(pCharacter);
	}
		
}
void ChaseState::OnExit(Character* pCharacter)
{
	pCharacter->SetIsChasing(false);
}