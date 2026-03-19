#pragma once
#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

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

    void RegisterTask(std::function<void()> task, ePhase phase);
    void StartPhase(ePhase phase);
    void WaitPhase();
    void WorkerThreadUpdate();
    void WorkerLoop();
    void SyncLoop();

private:
    ePhase CurrentPhase = ePhase::None;
    const int WorkerCount = 4;
    
    std::mutex WorkerQueueMutex;
    std::mutex AsyncrQueueMutex;
    std::mutex WaitMutex;
    
    std::mutex WorkerConditionMutex;
    std::mutex AsyncConditionMutex;
    
    std::mutex WaitConditionMutex;
    
    std::atomic<int> WorkerActiveTasks ;
    std::atomic<int> DrawActiveTasks ;
    std::atomic<int> UpdateActiveTasks ;
    
    std::thread threads[4];
    
    std::condition_variable WorkerCondition;
    std::condition_variable AsyncCondiction;
    std::condition_variable waitCondition;

    
    std::queue<std::function<void()>> WorkerQueue;
    std::queue<std::function<void()>> UpdateQueue;
    std::queue<std::function<void()>> DrawQueue;
};
