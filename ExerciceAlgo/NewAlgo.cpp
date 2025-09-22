#include "NewAlgo.h"

unsigned NewAlgo::ListEntities(const sTeam& team, std::vector<sEntity>& entities, std::vector<sEntity*>& out)
{
    out.reserve(entities.size());
    for (auto& entity : entities)
        if (&entity.Team == &team)
            out.push_back(&entity);
    return out.size();
}
