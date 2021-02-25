#pragma once
#include "Sprites.h"
#include "GameState.h"

class Pause : public GameState
{
private:
	Button* m_pButton;
	SDL_Texture* m_pButtonTexture;
	std::string stateID = "PAUSE";
public:
	std::string update() override;
	void render() override;
	std::string handleEvents() override;
	bool enter(SDL_Window* window, SDL_Renderer* renderer) override;
	bool exit() override;
	std::string getStateID() override;
};