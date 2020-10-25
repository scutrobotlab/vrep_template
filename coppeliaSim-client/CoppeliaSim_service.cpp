/**
 * @file   CoppeliaSim_service.cpp
 * @author Mentos Seetoo (201730645459@mail.scuut.edu.cn)
 * @brief  This is a demo for using the CoppeliaSim Client in your project,
 *         which we also demonstrate how to synchronize your threads with
 *         CoppeliaSim Client Service(thread created by this module).
 *         
 */

#include "CoppeliaSim_service.h"
#include <mutex>

static std::mutex g_com_state_mtx; /* g_com_state的互斥锁  */
static int g_com_state = 0;        /* 通信状态位，在transmit和通信线程中更改与查询 */
                                   /* 1:通信完成 0：开始通信*/
/**
 * @brief Used in test for CoppeliaSim Client.
 * @note  Start instance thread of coppeliasim-client, using 127.0.0.1:5000, Synchronous
          Mode is enabled.
 * 
 */
void CoppeliaSim_Service()
{
    std::cout << "[CoppeliaSim Client] Successfully created service thread ! \n";
    CoppeliaSim_Client *hClient = &CoppeliaSim_Client::getInstance();
    std::cout << "[CoppeliaSim Client] Connecting to server.. \n";

    /*
      Make sure to connect.
    */
    //you can change these parameters.
    while (!hClient->Start("127.0.0.1", 5000, 5, true, true))
    {
    };

    std::cout << "[CoppeliaSim Client] Successfully connected to server !\n";

    /*Infinite loop*/
    for (;;)
    {
        /*
            synchronize mode.
        */
        if (Get_com_state() == 0)
        {
            if (hClient->Is_Connected())
            {
                hClient->ComWithServer();
            }
            Set_com_state(1);
        }
        /*
            If you didn't use synchronize mode, just call one function as below.
        */
        // if (hClient->Is_Connected())
        // {
        //     hClient->ComWithServer();
        // }
    }
}

void Coppeliasim_Client_Init()
{
    CoppeliaSim_Client *hClient = &CoppeliaSim_Client::getInstance();
    std::cout << "[CoppeliaSim Client] Initializing service, setting parameters... \n";
    std::cout << "[CoppeliaSim Client] Creating client service thread... \n";
    std::thread sim_thread(CoppeliaSim_Service);
    sim_thread.detach();
}

int Get_com_state()
{
    return g_com_state;
}

void Set_com_state(int _com_state)
{

    while (!g_com_state_mtx.try_lock())
        ;
    g_com_state = _com_state;
    g_com_state_mtx.unlock();
}

/************************ END-OF-FILE SCUT-ROBOTLAB **************************/