#include "LButton.h"

//Player
int playerTurn = 0;

//Buttons objects
LButton gButtons[TOTAL_BUTTONS];

//Mouse button sprites
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
LTexture gButtonSpriteSheetTexture;

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e, char* table, int index)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < mPosition.x)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > mPosition.x + BUTTON_WIDTH)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < mPosition.y)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > mPosition.y + BUTTON_HEIGHT)
		{
			inside = false;
		}

		//Mouse is outside button
		if (table[index] == 'O')
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_WIN_O;
		}
		else if (table[index] == 'X')
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_WIN_X;
		}
		else if (table[index] == 'A')
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_LOSE;
		}
		//Mouse is outside button
		else if (!inside && table[index] == '-')
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;	
		}
		//Mouse is inside button
		else
		{
			if(table[index] == '-')
			{
				//Set mouse over sprite
				switch (e->type)
				{
				case SDL_MOUSEMOTION:
					if (playerTurn % 2) 
					{
						mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_X;
					}
					else
					{
						mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_O;
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					if (playerTurn % 2)
					{
						mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN_X;
						table[index] = 'x';
					}
					else
					{
						mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN_O;
						table[index] = 'o';
					}
					playerTurn++;
					break;

				/*
				case SDL_MOUSEBUTTONUP:
					mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
					break;
				*/
				}
			}
		}
	}
}

void LButton::render()
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}