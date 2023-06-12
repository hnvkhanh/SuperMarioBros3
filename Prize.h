#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#include "Brick.h"

#define MUSHROOM_SPEED 0.01f

#define ID_ANI_MUSHROOM 11100

#define	MUSHROOM_WIDTH 15
#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16

#define MUSHROOM_STATE_RISING 100
#define MUSHROOM_STATE_MOVING 101



class CMushroom : public CGameObject
{
protected:
	float y_rise;
public:
	CMushroom(float x, float y) : CGameObject(x, y) {
		this->y_rise = y - BRICK_BBOX_HEIGHT/2;
		this->vy = -MUSHROOM_SPEED;
	};
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();	
	int IsBlocking() { return 0; }
};

