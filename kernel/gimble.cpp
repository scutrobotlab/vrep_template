#include "gimble.h"

void Gimble::init()
{

        gimble_body = {0};
        gimble_joint = {0};
        /*---controller init---*/
        /*--- triangular wave generator demo---*/
                /*
                        No initialization steps.
                */
        /*--- PID demo ---*/  
                      
                        pid.init();
                        pid.tau_loop_enable(false);
                        pid.set_joint(gimble_joint);
                        pid.set_ctrlobj(CtrlObjectEnumdef::pos_type);
                        gimble_joint.pos_d = 2;
                        _pos = {5.5, 0, 0.2, 10, 10};//p-5.5 i-0.0 d-0.24 
                        _vel = {0.25, 0.06, 0.27, 10, 10};//p 0.3 i 0.08 d 0.27
                        _tau = {1, 0, 0, 10, 10};

                        pid.set_param(CtrlObjectEnumdef::pos_type, _pos);
                        pid.set_param(CtrlObjectEnumdef::vel_type, _vel);
                        pid.set_param(CtrlObjectEnumdef::tau_type, _tau);
                
        /*--- LQR demo ---*/

}

void Gimble::task()
{
        /*core code
        -Here you can do your kernel task.
        -You can validate your algorithm such as 3-loop-PID, LQR, MPC, WBC, ADRC, SMC and so on.
        -The control target is decided by yourself，and the final output to the joint must be torque(or may say current).
        */
        /*--- triangular wave generator demo---*/
                /*
                        static float delta = 1;
                        gimble_joint.tau_d += delta;
                        if (gimble_joint.tau_d >= 5)
                                delta = -1;
                        if (gimble_joint.tau_d <= -5)    
                                delta = 1;
                */
        /*--- PID demo ---*/  
                
                        pid.adjust();
                        pid.task();
                
        /*--- LQR demo ---*/

}
Gimble::Gimble(/* args */)
{
}
Gimble::~Gimble(){}
