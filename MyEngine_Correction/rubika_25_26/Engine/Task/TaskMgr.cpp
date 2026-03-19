#include "TaskMgr.h"

#include <Engine/Globals.h>

#include <assert.h>

#include <Windows.h>

void TaskMgr::Init()
{
	MainThreadId = std::this_thread::get_id();
	UpdateActiveJobs = 0;
	DrawActiveJobs = 0;
	CurrentPhase = ePhase::None;

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

void TaskMgr::Shut()
{
	ConditionWorker.notify_all();
	ConditionWorkerSync.notify_all();
	FinishedConditionWorkerSync.notify_all();

	for (auto& t : Workers)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	Workers.clear();

	for (auto& t : WorkersSync)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
	WorkersSync.clear();
}

void TaskMgr::RegisterTask(std::function<void()> task, ePhase time)
{
	std::unique_lock<std::mutex> lock(QueueMutex);
	switch (time)
	{
	case TaskMgr::ePhase::Update:
		TasksToRunOnUpdate.push(task);
		++UpdateActiveJobs;
		if (CurrentPhase == TaskMgr::ePhase::Update)
		{
			ConditionWorkerSync.notify_one();
		}
		break;

	case TaskMgr::ePhase::Draw:
		TasksToRunOnDraw.push(task);
		++DrawActiveJobs;
		if (CurrentPhase == TaskMgr::ePhase::Draw)
		{
			ConditionWorkerSync.notify_one();
		}
		break;

	case TaskMgr::ePhase::Worker:
	{
		TasksToRunOnWorker.push(task);
		++WorkerActiveJobs;
		ConditionWorker.notify_one();
	}
	break;
	}

}

void TaskMgr::StartPhase(ePhase time)
{
	CurrentPhase = time;
	ConditionWorkerSync.notify_all();
}

void TaskMgr::WaitPhase()
{
	//std::unique_lock<std::mutex> lock(FinishedMutex);
	//FinishedConditionWorkerSync.wait(lock, [this]()
	//	{
	//		return WorkerSyncActiveJobs == 0 && !HasAvailableJob();
	//	});

	while (true)
	{

		if (CurrentPhase == ePhase::Update && UpdateActiveJobs == 0)
		{
			return;
		}

		if (CurrentPhase == ePhase::Draw && DrawActiveJobs == 0)
		{
			return;
		}

		//Sleep(1);
	}
}

void TaskMgr::ResetPhase()
{
	CurrentPhase = ePhase::None;
}

bool TaskMgr::HasAvailableJob()
{
	std::unique_lock<std::mutex> lock(QueueMutex);

	if (CurrentPhase == ePhase::Update && !TasksToRunOnUpdate.empty())
		return true;

	if (CurrentPhase == ePhase::Draw && !TasksToRunOnDraw.empty())
		return true;

	return false;
}

void TaskMgr::WorkerLoop()
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
			if (TasksToRunOnWorker.empty())
				continue;

			job = TasksToRunOnWorker.front();
			TasksToRunOnWorker.pop();
		}

		job();

		WorkerActiveJobs--;
	}
}

void TaskMgr::WorkerSyncLoop()
{
	while (true)
	{
		FinishedConditionWorkerSync.notify_all();

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

		ePhase p = CurrentPhase;
		{

			std::unique_lock<std::mutex> lock(QueueMutex);
			switch (p)
			{
			case TaskMgr::ePhase::Draw:
				if (TasksToRunOnDraw.empty())
					continue;
				job = TasksToRunOnDraw.front();
				TasksToRunOnDraw.pop();
				break;

			case TaskMgr::ePhase::Update:
				if (TasksToRunOnUpdate.empty())
					continue;
				job = TasksToRunOnUpdate.front();
				TasksToRunOnUpdate.pop();
				break;

			default:
				assert(false);
			}
		}

		job();

		if (p == ePhase::Update)
		{
			--UpdateActiveJobs;
		}
		if (p == ePhase::Draw)
		{
			--DrawActiveJobs;
		}

		FinishedConditionWorkerSync.notify_one();
	}
}