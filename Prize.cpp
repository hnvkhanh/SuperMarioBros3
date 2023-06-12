#include "Prize.h"

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y <= y_rise) {
		/*y += vy * dt;*/
		SetState(MUSHROOM_STATE_MOVING);
	}
	vy += ay * dt;
	/*y += vy * dt;
	x += vx * dt;*/
	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MUSHROOM)->Render(x, y);

	//RenderBoundingBox();
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  MUSHROOM_STATE_RISING:
		vy = -MUSHROOM_RISING_SPEED;
		break;
	case  MUSHROOM_STATE_MOVING:
		vy = 0;
		ay = MUSHROOM_GRAVITY;
		vx = MUSHROOM_SPEED;
		break;	
	}
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;	
	

	if (e->ny != 0)
	{		
		vy = 0;		
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};