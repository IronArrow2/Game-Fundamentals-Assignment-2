#include "LoseState.h"

LoseState::LoseState()
{

}

std::string LoseState::update()
{
	return "";
}

void LoseState::render()
{
	SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
	SDL_RenderFillRect(m_pRenderer, NULL);

	SDL_RenderCopy(m_pRenderer, m_pButtonTexture, m_pButton1->GetSrcP(), m_pButton1->GetDstP());
	SDL_RenderCopy(m_pRenderer, m_pButtonTexture, m_pButton2->GetSrcP(), m_pButton2->GetDstP());
}

std::string LoseState::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		bool button1Clicked = m_pButton1->handleEvent(&event);
		if (button1Clicked)
		{
			cout << "Button 1 clicked!";
			return "TITLE";
		}
		bool button2Clicked = m_pButton2->handleEvent(&event);
		if (button2Clicked)
		{
			cout << "Button 2 clicked!";
			return "MAIN";
		}
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			return "QUIT";
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return "QUIT";
			break;
		}
	}
	return "";
}

bool LoseState::enter(SDL_Window* window, SDL_Renderer* renderer)
{
	m_pWindow = window;
	m_pRenderer = renderer;

	m_pButtonTexture = IMG_LoadTexture(m_pRenderer, "Img/ClickMe.png");
	m_pButton1 = new Button({ 0,0,256,128 }, { 256,192,256,128 });
	m_pButton2 = new Button({ 0,0,256,128 }, {256,480,256,128 });

	return true;
}

bool LoseState::exit()
{
	m_pWindow = nullptr;
	m_pRenderer = nullptr;

	return true;
}

std::string LoseState::getStateID()
{
	return stateID;
}
