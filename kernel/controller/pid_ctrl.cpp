#include "pid_ctrl.h"
void pid_ctrl::set_joint(JointDataTypedef &joint)
{
    _joint = &joint;
}

void pid_ctrl::set_ctrlobj(CtrlObjectEnumdef obj)
{
    _obj = obj;
}

pid_ctrl::pid_ctrl()
{   
}

pid_ctrl::~pid_ctrl()
{}
float pid_ctrl::ctrl()
{   
    if(is_tau_loop)
    {
        if (_obj == CtrlObjectEnumdef::pos_type)
            return ctrl(tau_data, ctrl(vel_data, ctrl(pos_data, _joint->pos_d, _joint->pos_fb), _joint->vel_fb), _joint->tau_fb);
        if (_obj == CtrlObjectEnumdef::vel_type)
            return ctrl(tau_data, ctrl(vel_data, _joint->vel_d, _joint->vel_fb), _joint->tau_fb);
        return ctrl(tau_data, _joint->tau_d, _joint->tau_fb);
    }
    else
    {
        if (_obj == CtrlObjectEnumdef::pos_type)
            return ctrl(vel_data, ctrl(pos_data, _joint->pos_d, _joint->pos_fb), _joint->vel_fb);
        if (_obj == CtrlObjectEnumdef::vel_type)
            return ctrl(vel_data, _joint->vel_d, _joint->vel_fb);
        return _joint->tau_d;
    }
}

float pid_ctrl::ctrl(PIDDataTypedef &_ctrl, float tar, float curr)
{
    _ctrl.curr_err = tar - curr;
    _ctrl.p_term = _ctrl._p * _ctrl.curr_err;

    _ctrl.all_err += _ctrl.curr_err; 
    _ctrl.i_term = _ctrl._i * _ctrl.all_err;
    if(_ctrl.i_term > 0)
        _ctrl.i_term = (_ctrl.i_term > _ctrl.i_limit)?(_ctrl.i_limit):(_ctrl.i_term);
    else
        _ctrl.i_term = (_ctrl.i_term < -_ctrl.i_limit)?(-_ctrl.i_limit):(_ctrl.i_term);

    _ctrl.d_term = _ctrl._d * (_ctrl.curr_err - _ctrl.last_err);
    _ctrl.last_err = _ctrl.curr_err;

    _ctrl.out = _ctrl.p_term + _ctrl.i_term + _ctrl.d_term;
    if (_ctrl.out > 0)
        _ctrl.out = (_ctrl.out > _ctrl.out_limit)?(_ctrl.out_limit):(_ctrl.out);
    else
        _ctrl.out = (_ctrl.out < -_ctrl.out_limit)?(-_ctrl.out_limit):(_ctrl.out);
    return _ctrl.out;
}

void pid_ctrl::set_param(CtrlObjectEnumdef obj, PIDDataTypedef &pid_param)
{
    switch (obj)
    {
        case CtrlObjectEnumdef::pos_type:
            pos_data = pid_param;
            break;
        case CtrlObjectEnumdef::vel_type:
            vel_data = pid_param;
            break;
        case CtrlObjectEnumdef::tau_type:
            tau_data = pid_param;
            break;
        default:
            break;
    }
}

void pid_ctrl::tau_loop_enable(bool yn)
{
    is_tau_loop = yn;
}

void pid_ctrl::init()
{
    pos_data = { 0 };
    vel_data = { 0 };
    tau_data = { 0 };
}
void pid_ctrl::task()
{
    _joint->tau_d = ctrl();
    //std::cout<<"V_err: "<<std::setprecision(2)<<vel_data.curr_err<<"   V_ALL:   "<<vel_data.all_err<<"  T_err: "<<std::setprecision(2)<<tau_data.curr_err<<"  T_ALL:   "<<tau_data.all_err<<std::endl;
}

void pid_ctrl::adjust()
{
    char key;
        static float delta = 0.005;
        static int type = 1;
        if (_kbhit()) 
        {  
            key = _getch();  
            std::cout << key <<std::endl;
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
                            pos_data._p += delta;
                            std::cout <<" pos_p "<<pos_data._p <<std::endl;
                    }else
                    {
                            vel_data._p += delta;
                            std::cout <<" vel_p "<<vel_data._p <<std::endl;
                    }
                    break;
            case 's':
                    if (type)
                    {
                            pos_data._p -= delta;
                            std::cout <<" pos_p "<<pos_data._p <<std::endl;
                    }else
                    {
                            vel_data._p -= delta;
                            std::cout <<" vel_p "<<vel_data._p <<std::endl;
                    }
                    break;
            case 'a':
                    if (type)
                    {
                            pos_data._i += delta;
                            std::cout <<" pos_i "<<pos_data._i <<std::endl;
                    }else
                    {
                            vel_data._i += delta;
                            std::cout <<" vel_i "<<vel_data._i <<std::endl;
                    }
                    
                    break;
            case 'd':
                    if (type)
                    {
                            pos_data._i -= delta;
                            std::cout <<" pos_i "<<pos_data._i <<std::endl;
                    }else
                    {
                            vel_data._i -= delta;
                            std::cout <<" vel_i "<<vel_data._i <<std::endl;
                    }
                    break;
            case 'q':
                    if (type)
                    {
                            pos_data._d += delta;
                            std::cout <<" pos_d "<<pos_data._d <<std::endl;
                    }else
                    {
                            vel_data._d += delta;
                            std::cout <<" vel_d "<<vel_data._d <<std::endl;
                    }
                    break;
            case 'e':
                    if (type)
                    {
                            pos_data._d -= delta;
                            std::cout <<" pos_d "<<pos_data._d <<std::endl;
                    }else
                    {
                            vel_data._d -= delta;
                            std::cout <<" vel_d "<<vel_data._d <<std::endl;
                    }
                    break;      
            case 'v':
                    std::cout << "joint velocity : " << _joint->vel_fb<< std::endl;  
                    break;      
            case 'p':
                    std::cout << "joint position : " << _joint->pos_fb<< std::endl;  
                    break;   
            case 'z':
                    _joint->pos_d += 1;
                    std::cout << "pos target : " << _joint->pos_d<< std::endl;  
                    break;
            case 'c':
                    _joint->pos_d -= 1;
                    std::cout << "pos target : " << _joint->pos_d<< std::endl;  
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
    }
}