/* Includes ------------------------------------------------------------------*/
#include "CoppeliaSim.h"
#include "matplotlibcpp.h"
#include "sys_log.h"
#include "core_test/car.h"

/* Usr defines ---------------------------------------------------------------*/
using namespace std; 
namespace plt = matplotlibcpp;
/*Object Handle -----------------------------------------------------------*/
_simObjectHandle_Type* Body;
_simObjectHandle_Type* Joint[5];
/*Signal Handle -----------------------------------------------------------*/
_simSignalHandle_Type* EulerAngle[3];

/*Usr Parameters ----------------------------------------------------------*/
mycar car;
LogFilter_t Filters[3];
/* Founctions ----------------------------------------------------------------*/
uint32_t getSimTime();

/**
* @brief This is the main function for user.
*/
void Usr_Main()
{
    //core code
    car.car_spin();
}

/**
* @brief User can config simulation client in this function.
* @note  It will be called before entering the main loop.   
*/
void Usr_ConfigSimulation()
{
    Body = CoppeliaSim->Add_Object("car", OTHER_OBJECT, { SIM_ORIENTATION | CLIENT_RO, SIM_VELOCITY | CLIENT_RO });
    Joint[lf_wheel] = CoppeliaSim->Add_Object("lf_wheel", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RW, SIM_FORCE | CLIENT_RO });
    Joint[lh_wheel] = CoppeliaSim->Add_Object("lh_wheel", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RW, SIM_FORCE | CLIENT_RO});
    Joint[rf_wheel] = CoppeliaSim->Add_Object("rf_wheel", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RW, SIM_FORCE | CLIENT_RO });
    Joint[rh_wheel] = CoppeliaSim->Add_Object("rh_wheel", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RW, SIM_FORCE | CLIENT_RO });
    Joint[servo] = CoppeliaSim->Add_Object("servo", JOINT, { SIM_VELOCITY | CLIENT_RW, SIM_POSITION | CLIENT_RW, SIM_FORCE | CLIENT_RO });
    EulerAngle[yaw] = CoppeliaSim->Add_Object("car.YawAng", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
    EulerAngle[pitch] = CoppeliaSim->Add_Object("car.PitchAng", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
    EulerAngle[roll] = CoppeliaSim->Add_Object("car.RollAng", SIM_FLOAT_SIGNAL, { SIM_SIGNAL_OP | CLIENT_RO });
}

/**
* @brief These two function will be called for each loop.
*        User can set their message to send or read from sim enviroment.
*/
void Usr_SendToSimulation()
{
    
    for(int i = 0; i < 4; i++)
    {
        if(i == servo){
            Joint[i]->obj_Target.angle_f = car.mycar_data.joint_data.out[i];
            return;
        }
        Joint[i]->obj_Target.angVelocity_f = car.mycar_data.joint_data.out[i];
    }
}

void Usr_ReadFromSimulation()
{
    for(int i = 0; i < 4; i++)
    {
        car.mycar_data.joint_data.curr_pos[i] = Joint[i]->obj_Data.angle_f;
        // car.mycar_data.joint_data.curr_vel[i] = Joint[i]->obj_Data.angVelocity_f;
        car.mycar_data.joint_data.curr_torque[i] = Joint[i]->obj_Data.torque_f;
    }
    car.mycar_data.roll = EulerAngle[roll]->data;
    car.mycar_data.pitch = EulerAngle[pitch]->data;
    car.mycar_data.yaw = EulerAngle[yaw]->data;

    // for (size_t i = 0; i < 4; i++)
    // {
    //     cout << "vel" << i <<:  " <<car.mycar_data.joint_data.curr_vel[i];
    // }
    

    cout << "RPY angle:" 
    <<car.mycar_data.roll << ","
    << car.mycar_data.pitch << ","
    << car.mycar_data.yaw << endl;
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
    std::cout << "[System Logger] Logger is ready ! \n";

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