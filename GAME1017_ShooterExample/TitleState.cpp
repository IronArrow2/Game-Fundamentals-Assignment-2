#include "TitleState.h"

std::string TitleState::update()
{//updates all the items in titlescreen
	return "";
}

void TitleState::render()
{//renders all the items in titlescreen
	SDL_RenderCopy(m_pRenderer, m_pBackgroundTexture, m_pBackground->GetSrcP(), m_pBackground->GetDstP());
	SDL_RenderCopy(m_pRenderer, m_pButtonTexture, m_pButton->GetSrcP(), m_pButton->GetDstP());
}

std::string TitleState::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		bool buttonClicked = m_pButton->handleEvent(&event);
		if (buttonClicked)
		{
			cout << "Button clicked!";
			return "SIDE SCROLLER";
			//return "MAIN";
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
				return "PAUSE";
			}
			break;
		}
	}
	return "";
}

bool TitleState::enter(SDL_Window* window, SDL_Renderer* renderer)
{//loads all the items in titlescreen
	m_pWindow = window;
	m_pRenderer = renderer;
	m_pButtonTexture = IMG_LoadTexture(m_pRenderer, "Img/ClickMe.png");
	m_pBackgroundTexture = IMG_LoadTexture(m_pRenderer, "Img/DiogenesTheCynicTheGame.png");

	m_pBackground = new Sprite({ 0,0,1024,768 }, { 0, 0, 1024, 768 });
	m_pButton = new Button({ 0,0,256,128 }, { 256,192,256,128 });

	return true;
}

bool TitleState::exit()
{//unloads all the items in titlescreen
	m_pWindow = nullptr;
	m_pRenderer = nullptr;
	return true;
}

std::string TitleState::getStateID()
{
	return stateID;
}
