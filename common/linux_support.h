
#include <time.h>
#include <fstream>
#include <iostream>

#ifndef _LINUX_SUPPORT_H
#define _LINUX_SUPPORT_H

uint32_t getLinux_runtime();
uint32_t LogFile_Output(uint8_t *msg, uint16_t len);
uint32_t LogConsole_Output(uint8_t *msg, uint16_t len);

/*!
 * @brief Timer for measuring time elapsed with clock_monotonic.
 *        Using Meyers' Singleton, luanching itself when the process is started.
 */
class Timer
{
public:
    static Timer &getInstance()
    {
        static Timer _instance;
        return _instance;
    }

    /*!
   * Construct and start timer
   */
    explicit Timer() { start(); }

    /*!
   * Start the timer
   */
    void start() { clock_gettime(CLOCK_MONOTONIC, &_startTime); }

    /*!
   * Get milliseconds elapsed
   */
    double getMs_double() { return (double)getNs_int() / 1.e6; }
    uint32_t getMs_uint() { return getNs_int() / 1.e6;}
    /*!
   * Get nanoseconds elapsed
   */
    int64_t getNs_int()
    {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return (int64_t)((now.tv_nsec - _startTime.tv_nsec) + 
               1000000000 * (now.tv_sec - _startTime.tv_sec));
    }

    /*!
   * Get seconds elapsed
   */
    double getSeconds_double() { return (double)getNs_int() / 1.e9; }

    struct timespec _startTime;
};

extern Timer* SysTimer;

#endif
/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/