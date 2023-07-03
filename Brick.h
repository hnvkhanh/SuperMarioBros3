#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BRICK 10000
#define ID_ANI_GLASS_BRICK 10003
#define BRICK_WIDTH 15
#define BRICK_BBOX_WIDTH 15
#define BRICK_BBOX_HEIGHT 15

class CBrick : public CGameObject {
public:
	CBrick(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class CGlassBrick : public CBrick {
public:
	CGlassBrick(float x, float y) : CBrick(x, y) {}
	void Render();
	void BrickTransformCoin();
};