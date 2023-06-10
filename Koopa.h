#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#include "Collision.h"

#include "Brick.h"
#include "Bush.h"
#include "Goomba.h"
#include "Coin.h"


#define KOOPA_GRAVITY 0.002f
#define KOOPA_WALKING_SPEED 0.05f
#define KOOPA_SLIDING_SPEED 0.2f


#define KOOPA_BBOX_WIDTH 18
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_DIE_TIMEOUT 3000
#define KOOPA_REVIVE_TIMEOUT 1000

#define KOOPA_STATE_WALKING 100
#define KOOPA_STATE_DIE 200
#define KOOPA_WALK_TO_LEFT 300
#define KOOPA_WALK_TO_RIGHT 400
#define KOOPA_STATE_DIE_SLIDE_LEFT 500
#define KOOPA_STATE_DIE_SLIDE_RIGHT 501
#define KOOPA_STATE_REVIVE 600

#define ID_ANI_KOOPA_WALKING_LEFT 6000
#define ID_ANI_KOOPA_WALKING_RIGHT 6001
#define ID_ANI_KOOPA_DIE 6002
#define ID_ANI_KOOPA_DIE_SLIDE 6003
#define ID_ANI_KOOPA_REVIVE 6004

class CKoopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG revive_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnCollisionWithGoomba(LPCOLLISIONEVENT e);

public:
	CKoopa(float x, float y);
	virtual void SetState(int state);
};