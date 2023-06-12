#include "Piranha.h"

void CPiranha::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIRANHA_BBOX_WIDTH / 2;
	t = y - PIRANHA_BBOX_HEIGHT / 2;
	r = l + PIRANHA_BBOX_WIDTH;
	b = t + PIRANHA_BBOX_HEIGHT;
}

void CPiranha::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_PIRANHA)->Render(x, y);

	RenderBoundingBox();
}

void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	

	if ((state == PIRANHA_STATE_UP) && (GetTickCount64() - up_start > PIRANHA_TIMEOUT))
	{
		DebugOut(L"piranha go down\n");
		SetState(PIRANHA_STATE_DOWN);
	} 
	else if ((state == PIRANHA_STATE_DOWN) && (GetTickCount64() - up_start > PIRANHA_TIMEOUT))
	{
		DebugOut(L"piranha go up\n");
		SetState(PIRANHA_STATE_WAIT);
	}
	else if ((state == PIRANHA_STATE_WAIT) && (GetTickCount64() - wait_start > PIRANHA_WAIT_TIMEOUT))
	{
		DebugOut(L"piranha go up\n");
		SetState(PIRANHA_STATE_UP);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CPiranha::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  PIRANHA_STATE_UP:		
		vy = -PIRANHA_SPEED;	
		wait_start = -1;
		up_start = GetTickCount64();
		break;
	case  PIRANHA_STATE_DOWN:
		vy = PIRANHA_SPEED;		
		wait_start = -1;
		up_start = GetTickCount64();
		break;
	case  PIRANHA_STATE_WAIT:
		vy = 0;		
		wait_start = GetTickCount64();
		up_start = -1;
		break;
	}
}
