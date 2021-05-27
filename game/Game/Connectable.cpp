#include "GameClasses.h"

void Connectable::Disconect()
{
	if (parent != nullptr)
		parent->UnAttach(this);

	parent = nullptr;
}