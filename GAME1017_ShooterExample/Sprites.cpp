#include <algorithm> // For min/max.
#include <cmath>	 // For cos/sin.
#include "Sprites.h"
#include "Engine.h"
using namespace std;

Sprite::Sprite(){}
Sprite::Sprite(SDL_Rect s, SDL_Rect d) :m_rSrc(s), m_rDst(d) {}
SDL_Rect* Sprite::GetSrcP() { return &m_rSrc; }
SDL_Rect* Sprite::GetDstP() { return &m_rDst; }
double Sprite::GetAngle() { return m_angle; }

AnimatedSprite::AnimatedSprite(int a, int fm, int sm, SDL_Rect s, SDL_Rect d): 
	Sprite(s, d), m_iFrameMax(fm), m_iSpriteMax(sm)
{
	// Added this here (and not in header line) just because. Note: initializers ARE resolved before body statements.
	m_angle = a; 
	m_iSprite = m_iFrame = 0; // Chaining assignments to the same value.
}

void AnimatedSprite::Animate()
{
	m_iFrame++;
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
		{
			m_iSprite = 0;
		}
	}
	m_rSrc.x = m_rSrc.w * m_iSprite;
}

Player::Player(SDL_Rect s, SDL_Rect d) : /*Animated*/Sprite(s, d) {}

Bullet::Bullet(SDL_Rect s, SDL_Rect d, int spd) :
	Sprite(s, d), m_speed(spd), m_active(true) {}

void Bullet::Update()
{
	m_rDst.x += m_speed;
}

Enemy::Enemy(SDL_Rect s, SDL_Rect d, vector<Bullet*>* bVec, Mix_Chunk* c, int fr): /*Animated*/Sprite(s, d),
	m_pBulletVec(bVec), m_pPew(c), m_bulletTimer(0), m_timerMax(fr) {}

void Enemy::Update()
{
	//Animate();
	m_rDst.x -= 3;
	if (m_bulletTimer++ == m_timerMax)
	{
		m_bulletTimer = 0;
		m_pBulletVec->push_back(new Bullet( {843,977,13,37}, {m_rDst.x,m_rDst.y-28,13,37}, -10 ));
		Mix_PlayChannel(-1, m_pPew, 0);
	}
}

Button::Button(SDL_Rect s, SDL_Rect d)
{
	pressed = false;
	m_rSrc = s;
	m_rDst = d;
}

bool Button::handleEvent(SDL_Event* e)
{
	pressed = false;
	if (e->type == SDL_MOUSEBUTTONDOWN)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		pressed = true;

		//Mouse is left of the button
		if (x < m_rDst.x)
		{
			pressed = false;
		}
		//Mouse is right of the button
		else if (x > m_rDst.x + m_rDst.w)
		{
			pressed = false;
		}
		//Mouse above the button
		else if (y < m_rDst.y)
		{
			pressed = false;
		}
		//Mouse below the button
		else if (y > m_rDst.y + m_rDst.h)
		{
			pressed = false;
		}
		//else mouse is outside button
	}
	return pressed;
}
