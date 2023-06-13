#include "Koopa.h"

#include "debug.h"

CKoopa::CKoopa(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	revive_start = -1;
	SetState(KOOPA_WALK_TO_LEFT);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_DIE || state == KOOPA_STATE_DIE_SLIDE_LEFT || state == KOOPA_STATE_DIE_SLIDE_RIGHT)
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT_DIE;
	}
	else
	{
		left = x - KOOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
}

void CKoopa::OnNoCollision(DWORD dt)
{		
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) {	
		if (dynamic_cast<CParaGoomba*>(e->obj)
			&& ((state == KOOPA_STATE_DIE_SLIDE_LEFT) || (state == KOOPA_STATE_DIE_SLIDE_RIGHT))) {
			OnCollisionWithParaGoomba(e);
			DebugOut(L"on collision with paragoomba\n");			
		}else if (dynamic_cast<CGoomba*>(e->obj)
			&& ((state == KOOPA_STATE_DIE_SLIDE_LEFT) || (state == KOOPA_STATE_DIE_SLIDE_RIGHT))) {
			OnCollisionWithGoomba(e);					
		}
		return;
	}		

	if (e->ny != 0)
	{		
		vy = 0;				
	}
	else if (e->nx != 0)
	{					
		vx = -vx;				
		
		if (state == KOOPA_WALK_TO_RIGHT) {
			SetState(KOOPA_WALK_TO_LEFT);
		}
		else if (state == KOOPA_WALK_TO_LEFT) {
			SetState(KOOPA_WALK_TO_RIGHT);
		}
	}
	
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	if (goomba->GetState() != GOOMBA_STATE_DIE) {
		goomba->SetState(GOOMBA_HIT_BY_KOOPA);
		DebugOut(L"set state goomba");
	}
}

void CKoopa::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* paragoomba = dynamic_cast<CParaGoomba*>(e->obj);
	if (paragoomba->GetState() == PARAGOOMBA_STATE_WING_FLYING || paragoomba->GetState() == PARAGOOMBA_STATE_WING_WALKING) {
		paragoomba->SetState(GOOMBA_STATE_WALKING);
		DebugOut(L"change to state walking\n");
	}
	else if (paragoomba->GetState() != GOOMBA_STATE_DIE) {
		paragoomba->SetState(GOOMBA_HIT_BY_KOOPA);		
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;	

	if ((state == KOOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
	{
		SetState(KOOPA_STATE_REVIVE);
	}
	else if ((state == KOOPA_STATE_REVIVE) && (GetTickCount64() - revive_start > KOOPA_REVIVE_TIMEOUT))
	{
		SetState(KOOPA_WALK_TO_LEFT);
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CKoopa::Render()
{
	int aniId = ID_ANI_KOOPA_DIE;
	switch (state) {
	case KOOPA_STATE_DIE:
		aniId = ID_ANI_KOOPA_DIE;
		break;

	case KOOPA_WALK_TO_LEFT:
		aniId = ID_ANI_KOOPA_WALKING_LEFT;
		break;
	case KOOPA_WALK_TO_RIGHT:
		aniId = ID_ANI_KOOPA_WALKING_RIGHT;
		break;
	case KOOPA_STATE_DIE_SLIDE_LEFT:
	case KOOPA_STATE_DIE_SLIDE_RIGHT:
		aniId = ID_ANI_KOOPA_DIE_SLIDE;
		break;
	case KOOPA_STATE_REVIVE:
		aniId = ID_ANI_KOOPA_REVIVE;
		break;
	}
	

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	/*RenderBoundingBox();*/
}

void CKoopa::SetState(int state)
{
	int previous_state = this->GetState();
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_DIE:
		die_start = GetTickCount64();				
		if (previous_state != KOOPA_STATE_DIE_SLIDE_LEFT && previous_state != KOOPA_STATE_DIE_SLIDE_RIGHT) {
			y += (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2;
		}		
		vx = 0;
		vy = 0;		
		ay = KOOPA_GRAVITY;
		break;
		
	case KOOPA_WALK_TO_LEFT:
		if (previous_state == KOOPA_STATE_REVIVE) {
			y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2;	
			ay = KOOPA_GRAVITY;
		}
		vx = -KOOPA_WALKING_SPEED;
		break;	
	case KOOPA_WALK_TO_RIGHT:
		if (previous_state == KOOPA_STATE_REVIVE) {
			y -= (KOOPA_BBOX_HEIGHT - KOOPA_BBOX_HEIGHT_DIE) / 2;	
			ay = KOOPA_GRAVITY;
		}
		vx = KOOPA_WALKING_SPEED;
		break;
	case KOOPA_STATE_DIE_SLIDE_RIGHT:
		vx = KOOPA_SLIDING_SPEED;		
		ay = KOOPA_GRAVITY;
		break;	
	case KOOPA_STATE_DIE_SLIDE_LEFT:
		vx = -KOOPA_SLIDING_SPEED;	
		ay = KOOPA_GRAVITY;
		break;
	case KOOPA_STATE_REVIVE:
		revive_start = GetTickCount64();
		ay = 0;
		vy = 0;
		break;
	}
}


