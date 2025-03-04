#include "GameEngine.h"

GameEngine::GameEngine(const std::string& assetsPath)
{
    init(assetsPath);
}

void GameEngine::init(const std::string& assetsPath)
{
	loadAssets(assetsPath);
    m_window.create(sf::VideoMode(1920, 1080), "Super Mario Bruster");
    m_window.setFramerateLimit(60);
	m_view.reset(sf::FloatRect(0,0, 1920, 1080));
	m_window.setView(m_view);
	changeScene("Menu", std::make_shared<SceneMenu>(this));
}

void GameEngine::process()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
		if (event.type == sf::Event::Closed)
		{
			quit();
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			if (currentScene()->getActionMap().find(event.key.code) != currentScene()->getActionMap().end())
			{
				const std::string actionType = event.type == sf::Event::KeyPressed ? "START" : "END";
				currentScene()->process(Action(currentScene()->getActionMap().at(event.key.code), actionType));
			}
		}
    }
}

void GameEngine::update()
{
    currentScene()->update();
}

void GameEngine::render()
{
    m_window.clear(sf::Color(92, 148, 252));
    currentScene()->render();
    m_window.display();
}

void GameEngine::run()
{
    while (!m_isRunning)
    {
        process();
        update();
        render();
    }
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_scenes[m_currentScene];
}

sf::RenderWindow& GameEngine::currentWindow()
{
    return m_window;
}

sf::View& GameEngine::currentView()
{
	return m_view;
}

void GameEngine::loadAssets(const std::string& assetsPath)
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
			m_assets.addTexture(textureName, filePath);
		}
		else if (category == "Animation")
		{
			inFile >> animationName >> textureName >> frame >> speed;
			m_assets.addAnimation(animationName, textureName, frame, speed);
		}
		else if (category == "Font")
		{
			inFile >> fontName >> filePath;
			m_assets.addFont(fontName, filePath);
		}
	}
}

Assets& GameEngine::assets()
{
	return m_assets;
}

void GameEngine::changeScene(const std::string& name, std::shared_ptr<Scene> scene)
{
	m_scenes[name] = scene;
	m_currentScene = name;
}

void GameEngine::quit()
{
	m_isRunning = false;
	m_window.close();
}