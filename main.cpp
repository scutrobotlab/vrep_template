/* Includes ------------------------------------------------------------------*/
#include "CoppeliaSim.h"
#include "kernel/gimble.h"
/* Usr defines ---------------------------------------------------------------*/
using namespace std; 

/*Object Handle -----------------------------------------------------------*/
_simObjectHandle_Type* Body;
_simObjectHandle_Type* Joint;
/*Signal Handle -----------------------------------------------------------*/
_simSignalHandle_Type* EulerAngle[3];

/*Usr Parameters ----------------------------------------------------------*/
Gimble gimble;
/* Founctions ----------------------------------------------------------------*/
uint32_t getSimTime();
/**
* @brief This is the initialization function for user.
*/
void Usr_Init()
{ 
    gimble.init();
}

/**
* @brief This is the main function for user.
*/
void Usr_Main()
{
    //core code
    gimble.task();
}

/**
* @brief User can config simulation client in this function.
* @note  It will be called before entering the main loop.   
*/
void Usr_ConfigSimulation()
{
    Body = CoppeliaSim->Add_Object("base", OTHER_OBJECT, { SIM_ORIENTATION | CLIENT_RO, SIM_VELOCITY | CLIENT_RO });
    Joint = CoppeliaSim->Add_Object("joint", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RO, SIM_FORCE | CLIENT_RW });
}

/**
* @brief These two function will be called for each loop.
*        User can set their message to send or read from sim enviroment.
*/
void Usr_SendToSimulation()
{
    Joint->obj_Target.torque_f = gimble.gimble_joint.tau_d;
}

void Usr_ReadFromSimulation()
{
    gimble.gimble_joint.tau_fb = Joint->obj_Data.torque_f;
    gimble.gimble_joint.pos_fb = Joint->obj_Data.angle_f;
    gimble.gimble_joint.vel_fb = Joint->obj_Data.angVelocity_f;
}
// std::cout << "gimble joint torque : " << gimble.gimble_joint.tau_fb<< endl;
    // std::cout << "gimble joint position : " << gimble.gimble_joint.pos_fb<< endl;
    // std::cout << "gimble joint velocity : " << gimble.gimble_joint.vel_fb<< endl;
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
    std::cout << "[System Logger] Logger is ready ! \n";
    
    /*
        Simulation connection init.
    */
    std::cout << "[CoppeliaSim Client] Connecting to server.. \n";
    while (!hClient->Start("127.0.0.1", 5000, 5, true)){};
    std::cout << "[CoppeliaSim Client] Successfully connected to server, configuring...\n";
    Usr_ConfigSimulation();
    std::cout << "[CoppeliaSim Client] Configure done, simulation is ready ! \n";

    /*
        User init.
    */
    Usr_Init();
    while (1)
    {
        if (hClient->Is_Connected())
        {
            hClient->ComWithServer();
        }
        Usr_ReadFromSimulation();
        Usr_Main();
        Usr_SendToSimulation();
    };
}
/************************* END-OF-FILE SCUT-ROBOTLAB **************************/