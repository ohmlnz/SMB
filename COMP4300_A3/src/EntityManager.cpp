#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager() {};

void EntityManager::update()
{
	pushEntities();
	removeEntities();
};

void EntityManager::pushEntities()
{
	for (auto& entity : m_entitiesToAdd)
	{
		m_entities.push_back(entity);
		m_entitiesMap[entity->tag()].push_back(entity);
	}

	m_entitiesToAdd.clear();
};

void EntityManager::removeEntities()
{
	m_entities.erase
	(
		std::remove_if
		(
			m_entities.begin(),
			m_entities.end(),
			[](const std::shared_ptr<Entity>& entity) { return !entity->isActive(); }
		),
		m_entities.end()
	);
	
	for (auto const& key : m_entitiesMap)
	{
		m_entitiesMap[key.first].erase
		(
			std::remove_if
			(
				m_entitiesMap[key.first].begin(),
				m_entitiesMap[key.first].end(),
				[](const std::shared_ptr<Entity>& entity) { return !entity->isActive(); }
			),
			m_entitiesMap[key.first].end()
		);
	}
};

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entitiesToAdd.push_back(entity);
	return entity;
};

std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities()
{
	return m_entities;
};

std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities(const std::string& entityLabel)
{
	return m_entitiesMap[entityLabel];
};