#include "PrizeBlock.h"

void CPrizeBlock::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  PRIZEBLOCK_STATE_KNOWN_MOVING_UP:
		ay = PRIZEBLOCK_BOUNCE_SPEED_Y;
		break;
	case  PRIZEBLOCK_STATE_KNOWN_MOVING_DOWN:
		ay = -PRIZEBLOCK_GRAVITY;
		break;
	case  PRIZEBLOCK_STATE_MYSTIC:
	case  PRIZEBLOCK_STATE_KNOWN_STATIC:
		ay = 0;
		vy = 0;
		break;	
	}
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
	RenderBoundingBox();
}
