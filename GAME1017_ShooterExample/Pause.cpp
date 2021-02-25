#include "Pause.h"

std::string Pause::update()
{
	return "";
}

void Pause::render()
{
	SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 128);
	SDL_RenderFillRect(m_pRenderer, NULL);

	SDL_RenderCopy(m_pRenderer, m_pButtonTexture, m_pButton->GetSrcP(), m_pButton->GetDstP());
}

std::string Pause::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		bool buttonClicked = m_pButton->handleEvent(&event);
		if (buttonClicked)
		{
			cout << "Button clicked!";
			return "POP";
		}
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			return "QUIT";
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return "QUIT";
			else if (event.key.keysym.sym == SDLK_p)
			{
				return "POP";
			}
			break;
		}
	}
	return "";
}

bool Pause::enter(SDL_Window* window, SDL_Renderer* renderer)
{
	m_pWindow = window;
	m_pRenderer = renderer;

	m_pButtonTexture = IMG_LoadTexture(m_pRenderer, "Img/ClickMe.png");
	m_pButton = new Button({ 0,0,256,128 }, { 338,205,256,128 });

	return true;
}

bool Pause::exit()
{
	m_pWindow = nullptr;
	m_pRenderer = nullptr;
	return true;
}

std::string Pause::getStateID()
{
	return stateID;
}
