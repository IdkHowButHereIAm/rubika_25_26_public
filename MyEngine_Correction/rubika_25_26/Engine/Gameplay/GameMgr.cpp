#include "GameMgr.h"

#include <Engine/Gameplay/Entity/Entity.h>
#include <Engine/Task/TaskMgr.h>
#include <Engine/Globals.h>
#include <Engine/Profiler.h>

GameMgr::GameMgr()
{}

GameMgr::~GameMgr()
{
	for (Entity* e : Entities)
	{
		e->Destroy();
		delete e;
	}

	Entities.clear();
}

#define UPDATE_ON_THREAD

void GameMgr::Update(float deltaTime)
{
	for (Entity* e : Entities)
	{
#ifdef UPDATE_ON_THREAD
		gData.TaskMgr->RegisterTask([e, deltaTime]()
			{
#endif
				PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Update %d", e);

				e->Update(deltaTime);

				PROFILER_EVENT_END();

#ifdef UPDATE_ON_THREAD

			}, TaskMgr::ePhase::Update);
#endif
	}
}

void GameMgr::Draw(sf::RenderWindow& window)
{
	for (Entity* e : Entities)
	{
		e->Draw(window);
	}
}

void GameMgr::AddEntity(Entity* entity)
{
	if (!entity)
	{
		return;
	}

	Entities.push_back(entity);
	entity->Start();
}
