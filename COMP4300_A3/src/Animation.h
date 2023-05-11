#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"

class Animation
{
private:
	sf::Sprite		m_sprite;
	int				m_frameCount		= 0;
	int             m_maxFrame			= 0;
	int				m_currentFrame      = 0;
	float           m_currentTime		= 0;
	int				m_speed				= 0;
	Vec2			m_size				= { 0, 0 };
	std::string		m_name				= "Default";
public:
	Animation();
	Animation(const std::string& animationName, std::shared_ptr<sf::Texture>, int frame, int speed);
	void update();
	bool hasEnded();
	const std::string& getName() const;
	const Vec2& getSize() const;
	sf::Sprite& getSprite();
};