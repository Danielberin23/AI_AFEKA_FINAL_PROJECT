#include "Cell.h"



Cell::Cell()
{
	SetIdentity(SPACE);
	SetParent(nullptr);
	
}
Cell::Cell(Cell* other)
{
	SetIdentity(other->GetIdentity());
	SetRow(other->GetRow());
	SetColumn(other->GetColumn());
	if (other->GetParent() == nullptr)
		SetParent(nullptr);
	else
		SetParent(new Cell(other->GetParent()));
}

Cell::Cell(int row, int column)
{
	this->row = row;
	this->column = column;
	SetIdentity(SPACE);
    SetParent(nullptr);
}


Cell::~Cell(){}

int Cell::CalculateH(int targetRow,int targetCol)
{
	if (parent != nullptr)
		return abs(row - targetRow) + abs(column - targetCol);
	else
		return 0;
}

int Cell::CalculateG()
{
	if (parent != nullptr)
		return abs(row - parent->GetRow()) + abs(column - parent->GetColumn());
	else
		return 0;
}
