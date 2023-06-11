#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

// bush
#define ID_ANI_BUSH 70000
#define BUSH_WIDTH 135
#define BUSH_BBOX_WIDTH 135
#define BUSH_BBOX_HEIGHT 66

//cloud
#define ID_ANI_SINGLE_BG_CLOUD 71000
#define SINGLE_BG_CLOUD_WIDTH 135
#define SINGLE_BG_CLOUD_BBOX_WIDTH 135
#define SINGLE_BG_CLOUD_BBOX_HEIGHT 66

class CBush : public CGameObject {
public:
	CBush(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; };	
};

class CSingleBackgroundCloud : public CGameObject {
public:
	CSingleBackgroundCloud(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; };
};





