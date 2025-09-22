#pragma once
#include "Algo.h"

class NewAlgo : public Algo
{
public:
    explicit NewAlgo(const std::string& name)
        : Algo(name)
    {
    }

private:
    unsigned ListEntities(const sTeam& team, std::vector<sEntity>& entities, std::vector<sEntity*>& out) override;
};
