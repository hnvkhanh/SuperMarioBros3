#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Coin.h"
#include "Brick.h"

// Mushroom
#define MUSHROOM_SPEED 0.07f
#define MUSHROOM_RISING_SPEED 0.01f
#define MUSHROOM_GRAVITY 0.001f

#define ID_ANI_MUSHROOM 11100

#define	MUSHROOM_WIDTH 15
#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_STATE_RISING 100
#define MUSHROOM_STATE_MOVING 101

//bounce up coin
#define ID_ANI_COIN_SPEED_UP 11001

#define COIN_STATE_BOUNCE_UP 200
#define COIN_STATE_DROP_DOWN 201

#define COIN_BOUNCE_SPEED 0.2f

#define COIN_BOUNCING_TIMEOUT 200

class CMushroom : public CGameObject
{
protected:
	float y_rise, ay;
	int state;	
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual int IsCollidable() { return 1; };
public:
	CMushroom(float x, float y) : CGameObject(x, y) {
		this->y_rise = y - BRICK_BBOX_HEIGHT/2;	
		this->ay = 0;		
		SetState(MUSHROOM_STATE_RISING);
	};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();	
	int IsBlocking() { return 0; }
	virtual void SetState(int state);
};


class CBouncingCoin : public CCoin
{
protected:
	float y_rise;	
	ULONGLONG bounce_start;
public:
	CBouncingCoin(float x, float y) : CCoin(x, y) {
		this->y_rise = 100;		
		bounce_start = -1;
		SetState(COIN_STATE_BOUNCE_UP);
	};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	int IsBlocking() { return 0; }	
	virtual void SetState(int state);
};
