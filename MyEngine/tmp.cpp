#include "TaskMgr_Clean.h"

#include "Globals.h"
#include <assert.h>

void TaskMgr_Clean::Init()
{
	MainThreadId = std::this_thread::get_id();
	CurrentPhase = eTemporality::None;

	const int nThread = 4;

	Workers.reserve(nThread);
	for (int i = 0; i < nThread; ++i)
	{
        Workers.emplace_back([this]()
        {
            WorkerLoop();
        });
	}

	WorkersSync.reserve(nThread);
	for (int i = 0; i < nThread; ++i)
	{
        WorkersSync.emplace_back([this]()
        {
            WorkerSyncLoop();
        });
	}
}

void TaskMgr_Clean::RegisterTask(std::function<void()> task, eTemporality time)
{
    std::unique_lock<std::mutex> lock(QueueMutex);
	switch (time)
	{
		case TaskMgr_Clean::eTemporality::Update:
			TasksToRunOnUpdate.push(task);
			if (CurrentPhase == TaskMgr_Clean::eTemporality::Update)
			{
				ConditionWorkerSync.notify_one();
			}
			break;

		case TaskMgr_Clean::eTemporality::Draw:
			TasksToRunOnDraw.push(task);
			if (CurrentPhase == TaskMgr_Clean::eTemporality::Draw)
			{
				ConditionWorkerSync.notify_one();
			}
			break;

		case TaskMgr_Clean::eTemporality::Worker:
		{
			TasksToRunOnWorker.push(task);
			++WorkerActiveJobs;
			ConditionWorker.notify_one();
		}
			break;
	}

}

void TaskMgr_Clean::Start(eTemporality time)
{
	CurrentPhase = time;
	ConditionWorkerSync.notify_all();
}

void TaskMgr_Clean::Wait()
{
    std::unique_lock<std::mutex> lock(Mutex);
    FinishedConditionWorkerSync.wait(lock, [this]()
    {
        return WorkerSyncActiveJobs == 0 && !HasAvailableJob();
    });
}

bool TaskMgr_Clean::HasAvailableJob()
{
    std::unique_lock<std::mutex> lock(QueueMutex);
    
	if (CurrentPhase == eTemporality::Update && !TasksToRunOnUpdate.empty())
        return true;

    if (CurrentPhase == eTemporality::Draw && !TasksToRunOnDraw.empty())
        return true;

    return false;
}

void TaskMgr_Clean::WorkerLoop()
{
	while (true)
    {
        std::function<void()> job;

		{
			std::unique_lock<std::mutex> lock(Mutex);

			ConditionWorker.wait(lock, [this]()
				{
					return gData.ExitApp || WorkerActiveJobs != 0;
				});

            if (gData.ExitApp)
                return;
		}

		{

			std::unique_lock<std::mutex> lock(QueueMutex);

			job = TasksToRunOnWorker.front();
			TasksToRunOnWorker.pop();
        }

        job();

		WorkerActiveJobs--;
    }
}

void TaskMgr_Clean::WorkerSyncLoop()
{
	while (true)
    {

		{
			std::unique_lock<std::mutex> lock(Mutex);

			ConditionWorkerSync.wait(lock, [this]()
				{
					return gData.ExitApp || HasAvailableJob();
				});

			if (gData.ExitApp)
				return;
		}

        std::function<void()> job;

		{
			std::unique_lock<std::mutex> lock(QueueMutex);
			switch (CurrentPhase)
			{
				case TaskMgr_Clean::eTemporality::Draw:
					job = TasksToRunOnDraw.front();
					TasksToRunOnDraw.pop();
					break;

				case TaskMgr_Clean::eTemporality::Update:
					job = TasksToRunOnUpdate.front();
					TasksToRunOnUpdate.pop();
					break;

				default:
					assert(false);
			}
        }

		WorkerSyncActiveJobs++;

        job();

        WorkerSyncActiveJobs--;

		FinishedConditionWorkerSync.notify_one();
    }
}