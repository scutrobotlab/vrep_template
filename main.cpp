#include "RobotLauncher.h"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int main(int argc, char *argv[])
{
    RobotLauncher launcher;
    launcher.LaunchAllProgram();
    int in_param = 0;
    while (1)
    {
        /* code */
        std::cout << "ENTER PARAM TO FSM" << std::endl;
        std::cin >> in_param;

        switch (in_param)
        {
            case 1:
                FSM_Base<float>::SetNextState(FSM_StateName::STAND_UP);
                break;
            case 2:
                FSM_Base<float>::SetNextState(FSM_StateName::LOCOMOTION);
            default:
                break;
        }
    };
    
}

/************************* END-OF-FILE SCUT-ROBOTLAB **************************/