#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "PSwitch.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_GLASS_BRICK 10003
#define ID_ANI_GLASS_BRICK_KNOWN 10002

#define BRICK_WIDTH 15
#define BRICK_BBOX_WIDTH 15
#define BRICK_BBOX_HEIGHT 15

#define GLASSBRICK_CONTAIN_COIN					0
#define GLASSBRICK_CONTAIN_PSWITCH				1

class CBrick : public CGameObject {
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class CGlassBrick : public CBrick {
protected:
	int containObject;
	int idAni;
public:
	CGlassBrick(float x, float y, int contain_object=0) : CBrick(x, y) {
		containObject = contain_object;
		idAni = ID_ANI_GLASS_BRICK;
	}
	void Render();
	void BrickTransformCoin();
	void BrickBreak();
};