#pragma once

#include "State.h"


class ChaseState :public State {

	void OnEnter(Character* pCharacter);
	void Transition(Character* pCharacter);
	void OnExit(Character* pCharacter);

};