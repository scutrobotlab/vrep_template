#include "gimble.h"

void Gimble::init()
{

        gimble_body = {0};
        gimble_joint = {0};
        /*---controller init---*/

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
        
}
void Gimble::adjust()
{
        char key;
        static float delta = 0.005;
        static int type = 1;
        if (_kbhit()) 
        {  
                key = _getch();  
                std::cout << key <<std::endl;
        }
        switch (key)
        {
                case 'b':
                        type = !type;
                        if (type)
                                std::cout <<"Position mode " <<std::endl;
                        else
                                std::cout <<"Velocity mode " <<std::endl;
                        break;
                case 'w':
                        if (type)
                        {
                                _pos._p += delta;
                                std::cout <<" pos_p "<<_pos._p <<std::endl;
                        }else
                        {
                                _vel._p += delta;
                                std::cout <<" vel_p "<<_vel._p <<std::endl;
                        }
                        break;
                case 's':
                        if (type)
                        {
                                _pos._p -= delta;
                                std::cout <<" pos_p "<<_pos._p <<std::endl;
                        }else
                        {
                                _vel._p -= delta;
                                std::cout <<" vel_p "<<_vel._p <<std::endl;
                        }
                        break;
                case 'a':
                        if (type)
                        {
                                _pos._i += delta;
                                std::cout <<" pos_i "<<_pos._i <<std::endl;
                        }else
                        {
                                _vel._i += delta;
                                std::cout <<" vel_i "<<_vel._i <<std::endl;
                        }
                        
                        break;
                case 'd':
                        if (type)
                        {
                                _pos._i -= delta;
                                std::cout <<" pos_i "<<_pos._i <<std::endl;
                        }else
                        {
                                _vel._i -= delta;
                                std::cout <<" vel_i "<<_vel._i <<std::endl;
                        }
                        break;
                case 'q':
                        if (type)
                        {
                                _pos._d += delta;
                                std::cout <<" pos_d "<<_pos._d <<std::endl;
                        }else
                        {
                                _vel._d += delta;
                                std::cout <<" vel_d "<<_vel._d <<std::endl;
                        }
                        break;
                case 'e':
                        if (type)
                        {
                                _pos._d -= delta;
                                std::cout <<" pos_d "<<_pos._d <<std::endl;
                        }else
                        {
                                _vel._d -= delta;
                                std::cout <<" vel_d "<<_vel._d <<std::endl;
                        }
                        break;      
                case 'v':
                        std::cout << "gimble joint velocity : " << gimble_joint.vel_fb<< std::endl;  
                        break;      
                case 'p':
                        std::cout << "gimble joint position : " << gimble_joint.pos_fb<< std::endl;  
                        break;   
                case 'z':
                        gimble_joint.pos_d += 1;
                        std::cout << "pos target : " << gimble_joint.pos_d<< std::endl;  
                        break;
                case 'c':
                        gimble_joint.pos_d -= 1;
                        std::cout << "pos target : " << gimble_joint.pos_d<< std::endl;  
                        break;
                case 'n':
                        delta += 0.005;
                        std::cout << "delta: " << delta << std::endl;  
                        break;
                case 'm':
                        delta -= 0.005;
                        std::cout << "delta: " << delta << std::endl;  
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
        /*---3-> LQR demo---*/

}
Gimble::Gimble(/* args */)
{
}
Gimble::~Gimble(){}
