#include "SideScrollerState.h"

SideScrollerState::SideScrollerState()
{

}

std::string SideScrollerState::update()
{
	for (unsigned int i = 0; i < m_backgrounds.size(); i++)
	{
		m_backgrounds[i]->Update();
	}
	return "";
}

void SideScrollerState::render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Render stuff, background first
	for (unsigned int i = 0; i < m_backgrounds.size(); i++)
	{
		m_backgrounds[i]->Render(m_pRenderer);
	}
}

std::string SideScrollerState::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
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

std::string SideScrollerState::checkCollision()
{
	return "";
}

bool SideScrollerState::enter(SDL_Window* window, SDL_Renderer* renderer)
{
	m_pWindow = window;
	m_pRenderer = renderer;

	m_backgroundTexture = IMG_LoadTexture(m_pRenderer, "Img/BG.png");

	m_backgrounds.reserve(10);
	m_backgrounds.push_back(new Background({ 0,0,1024,768 }, { 0,0,1024,768 }, 1));
	m_backgrounds.push_back(new Background({ 0,0,1024,768 }, { 1024,0,1024,768 }, 1));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 0,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 256,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 512,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 768,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,0,256,512 }, { 1024,0,256,512 }, 3));
	m_backgrounds.push_back(new Background({ 1024,512,512,256 }, { 0,512,512,256 }, 4));
	m_backgrounds.push_back(new Background({ 1024,512,512,256 }, { 512,512,512,256 }, 4));
	m_backgrounds.push_back(new Background({ 1024,512,512,256 }, { 1024,512,512,256 }, 4));


	return true;
}

bool SideScrollerState::exit()
{
	for (unsigned int i = 0; i < m_backgrounds.size(); i++)
	{
		delete m_backgrounds[i];
		m_backgrounds[i] = nullptr;
	}
	m_backgrounds.clear();
	m_backgrounds.shrink_to_fit();

	return true;
}

std::string SideScrollerState::getStateID()
{
	return stateID;
}
