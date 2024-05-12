#pragma once
#include "Cell.h"

class SafeDistanceComparator
{
public:
	bool operator()(Cell* c1, Cell* c2)
	{
		return c1->GetSafeDistance() < c2->GetSafeDistance();
	}

};
