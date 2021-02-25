#pragma once
#include <string>
#include "SDL.h"
#include <SDL_image.h>
#include "Utilities.h"

//an abstract class used as a template for other game state classes
class GameState
{
protected:
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	std::string stateID;
public:
	virtual std::string update() = 0;
	virtual void render() = 0;
	virtual std::string handleEvents() = 0;
	virtual bool enter(SDL_Window* window, SDL_Renderer* renderer) = 0;
	virtual bool exit() = 0;
	virtual std::string getStateID() = 0;
};