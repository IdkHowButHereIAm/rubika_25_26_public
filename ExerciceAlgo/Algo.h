#pragma once

#include "Shared.h"

#include <vector>

struct sTimeCompare
{
	sTimeCompare() : NbElement(0), TimeAllRetry(0), TimeAvg(0)
	{}

	unsigned NbElement;
	double TimeAllRetry;
	double TimeAvg;
};

class Algo
{

public:
	Algo(const std::string& name);

	void StartComputation(unsigned nbElementInList);
	void NewTestDuringComputation(const sTeam& team, std::vector<sEntity>& entities);
	void EndComputation(unsigned nbRetry);
	void PrintResult() const;

private:
	virtual unsigned ListEntities(const sTeam& team, std::vector<sEntity>& entities, std::vector<sEntity*>& out) = 0;
	
	std::string Name;
	std::vector<sTimeCompare> ExecutionTimes;
};