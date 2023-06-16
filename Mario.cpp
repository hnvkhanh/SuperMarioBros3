#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopa.h"
#include "Coin.h"
#include "Portal.h"
#include "Platform.h"
#include "Collision.h"
#include "PrizeBlock.h"
#include "Prize.h"
#include "Piranha.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt; 

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount64() - kick_start > MARIO_KICK_TIMEOUT)
	{
		kick_start = -1;
		kickSomething = false;		
	}
	if (GetTickCount64() - transform_start > MARIO_TRANSFORM_TIMEOUT && transforming)
	{
		transform_start = -1;
		transforming = false;
		SetLevel(MARIO_LEVEL_BIG);
	}

	isOnPlatform = false;

	if (isHolding) {
		float temp = -1; // to determine direction 
		if (nx >= 0) temp = 1;
		switch (level)
		{
		case MARIO_LEVEL_SMALL:
			koopa_shell->SetPosition(x + temp * MARIO_SMALL_BBOX_WIDTH / 2 + temp * KOOPA_BBOX_WIDTH / 2,
				y - MARIO_SMALL_BBOX_HEIGHT / 2);
			DebugOut(L"change koopa position\n");
			break;

		case MARIO_LEVEL_BIG:
			koopa_shell->SetPosition(x + temp * MARIO_BIG_BBOX_WIDTH / 2 + temp * KOOPA_BBOX_WIDTH / 2, y);
			DebugOut(L"change koopa position\n");
			break;
		}
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;	
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CRectangle*>(e->obj)) {
		DebugOut(L"collision with rectangle \n");
	}	
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}
	
	if (dynamic_cast<CParaGoomba*>(e->obj))
		OnCollisionWithParaGoomba(e);
	else if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CParaTroopa*>(e->obj))
		OnCollisionWithParaTroopa(e);
	else if (dynamic_cast<CKoopa*>(e->obj))
		OnCollisionWithKoopa(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CPrizeBlock*>(e->obj))
		OnCollisionWithPrizeBlock(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CPiranha*>(e->obj))
		OnCollisionWithPiranha(e);
	else if (dynamic_cast<CFireBall*>(e->obj))
		OnCollisionWithFireBall(e);

}

void CMario::OnCollisionWithPrizeBlock(LPCOLLISIONEVENT e)
{
	CPrizeBlock* prizeBlock = dynamic_cast<CPrizeBlock*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny > 0 && e->nx == 0)		
	{	
		if (prizeBlock->GetState() == PRIZEBLOCK_STATE_MYSTIC) {
			prizeBlock->SetKnownState();
		}
		prizeBlock->SetState(PRIZEBLOCK_STATE_KNOWN_MOVING_UP);		
	}	
	prizeBlock->ResetPosition();
	
	
}

void CMario::OnCollisionWithPiranha(LPCOLLISIONEVENT e)
{
	CPiranha* piranha = dynamic_cast<CPiranha*>(e->obj);

		
	if (untouchable == 0)
	{		
		if (level > MARIO_LEVEL_SMALL)
		{			
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
		
	}
	
}

void CMario::OnCollisionWithFireBall(LPCOLLISIONEVENT e)
{	
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level = MARIO_LEVEL_SMALL;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}

		e->obj->Delete();
	}
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{					
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* paragoomba = dynamic_cast<CParaGoomba*>(e->obj);	
	float x_goomba, y_goomba;
	paragoomba->GetPosition(x_goomba, y_goomba);
	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{					
		if (paragoomba->GetState() != GOOMBA_STATE_DIE)
		{
			if (paragoomba->GetState() == PARAGOOMBA_STATE_WING_FLYING ||
				paragoomba->GetState() == PARAGOOMBA_STATE_WING_WALKING) {				
				paragoomba->SetState(GOOMBA_STATE_WALKING);
				y_goomba -= (e->t * e->dy + e->ny * BLOCK_PUSH_FACTOR);
				paragoomba->SetPosition(x_goomba, y_goomba);
				
			}				
			else {
				paragoomba->SetState(GOOMBA_STATE_DIE);
			}
			

			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (paragoomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					DebugOut(L"touch goomba\n");
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);
	float x_koopa, y_koopa;
	koopa->GetPosition(x_koopa, y_koopa);
	if (e->ny < 0)
	{			

		if (koopa->GetState() != KOOPA_STATE_SHELL)
		{
			koopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else {							

			koopa->SetState(KOOPA_STATE_DIE_SLIDE_RIGHT);
			vy = -MARIO_JUMP_DEFLECT_SPEED;			
		}

		y_koopa -= (e->t * e->dy + e->ny * BLOCK_PUSH_FACTOR);
		koopa->SetPosition(x_koopa, y_koopa);
		
	}
	else 
	{				

		if (untouchable == 0)
		{
			if (koopa->GetState() == KOOPA_STATE_DIE_SLIDE_LEFT || koopa->GetState() == KOOPA_STATE_DIE_SLIDE_RIGHT)
			{	
				
				if (level > MARIO_LEVEL_SMALL)
				{					
					level = MARIO_LEVEL_SMALL;					
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
			else {
				if (e->nx != 0) {
					/*DebugOut(L"koopa state %d\n", koopa->GetState());
					DebugOut(L"mario state %d\n", state);*/
					if (ready_to_hold && (koopa->GetState() == KOOPA_STATE_SHELL || koopa->GetState() == KOOPA_STATE_REVIVE )) {
						koopa_shell = koopa;
						DebugOut(L"state ready\n");
						koopa->IsHold();
						koopa->SetState(KOOPA_STATE_SHELL_HOLD);
						float temp = -1; // to determine direction 
						if (nx >= 0) temp = 1;
						switch (level)
						{
						case MARIO_LEVEL_SMALL:
							koopa->SetPosition(x + temp * MARIO_SMALL_BBOX_WIDTH/2 + temp * KOOPA_BBOX_WIDTH / 2,
								y - MARIO_SMALL_BBOX_HEIGHT / 2);		
							DebugOut(L"change koopa position\n");
							break;

						case MARIO_LEVEL_BIG:
							koopa->SetPosition(x + temp * MARIO_BIG_BBOX_WIDTH/2 + temp * KOOPA_BBOX_WIDTH/2, y);
							DebugOut(L"change koopa position\n");
							break;
						}
						isHolding = true;
					}
					else {
						if (e->nx > 0 && !isHolding) {
							SetState(MARIO_STATE_KICK_LEFT);
							koopa->SetState(KOOPA_STATE_DIE_SLIDE_LEFT);
						}
						else if (e->nx < 0 && !isHolding) {
							SetState(MARIO_STATE_KICK_RIGHT);
							koopa->SetState(KOOPA_STATE_DIE_SLIDE_RIGHT);

						}
					}
					
				}						

			}
		}
	}
}

void CMario::OnCollisionWithParaTroopa(LPCOLLISIONEVENT e)
{
	CParaTroopa* koopa = dynamic_cast<CParaTroopa*>(e->obj);

	if (koopa->GetState() == PARATROOPA_STATE_FLY_DOWN && koopa->GetState() == PARATROOPA_STATE_FLY_UP) {
		if (e->ny < 0)
		{
			if (koopa->GetGoLeft()) {
				koopa->SetState(KOOPA_WALK_TO_LEFT);
			}
			else {
				koopa->SetState(KOOPA_WALK_TO_RIGHT);
			}
		}
		else {
			OnCollisionWithKoopa(e);
		}		
	}
	else {
		OnCollisionWithKoopa(e);
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	e->obj->Delete();	
	if (level == MARIO_LEVEL_SMALL) {
		DebugOut(L"transforming\n");
		SetState(MARIO_STATE_TRANSFORM_TO_BIG);
	}
	
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else		
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else if (isHolding) {
			if (nx > 0)
				aniId = ID_ANI_MARIO_SMALL_HOLD_STAND_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_HOLD_STAND_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
			}
			if (kickSomething) {
				if (nx > 0)
					aniId = ID_ANI_MARIO_SMALL_KICK_RIGHT;
				else
					aniId = ID_ANI_MARIO_SMALL_KICK_LEFT;
			}
			else if (transforming) {
				if (nx > 0)
					aniId = ID_ANI_TRANSFORM_TO_BIG_RIGHT;
				else
					aniId = ID_ANI_TRANSFORM_TO_BIG_LEFT;
			}
	
	

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{			
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}		
		else
		{
			if (isHolding) {				

				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_BIG_HOLD_STAND_RIGHT;
					else aniId = ID_ANI_MARIO_BIG_HOLD_STAND_LEFT;
				}
				else if (vx > 0)
				{
					aniId = ID_ANI_MARIO_BIG_HOLD_RUN_RIGHT;					
				}
				else // vx < 0
				{
					aniId = ID_ANI_MARIO_BIG_HOLD_RUN_LEFT;
				}
				
			}
			else {
				if (vx == 0)
				{
					if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
					else aniId = ID_ANI_MARIO_IDLE_LEFT;
				}
				else if (vx > 0)
				{
					if (ax < 0)
						aniId = ID_ANI_MARIO_BRACE_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
						aniId = ID_ANI_MARIO_RUNNING_RIGHT;
					else if (ax == MARIO_ACCEL_WALK_X)
						aniId = ID_ANI_MARIO_WALKING_RIGHT;
				}
				else // vx < 0
				{
					if (ax > 0)
						aniId = ID_ANI_MARIO_BRACE_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X)
						aniId = ID_ANI_MARIO_RUNNING_LEFT;
					else if (ax == -MARIO_ACCEL_WALK_X)
						aniId = ID_ANI_MARIO_WALKING_LEFT;
				}
				if (kickSomething) {
					if (nx > 0)
						aniId = ID_ANI_MARIO_KICK_RIGHT;
					else
						aniId = ID_ANI_MARIO_KICK_LEFT;
				}
			}
			
		}
		
			

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 
	ready_to_hold = false;

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	case MARIO_STATE_KICK_LEFT:
	case MARIO_STATE_KICK_RIGHT:
		kickSomething = true;
		kick_start = GetTickCount64();
		break;
	case MARIO_STATE_TRANSFORM_TO_BIG:		
		DebugOut(L"transform happen\n");
		transforming = true;
		transform_start = GetTickCount64();		
		break;
	case MARIO_STATE_READY_HOLD:
		ready_to_hold = true;
		break;
	case MARIO_STATE_HOLD_RELEASE:
		isHolding = false;
		if (koopa_shell != NULL) {
			if (nx > 0) {
				koopa_shell->SetState(KOOPA_STATE_DIE_SLIDE_RIGHT);
			}
			else {
				koopa_shell->SetState(KOOPA_STATE_DIE_SLIDE_LEFT);
			}
			koopa_shell = NULL;
		}
		
		break;
	}
	

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	DebugOut(L"change level state %d\n", state);
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

