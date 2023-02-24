#pragma once
#include "Entity.h"
#include <vector>
#include <memory>
#include <map>
#include <string>

class EntityManager
{
private:
	std::vector<std::shared_ptr<Entity>> m_entities;
	std::map<std::string, std::vector<std::shared_ptr<Entity>>> m_entitiesMap;
	std::vector<std::shared_ptr<Entity>> m_entitiesToAdd;
	size_t m_totalEntities = 0;
public:
	EntityManager();
	void update();
	void pushEntities();
	void removeEntities();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	std::vector<std::shared_ptr<Entity>>& getEntities();
	std::vector<std::shared_ptr<Entity>>& getEntities(const std::string& entityLabel);
};