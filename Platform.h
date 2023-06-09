#pragma once

#include "GameObject.h"

// 
// The most popular type of object in Mario! 
// 
class CPlatform : public CGameObject
{
protected: 
	bool nx_collidable;
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;

public: 
	CPlatform(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end, int collidable) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->nx_collidable = (collidable == 1);
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsDirectionColliable(float nx, float ny);
};

//typedef CPlatform* LPPLATFORM;


class CRectangle : public CGameObject
{
protected:
	int height, width;		
	float cellWidth;
	float cellHeight;
	int spriteIdBeginTop, spriteIdMiddleTop, spriteIdEndTop;
	int spriteIdBeginMid, spriteIdMiddleMid, spriteIdEndMid;
	int spriteIdBeginBottom, spriteIdMiddleBottom, spriteIdEndBottom;

public:
	CRectangle(float x, float y,
		float cell_width, float cell_height, int height, int width,
		int sprite_id_begin_top, int sprite_id_middle_top, int sprite_id_end_top,
		int sprite_id_begin_mid, int sprite_id_middle_mid, int sprite_id_end_mid,
		int sprite_id_begin_bottom, int sprite_id_middle_bottom, int sprite_id_end_bottom) :CGameObject(x, y)
	{
		this->height = height;
		this->width = width;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		
		this->spriteIdBeginTop = sprite_id_begin_top;
		this->spriteIdMiddleTop = sprite_id_middle_top;
		this->spriteIdEndTop = sprite_id_end_top;

		this->spriteIdBeginMid = sprite_id_begin_mid;
		this->spriteIdMiddleMid = sprite_id_middle_mid;
		this->spriteIdEndMid = sprite_id_end_mid;

		this->spriteIdBeginBottom = sprite_id_begin_bottom;
		this->spriteIdMiddleBottom = sprite_id_middle_bottom;
		this->spriteIdEndBottom = sprite_id_end_bottom;		
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	int IsDirectionColliable(float nx, float ny);	
	int IsBlocking() { return 1; };
};

//typedef CRectangle* LPRECTANGLE;



// Pipe
class CPipe : public CPlatform
{
protected:	
	int spriteIdBeginTop, spriteIdEndTop, spriteIdBeginBody, spriteIdEndBody;

public:
	CPipe(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin_top, int sprite_id_end_top, 
		int sprite_id_begin_body, int sprite_id_end_body) :CPlatform(x, y,
			cell_width, cell_height, length, sprite_id_begin_top, sprite_id_end_top, sprite_id_begin_body, 1)
	{		
		this->spriteIdBeginTop = sprite_id_begin_top;		
		this->spriteIdEndTop = sprite_id_end_top;
		this->spriteIdBeginBody = sprite_id_begin_body;
		this->spriteIdEndBody = sprite_id_end_body;
	}

	void Render();
	void Update(DWORD dt) {}
	int IsDirectionColliable(float nx, float ny);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();
	virtual int IsBlocking() { return 1; };
};