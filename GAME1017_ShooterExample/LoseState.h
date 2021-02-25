#pragma once
#include "Sprites.h"
#include "GameState.h"

class LoseState : public GameState
{
private:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	SDL_Texture* m_pButtonTexture;
	Button *m_pButton1, *m_pButton2;

	std::string stateID = "LOSE";
public:
	LoseState();
	std::string update();
	void render();
	std::string handleEvents();
	bool enter(SDL_Window* window, SDL_Renderer* renderer);
	bool exit();
	std::string getStateID();
};