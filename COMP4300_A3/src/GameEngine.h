#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "Assets.h"
#include "Action.h"
#include "Scene.h"
#include "SceneMenu.h"
#include "SceneLevelOne.h"

class GameEngine
{
private:
    sf::RenderWindow                                m_window;
    bool                                            m_isRunning;
    Assets                                          m_assets;
    std::string                                     m_currentScene = "Menu";
    std::map<std::string, std::shared_ptr<Scene>>   m_scenes;

public:
    GameEngine(const std::string& assetsPath);
    void init(const std::string& assetsPath);
    void run();
    void quit();

    // main loop
    void process();
    void update();
    void render();

    // game system
    std::shared_ptr<Scene> currentScene();
    sf::RenderWindow& currentWindow();
    Assets& assets();
    void loadAssets(const std::string& assetsPath);
    void changeScene(const std::string& name, std::shared_ptr<Scene>);
};