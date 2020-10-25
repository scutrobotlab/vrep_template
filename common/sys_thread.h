/**
  ******************************************************************************
  * Copyright (C) 2020 - ~, SCUT-RobotLab Development Team
  * @file    sys_thread.h
  * @author  Mentos Seetoo
  * @brief   Sys-Thread is an abstract extend class for threads, which also 
  *          record monitor running time & peroid. 
  * @warning 
  *     - At least C++11 is required.	
  *     - Only linux platform is supported for now.													 
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#ifndef _SYS_THREAD_H
#define _SYS_THREAD_H

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "linux_support.h"
/* Types define --------------------------------------------------------------*/
/**
 * @brief For single function
 */
template <typename TYPE, void (TYPE::*_thread)()>
void *_thread_t(void *param)
{
    ((TYPE *)param)->_thread();
    return NULL;
}

enum class ThreadType
{
    MASTER,
    SLAVE,
    INDEPENDENT
};

/**
 * @brief For series of threads
 */
class CThread
{
public:
    CThread(float period, std::string thread_mame, ThreadType thread_type);
    void start();
    void exit();
    void printStatus();
    void _thread();
    virtual void init() = 0;
    virtual void task() = 0;
    virtual void clean_up() = 0;
    ~CThread() { exit(); }

    /*!
    * Get the last time it took for a run
    */
    float getLastRuntime() { return _lastRuntime; }
    /*!
    * Get the maximum time it took for a run
    */
    float getMaxRuntime() { return _maxRuntime; }
    
    float getPreSetPeriod(){ return _period; }
    /*!
    * Clear the maximum time it took for a run
    */
    void clearMaxRuntime() { _maxRuntime = 0; }
    /*!
    * Clear the maximum time it took for a period
    */
    void clearMaxPeriodtime() { _maxPeriod = 0; }

    static void cutAllThread();
    static void printAllThread();
private:
    static std::vector<CThread*> _thread_list;
    std::string _name;
    pthread_t _pid;
    ThreadType _type;
    volatile bool _sync_lock;
    volatile bool _running;
    float _period;
    float _lastRuntime, _maxRuntime;
    float _lastPeriod, _maxPeriod;

    bool isSlow();
};
#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/