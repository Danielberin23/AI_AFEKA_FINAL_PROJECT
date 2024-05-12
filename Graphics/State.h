#pragma once
#include "Character.h"

class Character;

class State
{
public:
	virtual void OnEnter(Character* pCharacter) = 0;
	virtual void Transition(Character* pCharacter) = 0;//switch to another state
	virtual void OnExit(Character* pCharacter) = 0;

};

