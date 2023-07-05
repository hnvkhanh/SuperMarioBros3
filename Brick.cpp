#include "Brick.h"
#include "Game.h"
#include "PlayScene.h"
#include "Coin.h"
#include "Effect.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CGlassBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_GLASS_BRICK)->Render(x, y);
	RenderBoundingBox();
}

void CGlassBrick::BrickTransformCoin() {
	if (containObject == GLASSBRICK_CONTAIN_COIN
		&& (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene()) {
		this->Delete();

		CGameObject* coin = new CCoin(x, y, COIN_TRANSFORMED_FROM_BRICK);
		((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetObjects().push_back(coin);
	}
}

void CGlassBrick::BrickBreak()
{
	this->Delete();
	CEffect::GetInstance()->pushEffectIntoQueue(x, y, ID_ANI_EFFECT_BRICK_BREAK, false, false);
}
