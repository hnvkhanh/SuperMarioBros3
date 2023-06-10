#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_BUSH 70000
#define BUSH_WIDTH 135
#define BUSH_BBOX_WIDTH 135
#define BUSH_BBOX_HEIGHT 66

class CBush : public CGameObject {
public:
	CBush(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; };	
};



