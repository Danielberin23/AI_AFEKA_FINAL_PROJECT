#pragma once
#include "State.h"


//fighting scene
class AttackState : public State
{
	void OnEnter(Character* pCharacter);
	void Transition(Character* pCharacter );
	void OnExit(Character* pCharacter);
};