#pragma once
#include "Cell.h"

class CellComparator
{
public:
	bool  operator() (Cell* pc1, Cell* pc2)
	{
		return (pc1->GetF() > pc2->GetF());
	}
};