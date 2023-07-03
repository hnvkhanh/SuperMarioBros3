#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_BY_NONE					0
#define COIN_TRANSFORMED_FROM_BRICK		1

#define COIN_TIMEOUT					10000

class CCoin : public CGameObject {
private:

	int type;

	ULONGLONG remain_start;

	void CoinTransformBrick();
public:
	CCoin(float x, float y, int type = 0) : CGameObject(x, y) {	
		remain_start = 0;
		this->type = type;
		if (type == COIN_TRANSFORMED_FROM_BRICK) remain_start = GetTickCount64();
	}
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
};