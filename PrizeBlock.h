#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Brick.h"

#define PRIZEBLOCK_STATE_MYSTIC 100
#define PRIZEBLOCK_STATE_KNOWN_MOVING_UP 101
#define PRIZEBLOCK_STATE_KNOWN_MOVING_DOWN 102
#define PRIZEBLOCK_STATE_KNOWN_STATIC 103

#define ID_ANI_PRIZEBLOCK_MYSTIC 10001
#define ID_ANI_PRIZEBLOCK_KNOWN 10002


#define PRIZEBLOCK_BOUNCE_SPEED_Y 0.002f

#define BOUNCING_TIMEOUT 50


class CPrizeBlock : public CBrick
{
protected:
	float ay;
	int bounce_start;	
	float x_fixed, y_fixed;
	int IsCollidable(float nx, float ny);
	int IsBlocking() { return 1; };
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
public:
	CPrizeBlock(float x, float y) : CBrick(x, y) {		
		ay = 0;
		x_fixed = x;
		y_fixed = y;
		bounce_start = -1;
		SetState(PRIZEBLOCK_STATE_MYSTIC);
	}	
	void Update(DWORD dt) {}	
	void ResetPosition();
	virtual void SetState(int state);
};

