#include "gimble.h"

void Gimble::init()
{

        gimble_body = {0};
        gimble_joint = {0};
        /*---controller init---*/
        PIDParamTypedef _pos, _vel, _tau;

        pid.init();
        pid.set_joint(gimble_joint);
        pid.set_ctrlobj(CtrlObjectEnumdef::pos_type);
        _pos = {0.5, 0, 0.5, 5, 5};
        _vel = {1, 0.5, 0, 10, 10};
        _tau = {1, 0, 0, 10, 20};
        
        pid.set_param(CtrlObjectEnumdef::pos_type, _pos);
        pid.set_param(CtrlObjectEnumdef::vel_type, _vel);
        pid.set_param(CtrlObjectEnumdef::tau_type, _tau);
}

void Gimble::task()
{
        /*core code
        -Here you can do your kernel task.
        -You can validate your algorithm such as 3-loop-PID, LQR, MPC, WBC, ADRC, SMC and so on.
        -The control target is decided by yourself，and the final output to the joint must be torque(or may say current).
        */
        /*---1-> triangular wave generator demo---*/
                /*
                        static float delta = 1;
                        gimble_joint.tau_d += delta;
                        if (gimble_joint.tau_d >= 5)
                                delta = -1;
                        if (gimble_joint.tau_d <= -5)    
                                delta = 1;
                */
        /*---2-> 3-loop-PID demo---*/  
        gimble_joint.pos_d = 1.57;
        pid.task();
}
Gimble::Gimble(/* args */)
{
}
Gimble::~Gimble(){}
