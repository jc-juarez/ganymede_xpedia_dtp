// *************************************
// Ganymede Xpedia
// Common
// 'gXThreadPool.hh'
// Author: jcjuarez
// *************************************

#ifndef GX_THREAD_POOL_
#define GX_THREAD_POOL_

#include <queue>
#include <mutex>
#include <atomic>
#include <vector>
#include <thread>
#include <future>
#include <memory>
#include <optional>
#include <functional>
#include "gXStatus.hh"
#include <condition_variable>

namespace gX
{

//
// Thread pool class for handling concurrent tasks through preallocated threads.
//
class ThreadPool
{

public:

    //
    // Constructor.
    //
    ThreadPool();

    //
    // Initializes the thread pool.
    //
    StatusCode
    Init(
        const uint16_t p_NumberThreads);

    //
    // Destructor. Ensures all threads are finished properly.
    //
    ~ThreadPool();

    //
    // Returns the number of threads used by the pool.
    //
    uint16_t
    GetNumberThreads() const;

    //
    // Returns the number of tasks currently in execution.
    //
    uint64_t
    GetNumberTasksInExecution() const;

    //
    //  Enqueues a task into the queue.
    //
    template<typename Function, typename... Args>
    std::optional<std::future<typename std::result_of<Function(Args...)>::type>>
    EnqueueTask(
        Function&& p_Function, Args&&... p_args)
    {
        using ReturnType = typename std::result_of<Function(Args...)>::type;

        //
        // Package destination function and arguments into a simple task wrapper pointer.
        //
        std::shared_ptr<std::packaged_task<ReturnType()>> packagedTask = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<Function>(p_Function), std::forward<Args>(p_args)...));
        
        std::future<ReturnType> packagedTaskResult = packagedTask->get_future();

        {
            std::unique_lock<std::mutex> lock(m_Lock);

            //
            // If thread pool is in destruction process fail the enqueue request.
            //
            if (m_Stop)
            {
                return std::nullopt;
            }

            m_Tasks.emplace(
                [packagedTask]()
                {
                    (*packagedTask)();
                });
        }

        //
        // Notify the task handler of a new task to be executed.
        //
        m_Condition.notify_one();
        return std::make_optional<std::future<ReturnType>>(std::move(packagedTaskResult));
    }

private:

    //
    // Handles and executes tasks from the queue.
    //
    void
    TaskHandler();

    //
    // Worker threads to execute tasks in the pool.
    //
    std::vector<std::thread> m_WorkerThreads;

    //
    // Queue of packaged tasks to be executed.
    //
    std::queue<std::function<void()>> m_Tasks;
    
    //
    // Exclusive lock for synchronizing access to the tasks queue.
    //
    std::mutex m_Lock;

    //
    // Condition for awakening worker threads.
    //
    std::condition_variable m_Condition;

    //
    // Number of threads to be spawned for the pool.
    //
    uint16_t m_NumberThreads;

    //
    // Flag for stopping worker threads.
    //
    bool m_Stop;

    //
    // Number of tasks currently in execution.
    //
    std::atomic<uint64_t> m_NumberTasksInExecution;
    
};

} // namespace gX.

#endif