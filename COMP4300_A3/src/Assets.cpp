#include "Assets.h"

Assets::Assets() {}

std::shared_ptr<sf::Texture> Assets::getTexture(const std::string& textureName)
{
	if (m_textures.contains(textureName))
	{
		return m_textures[textureName];
	}

	std::cerr << "The texture " << textureName << "can't be found!" << std::endl;
	std::cerr << "Returning default instead...";
	return m_textures["default"];
};

Animation& Assets::getAnimation(const std::string& animationName)
{
	if (m_animations.contains(animationName))
	{
		return m_animations[animationName];
	}

	std::cerr << "The animation " << animationName << " can't be found!" << std::endl;
	std::cerr << "Returning default instead...";
	return m_animations["default"];
}

sf::Sound& Assets::getSound(const std::string& soundName)
{
	if (m_sounds.contains(soundName))
	{
		return m_sounds[soundName];
	}

	std::cerr << "The sound " << soundName << "can't be found!" << std::endl;
	std::cerr << "Returning default instead...";
	return m_sounds["default"];
}

sf::Font& Assets::getFont(const std::string& fontName)
{
	if (m_fonts.contains(fontName))
	{
		return m_fonts[fontName];
	}

	std::cerr << "The font " << fontName << "can't be found!" << std::endl;
	std::cerr << "Returning default instead...";
	return m_fonts["default"];
};

void Assets::addTexture(const std::string& textureName, const std::string& filePath)
{
	std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

	if (!texture->loadFromFile(filePath))
	{
		exit(1);
	}

	m_textures[textureName] = texture;
	std::cout << textureName << " has been properly loaded." << std::endl;
};

void Assets::addAnimation(const std::string& animationName, const std::string& textureName, int frame, int speed)
{
	auto texture = getTexture(textureName);
	Animation animation(animationName, texture, frame, speed);
	m_animations[animationName] = animation;
};

void Assets::addSound(const std::string& soundName, const std::string& filePath)
{
	sf::SoundBuffer buffer;

	if (!buffer.loadFromFile(filePath))
	{
		exit(1);
	}
	sf::Sound sound;
	sound.setBuffer(buffer);
	m_sounds[soundName] = sound;
	std::cout << soundName << " has been properly loaded." << std::endl;
};

void Assets::addFont(const std::string& fontName, const std::string& filePath)
{
	sf::Font font;

	if (!font.loadFromFile(filePath))
	{
		exit(1);
	}

	m_fonts[fontName] = font;
	std::cout << fontName << " has been properly loaded." << std::endl;
};

void Assets::loadAssets(const std::string& assetsPath)
{
	std::ifstream inFile(assetsPath);
	std::string category, animationName, textureName, fontName, filePath;
	int frame, speed;

	if (!inFile)
	{
		std::cerr << "The assets file could not be opened at path: " << assetsPath << std::endl;
	}

	while (inFile >> category)
	{
		if (category == "Texture")
		{
			inFile >> textureName >> filePath;
			addTexture(textureName, filePath);
		}
		else if (category == "Animation")
		{
			inFile >> animationName >> textureName >> frame >> speed;
			addAnimation(animationName, textureName, frame, speed);
		}
		else if (category == "Font")
		{
			inFile >> fontName >> filePath;
			addFont(fontName, filePath);
		}
	}
}