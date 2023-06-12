#include "Prize.h"

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (y > y_rise) {
		y += vy * dt;
	}
	
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMushroom::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_MUSHROOM)->Render(x, y);

	//RenderBoundingBox();
}

void CMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - MUSHROOM_BBOX_WIDTH / 2;
	t = y - MUSHROOM_BBOX_HEIGHT / 2;
	r = l + MUSHROOM_BBOX_WIDTH;
	b = t + MUSHROOM_BBOX_HEIGHT;
}