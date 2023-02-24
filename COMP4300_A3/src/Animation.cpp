#include "Animation.h"

Animation::Animation() {};

Animation::Animation(const std::string& animationName, std::shared_ptr<sf::Texture> texture, int frame, int speed)
	: m_name(animationName)
	, m_currentFrame(frame)
	, m_speed(speed)
	, m_size(Vec2(texture->getSize().x, texture->getSize().y))
{
	m_sprite.setTexture(*texture);
}


void Animation::update()
{

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

const sf::Sprite& Animation::getSprite() const
{
	return m_sprite;
}