#pragma once
/*****************************
Using this in case: when I should update Wing Koopas
****************************/

#include "GameObject.h"
#include "debug.h"

#define INVISIBLE_BBOX_WIDTH 5
#define INVISIBLE_BBOX_HEIGHT 100

class CInvisibleObject : public CGameObject
{
private:
	int block;
public:

	CInvisibleObject(float x, float y) : CGameObject(x, y)
	{
		block = 0;
	}
	void Render() { 
		DebugOut(L"render bounding box\n");
		RenderBoundingBox(); 
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void OnNoCollision(DWORD dt) { return; };
	virtual int IsCollidable() { return 1; };
	int IsBlocking() { return block; }
};
