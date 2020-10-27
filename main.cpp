/* Includes ------------------------------------------------------------------*/
#include "coppeliaSim.h"
#include "matplotlibcpp.h"
#include "sys_log.h"

/* Usr defines ---------------------------------------------------------------*/
using namespace std; 
namespace plt = matplotlibcpp;
enum {
    yaw = 0,
    pitch,
    roll
};
enum {
    left_hip = 0,
    left_wheel,
    right_hip,
    right_wheel
};
_simObjectHandle_Type* Body;
_simObjectHandle_Type* Joint[4];
_simSignalHandle_Type* EulerAngle[3];
LogFilter_t Filters[3];

/* Founctions ----------------------------------------------------------------*/
uint32_t getSimTime();

/**
* @brief This is the main function for user.
*/
void Usr_Main()
{

}

/**
* @brief User can config simulation client in this function.
* @note  It will be called before entering the main loop.   
*/
void Usr_ConfigSimulation()
{
    Body = CoppeliaSim->Add_Object("Body", OTHER_OBJECT, { SIM_ORIENTATION | CLIENT_RO, SIM_VELOCITY | CLIENT_RO });
    Joint[left_hip] = CoppeliaSim->Add_Object("left_hip", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO, SIM_FORCE | CLIENT_WO });
    Joint[left_wheel] = CoppeliaSim->Add_Object("left_wheel", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO, SIM_FORCE | CLIENT_WO});
    Joint[right_hip] = CoppeliaSim->Add_Object("right_hip", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO, SIM_FORCE | CLIENT_WO });
    Joint[right_wheel] = CoppeliaSim->Add_Object("right_wheel", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO, SIM_FORCE | CLIENT_WO });
    EulerAngle[yaw] = CoppeliaSim->Add_Object("Infantry.YawAng", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
    EulerAngle[pitch] = CoppeliaSim->Add_Object("Infantry.PitchAng", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
    EulerAngle[roll] = CoppeliaSim->Add_Object("Infantry.RollAng", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
}

/**
* @brief These two function will be called for each loop.
*        User can set their message to send or read from sim enviroment.
*/
void Usr_SendToSimulation()
{

}

void Usr_ReadFromSimulation()
{

}

/**
* @brief It's NOT recommended that user modefies this function.
*        Plz programm the functions with the prefix "Usr_". 
*/
int main(int argc, char *argv[])
{
    CoppeliaSim_Client *hClient = &CoppeliaSim_Client::getInstance();
    /*
        System Logger tool init.
    */
    std::cout << "[System Logger] Configuring... \n";
    SysLog->getMilliTick_regist(getSimTime);
    std::cout << "[System Logger] Logger is ready to be used ! \n";

    /*
        Simulation connection init.
    */
    std::cout << "[CoppeliaSim Client] Connecting to server.. \n";
    while (!hClient->Start("127.0.0.1", 5000, 5, true)){};
    std::cout << "[CoppeliaSim Client] Successfully connected to server, configuring...\n";
    Usr_ConfigSimulation();
    std::cout << "[CoppeliaSim Client] Configure done, simulation is ready ! \n";

    while (1)
    {
        std::cout << "I'm running" <<endl;
        if (hClient->Is_Connected())
        {
            hClient->ComWithServer();
        }
        Usr_ReadFromSimulation();
        Usr_Main();
        Usr_SendToSimulation();
    };
}


uint32_t getSimTime()
{ 
    return 0;
}
/************************* END-OF-FILE SCUT-ROBOTLAB **************************/