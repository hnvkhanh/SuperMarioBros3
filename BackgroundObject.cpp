#include "BackgroundObject.h"

void CBush::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BUSH)->Render(x, y);
	/*RenderBoundingBox();*/
}

void CBush::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BUSH_BBOX_WIDTH / 2;
	t = y - BUSH_BBOX_HEIGHT / 2;
	r = l + BUSH_BBOX_WIDTH;
	b = t + BUSH_BBOX_HEIGHT;
}

void CSingleBackgroundCloud::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_SINGLE_BG_CLOUD)->Render(x, y);
	/*RenderBoundingBox();*/
}

void CSingleBackgroundCloud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SINGLE_BG_CLOUD_BBOX_WIDTH / 2;
	t = y - SINGLE_BG_CLOUD_BBOX_HEIGHT / 2;
	r = l + SINGLE_BG_CLOUD_BBOX_WIDTH;
	b = t + SINGLE_BG_CLOUD_BBOX_HEIGHT;
}
