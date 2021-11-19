#include "gimble.h"
void Gimble::task()
{
        /*core code
        -Here you can do your kernel task.
        -You can validate your algorithm such as 3-loop-PID, LQR, MPC, WBC, ADRC, SMC and so on.
        -The control target is decided by yourself，and the final output to the joint must be torque(or may say current).
        */
        static float delta = 1;
        gimble_joint.tau_d += delta;
        if (gimble_joint.tau_d >= 5)
                delta = -1;
        if (gimble_joint.tau_d <= -5)    
                delta = 1;
}
void Gimble::init()
{
        gimble_body = {0};
        gimble_joint = {0};
}

Gimble::Gimble(/* args */)
{
        init();
}
Gimble::~Gimble(){}
