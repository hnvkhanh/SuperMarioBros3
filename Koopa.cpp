#include "Koopa.h"
#include "PrizeBlock.h"

#include "debug.h"

CKoopa::CKoopa(float x, float y, int c) :CGameObject(x, y)
{	
	this->color = c;
	this->ax = 0;
	this->ay = KOOPA_GRAVITY;
	die_start = -1;
	revive_start = -1;
	SetState(KOOPA_WALK_TO_LEFT);
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_SHELL || state == KOOPA_STATE_DIE_SLIDE_LEFT || state == KOOPA_STATE_DIE_SLIDE_RIGHT)
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
		}else if (dynamic_cast<CGoomba*>(e->obj)
			&& ((state == KOOPA_STATE_DIE_SLIDE_LEFT) || (state == KOOPA_STATE_DIE_SLIDE_RIGHT))) {
			OnCollisionWithGoomba(e);					
		}
		return;
	}
	else if (dynamic_cast<CPrizeBlock*>(e->obj)) {
		if (e->obj->GetState() == PRIZEBLOCK_STATE_MYSTIC)
			OnCollisionWithPrizeBlock(e);
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
	}
}

void CKoopa::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* paragoomba = dynamic_cast<CParaGoomba*>(e->obj);
	if (paragoomba->GetState() == PARAGOOMBA_STATE_WING_FLYING || paragoomba->GetState() == PARAGOOMBA_STATE_WING_WALKING) {
		paragoomba->SetState(GOOMBA_STATE_WALKING);		
	}
	else if (paragoomba->GetState() != GOOMBA_STATE_DIE) {
		paragoomba->SetState(GOOMBA_HIT_BY_KOOPA);		
	}
}

void CKoopa::OnCollisionWithPrizeBlock(LPCOLLISIONEVENT e)
{
	CPrizeBlock* prizeblock = dynamic_cast<CPrizeBlock*>(e->obj);	
	if (prizeblock->GetState() == PRIZEBLOCK_STATE_MYSTIC && e->nx != 0) {
		prizeblock->SetKnownState();
		prizeblock->SetState(PRIZEBLOCK_STATE_KNOWN_STATIC);
	}
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == KOOPA_STATE_SHELL_HOLD) {
		return;
	}
	vy += ay * dt;
	vx += ax * dt;	

	if ((state == KOOPA_STATE_SHELL) && (GetTickCount64() - die_start > KOOPA_DIE_TIMEOUT))
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
	int aniId;
	if (color == 0) {
		aniId = ID_ANI_RED_KOOPA_DIE;
		switch (state) {
		case KOOPA_STATE_SHELL:
			aniId = ID_ANI_RED_KOOPA_DIE;
			break;

		case KOOPA_WALK_TO_LEFT:
			aniId = ID_ANI_RED_KOOPA_WALKING_LEFT;
			break;
		case KOOPA_WALK_TO_RIGHT:
			aniId = ID_ANI_RED_KOOPA_WALKING_RIGHT;
			break;
		case KOOPA_STATE_DIE_SLIDE_LEFT:			
		case KOOPA_STATE_DIE_SLIDE_RIGHT:
			aniId = ID_ANI_RED_KOOPA_DIE_SLIDE;
			break;
		case KOOPA_STATE_REVIVE:
			aniId = ID_ANI_RED_KOOPA_REVIVE;
			break;
		}
	}
	else {
		aniId = ID_ANI_GREEN_KOOPA_DIE;
		switch (state) {
		case KOOPA_STATE_SHELL:
			aniId = ID_ANI_GREEN_KOOPA_DIE;
			break;

		case KOOPA_WALK_TO_LEFT:
			aniId = ID_ANI_GREEN_KOOPA_WALKING_LEFT;
			break;
		case KOOPA_WALK_TO_RIGHT:
			aniId = ID_ANI_GREEN_KOOPA_WALKING_RIGHT;
			break;
		case KOOPA_STATE_DIE_SLIDE_LEFT:
		case KOOPA_STATE_DIE_SLIDE_RIGHT:
			aniId = ID_ANI_GREEN_KOOPA_DIE_SLIDE;
			break;
		case KOOPA_STATE_REVIVE:
			aniId = ID_ANI_GREEN_KOOPA_REVIVE;
			break;
		}
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
	case KOOPA_STATE_SHELL_HOLD:
		vx = 0;
		vy = 0;
		ay = 0;
		ax = 0;
		is_hold = true;
		break;
	case KOOPA_STATE_SHELL:
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


// koopa paratroopa

void CParaTroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if ((state == PARATROOPA_STATE_FLY_UP) && (GetTickCount64() - fly_start > PARATROOPA_FLY_TIMEOUT)) {
		SetState(PARATROOPA_STATE_FLY_DOWN);
	}

	CKoopa::Update(dt, coObjects);

	
}

void CParaTroopa::Render()
{
	int aniId;
	if (color == 0) {
		aniId = ID_ANI_RED_KOOPA_DIE;
		switch (state) {
		case KOOPA_STATE_SHELL:
			aniId = ID_ANI_RED_KOOPA_DIE;
			break;

		case KOOPA_WALK_TO_LEFT:
			aniId = ID_ANI_RED_KOOPA_WALKING_LEFT;
			break;
		case KOOPA_WALK_TO_RIGHT:
			aniId = ID_ANI_RED_KOOPA_WALKING_RIGHT;
			break;
		case KOOPA_STATE_DIE_SLIDE_LEFT:
		case KOOPA_STATE_DIE_SLIDE_RIGHT:
			aniId = ID_ANI_RED_KOOPA_DIE_SLIDE;
			break;
		case KOOPA_STATE_REVIVE:
			aniId = ID_ANI_RED_KOOPA_REVIVE;
			break;
		case PARATROOPA_STATE_FLY_UP:
		case PARATROOPA_STATE_FLY_DOWN:
			if (go_left) {
				aniId = ID_ANI_RED_PARATROOPA_FLY_LEFT;
			}
			else {
				aniId = ID_ANI_RED_PARATROOPA_FLY_RIGHT;
			}
			break;
		}
	}
	else {
		aniId = ID_ANI_GREEN_KOOPA_DIE;
		switch (state) {
		case KOOPA_STATE_SHELL:
			aniId = ID_ANI_GREEN_KOOPA_DIE;
			break;

		case KOOPA_WALK_TO_LEFT:
			aniId = ID_ANI_GREEN_KOOPA_WALKING_LEFT;
			break;
		case KOOPA_WALK_TO_RIGHT:
			aniId = ID_ANI_GREEN_KOOPA_WALKING_RIGHT;
			break;
		case KOOPA_STATE_DIE_SLIDE_LEFT:
		case KOOPA_STATE_DIE_SLIDE_RIGHT:
			aniId = ID_ANI_GREEN_KOOPA_DIE_SLIDE;
			break;
		case KOOPA_STATE_REVIVE:
			aniId = ID_ANI_GREEN_KOOPA_REVIVE;
			break;
		case PARATROOPA_STATE_FLY_UP:
		case PARATROOPA_STATE_FLY_DOWN:
			if (go_left) {
				aniId = ID_ANI_GREEN_PARATROOPA_FLY_LEFT;
			}
			else {
				aniId = ID_ANI_GREEN_PARATROOPA_FLY_RIGHT;
			}
			break;
		}
	}
	

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	/*RenderBoundingBox();*/
}

void CParaTroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) {
		if (dynamic_cast<CParaGoomba*>(e->obj)
			&& ((state == KOOPA_STATE_DIE_SLIDE_LEFT) || (state == KOOPA_STATE_DIE_SLIDE_RIGHT))) {
			OnCollisionWithParaGoomba(e);
		}
		else if (dynamic_cast<CGoomba*>(e->obj)
			&& ((state == KOOPA_STATE_DIE_SLIDE_LEFT) || (state == KOOPA_STATE_DIE_SLIDE_RIGHT))) {
			OnCollisionWithGoomba(e);
		}
		return;
	}
	if (state == PARATROOPA_STATE_FLY_UP || state == PARATROOPA_STATE_FLY_DOWN) {
		if (e->ny < 0 && state == PARATROOPA_STATE_FLY_DOWN)
		{
			if (GetTickCount64() - wait_start > PARATROOPA_WAIT_TIMEOUT) {
				wait_start = -1;				
				SetState(PARATROOPA_STATE_FLY_UP);
			}
			else {
				vy = 0;
			}			
			
		}
		else if (e->ny > 0 && state == PARATROOPA_STATE_FLY_UP) {
			SetState(PARATROOPA_STATE_FLY_DOWN);			
		}
		else if (e->nx != 0)
		{
			vx = -vx;		
			go_left = !go_left;
		}
	}
	else {
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
	

}

CParaTroopa::CParaTroopa(float x, float y, int c) : CKoopa(x, y, c)
{
	go_left = true;
	fly_start = -1;
	wait_start = -1;
	SetState(PARATROOPA_STATE_FLY_DOWN);
}

void CParaTroopa::SetState(int state)
{
	CKoopa::SetState(state);

	switch (state)
	{
	case PARATROOPA_STATE_FLY_UP:
		vy = -PARATROOPA_FLY_UP_SPEED;
		ay = 0;		
		fly_start = GetTickCount64();
		break;
	case PARATROOPA_STATE_FLY_DOWN:
		vy = 0;
		ay = KOOPA_GRAVITY;		
		wait_start = GetTickCount64();
		break;
	}

}
