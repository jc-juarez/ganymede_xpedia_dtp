// *************************************
// Ganymede Xpedia
// Common
// 'gXThreadPool.cc'
// Author: jcjuarez
// *************************************

#include "gXThreadPool.hh"
#include <iostream> // delete.

namespace gX
{

ThreadPool::ThreadPool()
    : m_Stop(false)
{}

StatusCode
ThreadPool::Init(
    const uint16_t p_NumberThreads)
{
    m_NumberThreads = p_NumberThreads;

    //
    // Spawn threads for the thread pool.
    //
    try
    {
        for (uint16_t threadIndex = 0; threadIndex < m_NumberThreads; ++threadIndex)
        {
            m_WorkerThreads.emplace_back(&ThreadPool::TaskHandler, this);

            if (!m_WorkerThreads.back().joinable())
            {
                return Status::ThreadLaunchFailed;
            }
        }
    }
    catch (const std::system_error& exception)
    {
        return Status::ThreadLaunchFailed;
    }

    return Status::Success;
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(m_Lock);
        m_Stop = true;
    }

    //
    // Awake all threads and finish them.
    //
    m_Condition.notify_all();

    for (std::thread& worker_thread : m_WorkerThreads)
    {
        worker_thread.join();
    }
}

uint16_t
ThreadPool::GetNumberThreads() const
{
    return m_NumberThreads;
}

uint64_t
ThreadPool::GetNumberTasksInExecution() const
{
    return m_NumberTasksInExecution;
}

void
ThreadPool::TaskHandler()
{
    FOREVER
    {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(m_Lock);

            m_Condition.wait(lock,
                [this]
                {
                    return this->m_Stop ||
                        !this->m_Tasks.empty();
                });

            //
            // If the destructor has been invoked, wait for finishing all pending 
            // tasks and then terminate the invoked thread.
            //
            if (m_Stop && m_Tasks.empty())
            {
                return;
            }

            //
            //  Retrieve and move task from the queue for non-blocking execution.
            //
            task = std::move(m_Tasks.front());
            m_Tasks.pop();
        }

        ++m_NumberTasksInExecution;
        task();
        --m_NumberTasksInExecution;
    }      
}

} // namespace gX.