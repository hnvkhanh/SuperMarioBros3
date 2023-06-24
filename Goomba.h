#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define GOOMBA_GRAVITY 0.001f
#define GOOMBA_WALKING_SPEED 0.025f


#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500
#define GOOMBA_BOUNCE_TIMEOUT 80

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_HIT_BY_KOOPA 300

#define ID_ANI_GOOMBA_WALKING 5000
#define ID_ANI_GOOMBA_DIE 5001
#define ID_ANI_HIT_BY_KOOPA 5002


// PARA-GOOMBA 


#define PARAGOOMBA_FLYING_BBOX_WIDTH 20
#define PARAGOOMBA_FLYING_BBOX_HEIGHT 25

#define PARAGOOMBA_WINGWALKING_BBOX_WIDTH 16
#define PARAGOOMBA_WINGWALKING_BBOX_HEIGHT 14

#define PARAGOOMBA_STATE_WING_FLYING 410
#define PARAGOOMBA_STATE_WING_WALKING 411
#define PARAGOOMBA_STATE_WING_JUMPING 412

#define PARAGOOMBA_UP_SPEED 0.04f
#define PARAGOOMBA_JUMP_SPEED 0.04f

#define PARAGOOMBA_FLYING_TIMEOUT 600
#define PARAGOOMBA_WAIT_TIMEOUT 200
#define PARAGOOMBA_JUMP_TIMEOUT 200

#define ID_ANI_PARAGOOMBA_WALKING 5100
#define ID_ANI_PARAGOOMBA_DIE 5101
#define ID_ANI_PARAGOOMBA_HIT_BY_KOOPA 5102
#define ID_ANI_PARAGOOMBA_FLYING 5103
#define ID_ANI_PARAGOOMBA_WING_WALK 5104


class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 
	bool frozen, started_flag;
	ULONGLONG die_start;
	ULONGLONG bounce_start;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y);
	virtual void SetState(int state);	
};


//Para-Goomba


class CParaGoomba : public CGoomba
{
protected:		
	ULONGLONG fly_start, wait_start, jump_start;
	int jump_count;
	bool on_platform;
	virtual void Render();			
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	int IsCollidable() { return 1; };
	int IsBlocking() { return 0; }
public:
	CParaGoomba(float x, float y);		
	virtual void SetState(int state);
	void SetOnPlatform() {
		on_platform = !on_platform;
	};

	void StartWaitTime() {
		wait_start = GetTickCount64();
	}

};