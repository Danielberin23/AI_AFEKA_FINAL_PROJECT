#include "AttackState.h"


void AttackState::OnEnter(Character* pCharacter)
{
	pCharacter->SetIsMoving(false); // fighting for 2 iterations
}
void AttackState::Transition(Character* pCharacter)
{

}
void AttackState::OnExit(Character* pCharacter)
{

}