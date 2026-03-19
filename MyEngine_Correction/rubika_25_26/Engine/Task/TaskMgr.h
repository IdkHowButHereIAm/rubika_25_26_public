#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <shared_mutex>

class TaskMgr
{
public:
	void Init();
	void Shut();

	enum class ePhase
	{
		None,
		Worker,
		Update,
		Draw,
	};

	void RegisterTask(std::function<void()> task, ePhase time);
	void StartPhase(ePhase time);
	void WaitPhase();
	void ResetPhase();

private:

	void WorkerLoop();
	void WorkerSyncLoop();

	bool HasAvailableJob();

	std::thread::id MainThreadId;

	std::vector<std::thread> Workers;
	std::condition_variable ConditionWorker;
	std::atomic<unsigned> WorkerActiveJobs;

	std::vector<std::thread> WorkersSync;
	std::condition_variable ConditionWorkerSync;
	std::condition_variable FinishedConditionWorkerSync;
	std::atomic<unsigned> UpdateActiveJobs;
	std::atomic<unsigned> DrawActiveJobs;

	std::mutex Mutex;
	std::mutex FinishedMutex;
	std::mutex QueueMutex;

	std::queue<std::function<void()>> TasksToRunOnWorker;
	std::queue<std::function<void()>> TasksToRunOnUpdate;
	std::queue<std::function<void()>> TasksToRunOnDraw;

	std::atomic<ePhase> CurrentPhase;

	int NbThread = 4;
	int NbWorkers = 4;
};
