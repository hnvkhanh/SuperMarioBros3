#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Brick.h"

#define PRIZEBLOCK_STATE_MYSTIC 100
#define PRIZEBLOCK_STATE_KNOWN 101

#define ID_ANI_PRIZEBLOCK_MYSTIC 21001
#define ID_ANI_PRIZEBLOCK_KNOWN 21002


class CPrizeBlock : public CBrick
{
protected:
	float ay;
	virtual int IsCollidable() { return 1; };
public:
	CPrizeBlock(float x, float y) : CBrick(x, y) {}	
	void Update(DWORD dt) {}		
	virtual void SetState(int state);
};

