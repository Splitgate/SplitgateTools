#pragma once

#include "Games/BaseGame.h"

class Splitgate : public BaseGame
{
public:

	Splitgate();

	virtual void Init_PreEngine() override;
	virtual void Init_PostEngine() override;

	virtual void ConstructTitleBar() override;
};
