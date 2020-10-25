/**
  ******************************************************************************
  * Copyright (C) 2020 - ~, SCUT-RobotLab Development Team
  * @file    sys_thread.cpp
  * @author  Mentos Seetoo
  * @brief   Sys-Thread is an abstract extend class for threads, which also 
  *          record monitor running time & peroid. 
  *          Master-Slave Mode, which somehow like Reactor Mode in Java, is used
  *          to simply synchronize task in different threads, but it's unsafe
  *          and not tested for now.
  * @warning 
  *     - At least C++11 is required.	
  *     - Only linux platform is supported for now.
  * //TODO Add support for WIN32.													 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <sys/timerfd.h>
#include <unistd.h>
#include <cmath>
#include "sys_thread.h"


std::vector<CThread *> CThread::_thread_list;

CThread::CThread(float period, std::string thread_mame, 
                ThreadType thread_type = ThreadType::INDEPENDENT) 
                : _period(period), _name(thread_mame), _type(thread_type)
{
    _running = false;
    _maxPeriod = 0;
    _maxRuntime = 0;
    _lastPeriod = 0;
    _lastRuntime = 0;

    if(_type == ThreadType::SLAVE)
        _sync_lock = true;
    else _sync_lock = false;

    _thread_list.push_back(this);
}

void CThread::start()
{
    if (_running)
    {
        printf("[Thread Manager] Tried to start %s but it was already running!\n",
               _name.c_str());
        return;
    }

    pthread_create(&_pid, NULL, _thread_t<CThread, &CThread::_thread>, this);
    _running = true;
}

void CThread::exit()
{
    if (!_running)
    {
        printf("[Thread Manager] Tried to stop %s but it wasn't running!\n",
               _name.c_str());
        return;
    }

    _running = false;
    printf("[Thread Manager] Waiting for %s to stop...\n", _name.c_str());
    void *rev;
    pthread_join(_pid, &rev);
    printf("[Thread Manager] Done!\n");
    clean_up();
}

bool CThread::isSlow()
{
    return _maxRuntime > _period * 1.3f;
}

void CThread::printStatus()
{
    if (!_running)
        return;
    if (isSlow())
    {
        printf("|%-20s|%6.4f|%6.4f|%6.4f|%6.4f|%6.4f|Delay\n", _name.c_str(),
               _lastRuntime, _maxRuntime, _period, _lastPeriod, _maxPeriod);
    }
    else
    {
        printf("|%-20s|%6.4f|%6.4f|%6.4f|%6.4f|%6.4f|*    \n", _name.c_str(),
               _lastRuntime, _maxRuntime, _period, _lastPeriod, _maxPeriod);
    }
}

void CThread::printAllThread()
{
    printf("\n----------------------------THREADS----------------------------\n");
    printf("|%-20s|%-6s|%-6s|%-6s|%-6s|%-6s|%-6s\n", "name", "rt", "rt-max", "T-des",
           "T-act", "T-max", "Status");
    printf("---------------------------------------------------------------\n");
    for (auto &thread : _thread_list)
    {
        thread->printStatus();
        thread->clearMaxRuntime();
        thread->clearMaxPeriodtime();
    }
    printf("---------------------------------------------------------------\n\n");
}

void CThread::cutAllThread()
{
    for (auto &thread : _thread_list)
        thread->exit();
}

void CThread::_thread()
{
    /*Pre-load for task*/
    double last_enter_time = 0;
    int seconds = (int)_period;
    int nanoseconds = (int)(1e9 * std::fmod(_period, 1.f));
    printf("[Thread Manager] Start %s (Period : %d s, %d ns)\n", _name.c_str(), seconds, nanoseconds);
    init();

    /*Set the timer*/
    auto timerFd = timerfd_create(CLOCK_MONOTONIC, 0);
    itimerspec timerSpec;
    timerSpec.it_interval.tv_sec = seconds;
    timerSpec.it_value.tv_sec = seconds;
    timerSpec.it_value.tv_nsec = nanoseconds;
    timerSpec.it_interval.tv_nsec = nanoseconds;
    timerfd_settime(timerFd, 0, &timerSpec, nullptr);
    unsigned long long missed = 0;
    last_enter_time = SysTimer->getSeconds_double();

    /*Infinite loop*/
    while (_running)
    {
        /*Synchronize under master-slave mode*/
        if(this->_type == ThreadType::MASTER)
        {
            auto len = _thread_list.size();
            this->_sync_lock = false;
            for(auto tr(0); tr < len; tr++)
            {
                if(_thread_list[tr]->_type == ThreadType::SLAVE && !_thread_list[tr]->_sync_lock)
                {
                    this->_sync_lock = true;
                    break;
                }
            }
        }

        if(!this->_sync_lock)
        {
            _lastPeriod = SysTimer->getSeconds_double() - last_enter_time;
            last_enter_time += _lastPeriod;
            /*Run the main task*/
            task();

            /*Record & pass to next thread*/
            _lastRuntime = SysTimer->getSeconds_double() - last_enter_time;
            _maxRuntime = std::max(_maxRuntime, _lastRuntime);
            _maxPeriod = std::max(_maxPeriod, _lastPeriod);
            _maxPeriod = (_maxPeriod > 1.5 * _period) ? 1.5 * _period : _maxPeriod;
            if (_period != 0)
                int m = read(timerFd, &missed, sizeof(missed));
            else
            {
                _maxPeriod = 0;
                _lastPeriod = 0;
            }

            /*Synchronize under master-slave mode*/
            if(_type == ThreadType::INDEPENDENT) _sync_lock = false;
            else _sync_lock = true;

            if(_type == ThreadType::MASTER)
            {
                for(auto tr : _thread_list) 
                {
                    if(tr->_type == ThreadType::SLAVE)
                        tr->_sync_lock = false;
                }
            }
        }
    }
    printf("[Thread Manager] %s has stopped!\n", _name.c_str());
}

/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/