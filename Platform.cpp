#include "Platform.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"

void CPlatform::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

void CPlatform::Render()
{
	if (this->length <= 0) return; 
	float xx = x; 
	CSprites * s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (length>1)
		s->Get(this->spriteIdEnd)->Draw(xx, y);

	RenderBoundingBox();
}

void CPlatform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth * this->length;
	b = t + this->cellHeight;
}

int CPlatform::IsDirectionColliable(float nx, float ny)
{
	return 1;
	if (nx == 0 && ny == -1) return 1;
	else return 0;
}


//Rectangle

void CRectangle::Render()
{
	if (this->height <= 0 || this->width <= 0) return;
	float xx = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBeginTop)->Draw(xx, y);
	xx += this->cellWidth;
	for (int i = 1; i < this->width - 1; i++)
	{
		s->Get(this->spriteIdMiddleTop)->Draw(xx, y);
		xx += this->cellWidth;
	}
	if (width > 1)
		s->Get(this->spriteIdEndTop)->Draw(xx, y);


	xx = x;
	s->Get(this->spriteIdBeginBottom)->Draw(x, y + this->cellHeight * (this->height - 1));
	xx += this->cellWidth;
	for (int i = 1; i < this->width - 1; i++)
	{
		s->Get(this->spriteIdMiddleBottom)->Draw(xx, y + this->cellHeight * (this->height - 1));
		xx += this->cellWidth;
	}
	if (width > 1)
		s->Get(this->spriteIdEndBottom)->Draw(xx, y + this->cellHeight * (this->height - 1));

	
	for (int i = 1; i < this->height - 1; i++) {
		xx = x;
		s->Get(this->spriteIdBeginMid)->Draw(xx, y + this->cellHeight * i);
		xx += this->cellWidth;
		for (int j = 1; j < this->width - 1; j++)
		{
			s->Get(this->spriteIdMiddleMid)->Draw(xx, y + this->cellHeight * i);
			xx += this->cellWidth;
		}
		if (width > 1)
			s->Get(this->spriteIdEndMid)->Draw(xx, y + this->cellHeight * i);
	}

	/*RenderBoundingBox();*/
}


void CRectangle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;		
	t = y - this->cellHeight / 2;	
	r = l + this->cellWidth * this->width;
	b = t + this->cellHeight;
	
}

void CRectangle::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

int CRectangle::IsDirectionColliable(float nx, float ny)
{	
	/*if (nx == 0 && ny == -1) return 1;
	else return 0;*/
	return 0;
}

//pipe

void CPipe::Render()
{
	if (this->length <= 0) return;
	float xx = x;
	float yy;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBeginTop)->Draw(x, y);
	xx += this->cellWidth;		
	s->Get(this->spriteIdEndTop)->Draw(xx, y);

	for (int i = 1; i < length; i++) {
		yy = y + this->cellHeight * i;
		s->Get(this->spriteIdBeginBody)->Draw(x, yy);
		s->Get(this->spriteIdEndBody)->Draw(xx, yy);
	}	
	

	/*RenderBoundingBox();*/
}

int CPipe::IsDirectionColliable(float nx, float ny)
{
	return 1;
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	
	r = l + this->cellWidth * 2 ;
	b = t + this->cellHeight * this->length;
}

void CPipe::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;
	float yy = y + this->cellHeight * this->length / 2 - this->cellWidth / 2;	
	CGame::GetInstance()->Draw(xx - cx,yy  - cy, 
		bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
	
}
