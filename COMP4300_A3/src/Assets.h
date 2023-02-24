#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <map>

#include "Animation.h"

class Assets
{
private:
	std::map<const std::string, std::shared_ptr<sf::Texture>>	m_textures;
	std::map<const std::string, Animation> m_animations;
	std::map<const std::string, sf::Sound> m_sounds;
	std::map<const std::string, sf::Font> m_fonts;
									 
public:
	Assets();
	std::shared_ptr<sf::Texture> getTexture(const std::string& textureName);
	Animation& getAnimation(const std::string& animationName);
	sf::Sound& getSound(const std::string& soundName);
	sf::Font& getFont(const std::string& fontName);

	void addTexture(const std::string& textureName, const std::string& filePath);
	void addAnimation(const std::string& animationName, const std::string& textureName, int frame, int speed);
	void addSound(const std::string& soundName, const std::string& filePath);
	void addFont(const std::string& fontName, const std::string& filePath);

	void loadAssets(const std::string& assetsPath);
};