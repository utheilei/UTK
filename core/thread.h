#ifndef THREAD_H
#define THREAD_H

#include "utkcore_export.h"

#include <future>
#include <functional>

#include <QThread>

template <class T>
class UTKCORE_EXPORT QThreadCreateThread : public QThread
{
public:
    explicit QThreadCreateThread(std::future<T> &&future, std::function<void()> &&callback)
        : m_future(std::move(future)), m_callback(std::move(callback))
    {
    }

    inline T getResult()
    {
        return m_result;
    }

private:
    void run() override
    {
        m_result = m_future.get();
        if (m_callback)
        {
            m_callback();
        }
    }

    std::function<void()> m_callback;
    std::future<T> m_future;
    T m_result;
};

template <class T>
class UTKCORE_EXPORT QThreadCreateTaskThread : public QThread
{
public:
    explicit QThreadCreateTaskThread(std::packaged_task<T()> &&task)
        : m_task(std::move(task))
    {
    }

    inline T getResult()
    {
        return m_result;
    }

private:
    void run() override
    {
        auto future = m_task.get_future();
        m_task();
        m_result = future.get();
    }

    std::packaged_task<T()> m_task;
    T m_result;
};

class Thread
{
public:
    Thread();
    ~Thread();

    template <typename R, typename Function, typename... Args, typename FunctionCallback>
    static QThread* create(FunctionCallback &&f1, Function &&f, Args &&... args)
    {
        std::future<R> strFuSec = std::async(std::launch::deferred, std::forward<Function>(f), std::forward<Args>(args)...);
        return new QThreadCreateThread<R>(std::move(strFuSec), std::move(f1));
    }

    template <typename R, typename Function, typename... Args>
    static QThread* createFuture(Function &&f, Args &&... args)
    {
        std::packaged_task<R()> task(std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
        return new QThreadCreateTaskThread<R>(std::move(task));
    }
};

#endif // THREAD_H
