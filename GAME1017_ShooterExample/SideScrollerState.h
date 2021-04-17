#pragma once
#ifndef _SIDESCROLLERSTATE_
#define _SIDESCROLLERSTATE_
#include "Sprites.h"
#include "GameState.h"

class SideScrollerState : public GameState
{
private:
	const Uint8* m_iKeystates; // Keyboard state container.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.

	vector<Background*> m_backgrounds;
	SDL_Texture* m_backgroundTexture;

	std::string stateID = "SIDE SCROLLER";
public:
	SideScrollerState();
	std::string update();
	void render();
	std::string handleEvents();
	std::string checkCollision();
	bool enter(SDL_Window* window, SDL_Renderer* renderer);
	bool exit();
	std::string getStateID();
};

#endif // !_SIDESCROLLERSTATE_