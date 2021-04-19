#pragma once
#ifndef _SIDESCROLLERSTATE_
#define _SIDESCROLLERSTATE_
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#include "Sprites.h"
#include "GameState.h"

class SideScrollerState : public GameState
{
private:
	const Uint8* m_iKeystates; // Keyboard state container.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.

	int m_iBSpawn; // The obstacle spawn frame timer properties.
	const int m_iBSpawnMax = 120;
	bool m_bONull = false;

	vector<Background*> m_backgrounds;
	vector<Obstacle*> m_obstacles;

	SideScrollerPlayer* m_pPlayer;

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