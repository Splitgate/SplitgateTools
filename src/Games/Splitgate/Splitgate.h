#pragma once

#include "Games/BaseGame.h"

class Splitgate : public BaseGame
{
public:
	virtual void Init_PreEngine() override;
	virtual void Init_PostEngine() override;
};
