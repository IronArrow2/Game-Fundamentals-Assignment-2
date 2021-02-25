#pragma once
#include "GameState.h"
#include "Sprites.h"

class TitleState : public GameState
{
private:
	SDL_Texture* m_pButtonTexture;
	SDL_Texture* m_pBackgroundTexture;
	Sprite* m_pBackground;
	Button* m_pButton;

	std::string stateID = "TITLE";
public:
	std::string update() override;
	void render() override;
	std::string handleEvents() override;
	bool enter(SDL_Window* window, SDL_Renderer* renderer) override;
	bool exit() override;
	std::string getStateID() override;
};