#include "gimble.h"

void Gimble::init()
{

        gimble_body = {0};
        gimble_joint = {0};
        /*---controller init---*/
        pid.init();
        pid.enable(false);
        pid.set_joint(gimble_joint);
        pid.set_ctrlobj(CtrlObjectEnumdef::vel_type);
        _pos = {0.5, 0, 0.1, 10, 10};
        _vel = {0.17, 0.07, 0.27, 10, 10};//p 0.17 i 0.07 d 0.27
        _tau = {1, 0, 0, 10, 10};

        pid.set_param(CtrlObjectEnumdef::pos_type, _pos);
        pid.set_param(CtrlObjectEnumdef::vel_type, _vel);
        pid.set_param(CtrlObjectEnumdef::tau_type, _tau);

        gimble_joint.vel_d = 1;
}
void Gimble::adjust()
{
        char key;
        if (_kbhit()) 
        {  
                key = _getch();  
                std::cout << key <<std::endl;
        }
        switch (key)
        {
                case 'w':
                        _vel._p += 0.005;
                        std::cout <<" vel_P "<<_vel._p <<std::endl;
                        break;
                case 's':
                        _vel._p -= 0.005;
                        std::cout <<" vel_P "<<_vel._p <<std::endl;
                        break;
                case 'a':
                        _vel._i += 0.005;
                        std::cout <<" vel_I "<<_vel._i <<std::endl;
                        break;
                case 'd':
                        _vel._i -= 0.005;
                        std::cout <<" vel_I "<<_vel._i <<std::endl;
                        break;
                case 'q':
                        _vel._d += 0.005;
                        std::cout <<" vel_D "<<_vel._d <<std::endl;
                        break;
                case 'e':
                        _vel._d -= 0.005;
                        std::cout <<" vel_D "<<_vel._d <<std::endl;
                        break;      
                case 'v':
                        std::cout << "gimble joint velocity : " << gimble_joint.vel_fb<< std::endl;  
                        break;      
                case 'p':
                        std::cout << "gimble joint position : " << gimble_joint.pos_fb<< std::endl;  
                        break;   
                case 'z':
                        gimble_joint.vel_d += 0.2;
                        std::cout << "velocity target : " << gimble_joint.vel_d<< std::endl;  
                        break;
                case 'c':
                        gimble_joint.vel_d -= 0.2;
                        std::cout << "velocity target : " << gimble_joint.vel_d<< std::endl;  
                        break;
                default:
                        break;
        }
        pid.set_param(CtrlObjectEnumdef::vel_type, _vel);
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
        pid.task();
        adjust();
}
Gimble::Gimble(/* args */)
{
}
Gimble::~Gimble(){}
