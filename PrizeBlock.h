#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Brick.h"

#define PRIZEBLOCK_STATE_MYSTIC 100
#define PRIZEBLOCK_STATE_KNOWN_MOVING_UP 101
#define PRIZEBLOCK_STATE_KNOWN_MOVING_DOWN 102
#define PRIZEBLOCK_STATE_KNOWN_STATIC 103

#define ID_ANI_PRIZEBLOCK_MYSTIC 21001
#define ID_ANI_PRIZEBLOCK_KNOWN 21002


#define PRIZEBLOCK_GRAVITY 0.002f
#define PRIZEBLOCK_BOUNCE_SPEED_Y 0.5f

#define BOUNCING_TIMEOUT 500


class CPrizeBlock : public CBrick
{
protected:
	float ay;
	int bounce_start;	
	virtual int IsCollidable() { return 1; };
	virtual void Render();
public:
	CPrizeBlock(float x, float y) : CBrick(x, y) {		
		ay = 0;
		bounce_start = -1;
		SetState(PRIZEBLOCK_STATE_MYSTIC);
	}	
	void Update(DWORD dt) {}	
	virtual void SetState(int state);
};

