#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Coin.h"
#include "Brick.h"

#define ID_ANI_PIRANHA 80001

#define PIRANHA_WIDTH 16
#define PIRANHA_BBOX_WIDTH 16
#define PIRANHA_BBOX_HEIGHT 22

#define PIRANHA_STATE_UP 100
#define PIRANHA_STATE_DOWN 101
#define PIRANHA_STATE_WAIT 102

#define PIRANHA_SPEED 0.025f

#define PIRANHA_TIMEOUT 1150
#define PIRANHA_WAIT_TIMEOUT 3000

// venus fire trap

#define ID_ANI_GREEN_VENUS_LOOK_DOWN_LEFT 81001
#define ID_ANI_GREEN_VENUS_LOOK_UP_LEFT 81002
#define ID_ANI_GREEN_VENUS_LOOK_DOWN_RIGHT 81003
#define ID_ANI_GREEN_VENUS_LOOK_UP_RIGHT 81004
#define ID_ANI_GREEN_VENUS_RISING 81005

#define ID_ANI_RED_VENUS_LOOK_DOWN_LEFT 82001
#define ID_ANI_RED_VENUS_LOOK_UP_LEFT 82002
#define ID_ANI_RED_VENUS_LOOK_DOWN_RIGHT 82003
#define ID_ANI_RED_VENUS_LOOK_UP_RIGHT 82004
#define ID_ANI_RED_VENUS_RISING 82005

#define VENUS_WIDTH 16
#define VENUS_BBOX_WIDTH 16
#define VENUS_BBOX_HEIGHT 32

#define VENUS_STATE_IDLE 103
#define VENUS_STATE_FIRE 104

#define VENUS_SPEED 0.03f

#define VENUS_IDLE_TIMEOUT 3000



class CPiranha : public CGameObject
{
protected:
	virtual void OnNoCollision(DWORD dt) { return; };
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) { return; };
	ULONGLONG up_start, wait_start;		
	float y_fixed, speed;
public:
	CPiranha(float x, float y) : CGameObject(x, y) {
		up_start = -1;
		wait_start = -1;		
		SetState(PIRANHA_STATE_WAIT);
		y_fixed = y;
		speed = PIRANHA_SPEED;
		
	};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	int IsBlocking() { return 0; }
	int IsCollidable()
	{
		return (state != PIRANHA_STATE_WAIT);
	}
	void ResetPosition() {
		y = y_fixed;
	}
};



class CVenusFireTrap : public CPiranha {
protected:
	ULONGLONG idle_start;
	float x_mario, y_mario;
public:
	CVenusFireTrap(float x, float y) : CPiranha(x, y) {
		idle_start = -1;
		speed = VENUS_SPEED;
		x_mario = 0;
		y_mario = 0;
	};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void SetState(int state);
	void GetMarioPosition(float x, float y);
	bool IsMarioOnLeft();
	bool IsMarioHigher();
};

