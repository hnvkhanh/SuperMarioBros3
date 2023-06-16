#include "InvisibleObject.h"
#include "Koopa.h"

void CInvisibleObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - INVISIBLE_BBOX_WIDTH / 2;
	t = y - INVISIBLE_BBOX_HEIGHT / 2;
	r = l + INVISIBLE_BBOX_WIDTH;
	b = t + INVISIBLE_BBOX_HEIGHT;
}

void CInvisibleObject::OnCollisionWith(LPCOLLISIONEVENT e)
{
	
	if (dynamic_cast<CKoopa*>(e->obj))
	{		
		CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
		if (koopa->GetState() == KOOPA_STATE_SHELL_HOLD 
			|| koopa->GetState() == KOOPA_STATE_DIE_SLIDE_LEFT
			|| koopa->GetState() == KOOPA_STATE_DIE_SLIDE_RIGHT) {
			block = 0;
		}
		else {
			block = 1;
		}
	}
	else
		block = 0;
	
}

void CInvisibleObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}