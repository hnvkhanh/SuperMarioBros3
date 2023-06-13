#include "PrizeBlock.h"

void CPrizeBlock::ResetPosition()
{
	x = x_fixed;
	y = y_fixed;
	return;
}

void CPrizeBlock::SetKnownState()
{
	if (isKnown == 0) {
		isKnown = 1;
	}
	else if (isKnown == 1) {
		isKnown = -1;
	}
}

void CPrizeBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  PRIZEBLOCK_STATE_KNOWN_MOVING_UP:
		ay = -PRIZEBLOCK_BOUNCE_SPEED_Y;
		bounce_start = GetTickCount64();		
		break;
	case  PRIZEBLOCK_STATE_KNOWN_MOVING_DOWN:
		vy = -vy;
		ay = PRIZEBLOCK_BOUNCE_SPEED_Y;
		ay = 0;
		bounce_start = GetTickCount64();
		break;
	case  PRIZEBLOCK_STATE_MYSTIC:
	case  PRIZEBLOCK_STATE_KNOWN_STATIC:
		ResetPosition();
		ay = 0;
		vy = 0;
		break;	
	}
}

int CPrizeBlock::IsCollidable(float nx, float ny)
{
	/*if (nx == 0 && ny == -1) return 1;
	else return 0;*/
	return 0;
}

void CPrizeBlock::Render()
{
	int aniId = ID_ANI_PRIZEBLOCK_MYSTIC;
	if (state == PRIZEBLOCK_STATE_MYSTIC)
	{
		aniId = ID_ANI_PRIZEBLOCK_MYSTIC;
	}
	else if (state == PRIZEBLOCK_STATE_KNOWN_MOVING_DOWN 
		|| state == PRIZEBLOCK_STATE_KNOWN_MOVING_UP 
		|| state == PRIZEBLOCK_STATE_KNOWN_STATIC) {
		aniId = ID_ANI_PRIZEBLOCK_KNOWN;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	/*RenderBoundingBox();*/
}

void CPrizeBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;	
	y += vy * dt;

	if ((state == PRIZEBLOCK_STATE_KNOWN_MOVING_UP) && (GetTickCount64() - bounce_start > BOUNCING_TIMEOUT))
	{			
		SetState(PRIZEBLOCK_STATE_KNOWN_MOVING_DOWN);				
	}
	else if ((state == PRIZEBLOCK_STATE_KNOWN_MOVING_DOWN) && (GetTickCount64() - bounce_start > BOUNCING_TIMEOUT))
	{		
		SetState(PRIZEBLOCK_STATE_KNOWN_STATIC);
	}

	

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}