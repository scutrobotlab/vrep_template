#include "linux_support.h"
#include <string.h>
#include <pthread.h>
/*System log support*/
using namespace std;
Timer* SysTimer = &Timer::getInstance();
ofstream outFile;
char outMsg[256];
pthread_mutex_t g_outFile_mx = PTHREAD_MUTEX_INITIALIZER,
                g_Console_mx = PTHREAD_MUTEX_INITIALIZER;

uint32_t LogFile_Output(uint8_t *msg, uint16_t len)
{
    //TEST Use mutex may cause delay in control
    pthread_mutex_lock(&g_outFile_mx);

    memcpy(outMsg, msg, len);
    outMsg[len - 1] = 0;

    outFile.open("./cache/sys_log.txt", ios::app);
    if (outFile.is_open())
        outFile << outMsg;
    else
    {
    };
    outFile.close();

    pthread_mutex_unlock(&g_outFile_mx);
    return 0;
}

uint32_t LogConsole_Output(uint8_t *msg, uint16_t len)
{
    //TEST Use mutex may cause delay in control
    pthread_mutex_lock(&g_Console_mx);

    memcpy(outMsg, msg, (size_t)len);
    outMsg[len] = 0;

    cout << outMsg;

    pthread_mutex_unlock(&g_Console_mx);
    return 0;
}

uint32_t getLinux_runtime()
{
    // /* Time may overflow */
    // static struct timeval raw_time;
    // static unsigned int begin_t = 0, now_t = 0;
    // gettimeofday(&raw_time, NULL);
    // if (begin_t == 0)
    //     begin_t = raw_time.tv_sec * 1000 + raw_time.tv_usec * 0.001;
    // else
    //     now_t = raw_time.tv_sec * 1000 + raw_time.tv_usec * 0.001 - begin_t;
    // return (uint32_t)now_t;
    return SysTimer->getMs_uint();
}