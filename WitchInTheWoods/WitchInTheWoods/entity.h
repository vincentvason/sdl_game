#pragma once
#include "game.h"

class Entity
{
public:
	virtual void update() {};

protected:
	int LocXToPosX(float locX) { return (int)(16.0 + (locX * 32.0)); };
	int LocYToPosY(float locY) { return (int)(32.0 + (locY * 32.0)); };

	bool isActive = false;

	static const int SPRITE_WIDTH = 32;
	static const int SPRITE_HEIGHT = 32;

	int mLocX = 0, mLocY = 0;
	int mVelX = 0, mVelY = 0;

	SDL_Rect mCollider;
	LTexture* mSprite;
};

class Object : public Entity
{
public:
	virtual void update() {};

protected:
	bool isActive = false;

	float mLocX = 0, mLocY = 0;
	float mVelX = 0, mVelY = 0;

	SDL_Rect mCollider;
	LTexture* mSprite;
};

class Actor : public Entity
{
public:
	virtual void handleEvent(SDL_Event* e) {};
	virtual void update() {};

protected:
	bool isActive = false;

	float mLocX = 0, mLocY = 0;
	float mVelX = 0, mVelY = 0;

	SDL_Rect mCollider;
	LTexture* mSprite;
};