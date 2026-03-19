#include "TaskMgr.h"

#include "Globals.h"
#include "Profiler.h"

void TaskMgr::Init()
{
    for (int i = 0; i<= WorkerCount; i++)
    {
        threads[i] = std::thread([this](){WorkerThreadUpdate();});
    }
}

void TaskMgr::Shut()
{
    WorkerCondition.notify_all();
    AsyncCondiction.notify_all();
    for (auto& element : threads)
        element.join();
    
    
}

void TaskMgr::RegisterTask(std::function<void()> task, ePhase phase)
{
    WorkerQueueMutex.lock();
    AsyncrQueueMutex.lock();
    switch (phase)
    {
        case ePhase::Worker:
            WorkerQueue.push(task);
            WorkerCondition.notify_one();
            WorkerActiveTasks++;
            break;
        case ePhase::Update:
            UpdateQueue.push(task);
            AsyncCondiction.notify_one();
            DrawActiveTasks++;
            break;
            
        case ePhase::Draw:
            DrawQueue.push(task);
            AsyncCondiction.notify_one();
            DrawActiveTasks++;
            break;

        case ePhase::None:
            break;
            
        default:
            break;
        }
    WorkerQueueMutex.unlock();
    AsyncrQueueMutex.unlock();
}

void TaskMgr::StartPhase(ePhase phase)
{
    CurrentPhase = phase;
    WorkerCondition.notify_all();
    WorkerActiveTasks = 0;
    AsyncCondiction.notify_all();
    DrawActiveTasks = 0;
}

void TaskMgr::WaitPhase()
{
    std::unique_lock lock(WorkerConditionMutex);
    waitCondition.wait(lock, [this](){return !WorkerQueue.empty();});
}

void TaskMgr::WorkerThreadUpdate()
{
    
}

void TaskMgr::WorkerLoop()
{
    while (true)
    {
        {
            std::unique_lock lock(WaitMutex);
            WorkerCondition.wait(lock, [this]()
            {
                if (CurrentPhase == ePhase::Draw)
                    return DrawActiveTasks > 0;

                if (CurrentPhase == ePhase::Update)
                    return UpdateActiveTasks > 0;

                return false;
            });
        }
    }
}

void TaskMgr::SyncLoop()
{
    while (true)
    {
        {
            std::unique_lock lock(AsyncConditionMutex);
            AsyncCondiction.wait(lock, [this](){return !WorkerQueue.empty();});
        }

        std::function<void()> current ; 
        {
            std::unique_lock lock(AsyncrQueueMutex);

            if (CurrentPhase == ePhase::Draw)
            {
                current =  DrawQueue.front() ;
                DrawQueue.pop();
            }
            else if (CurrentPhase == ePhase::Update)
            {
                current =  UpdateQueue.front();
                UpdateQueue.pop();
            }
            
            WorkerQueue.pop();
        }
        current();
        
        DrawActiveTasks --;
    }
}
