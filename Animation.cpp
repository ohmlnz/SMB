#include "Animation.h"
#include <iostream>

Animation::Animation() {};

Animation::Animation(const std::string& animationName, std::shared_ptr<sf::Texture> texture, int frame, int speed)
	: m_name(animationName)
	, m_maxFrame(frame)
	, m_speed(speed)
	, m_size(Vec2(texture->getSize().x, texture->getSize().y))
{
	m_sprite.setTexture(*texture);
	m_sprite.setOrigin((m_size.x / m_maxFrame) / 2, m_size.y / 2);
}

void Animation::update()
{
	if (m_speed > 0)
	{
		m_currentTime++;

		if (m_currentTime >= m_speed)
		{
			m_currentFrame = (m_currentFrame + 1) % m_maxFrame;
			m_currentTime = 0;
		}

		m_sprite.setTextureRect(sf::IntRect(m_currentFrame * (m_size.x / m_maxFrame), 0, m_size.y, m_size.y));
	}
}

bool Animation::hasEnded()
{
	return false;
}

const std::string& Animation::getName() const
{
	return m_name;
}

const Vec2& Animation::getSize() const
{
	return m_size;
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}