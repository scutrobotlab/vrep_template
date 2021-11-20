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

void pid_ctrl::set_param(CtrlObjectEnumdef obj, PIDParamTypedef &pid_param)
{
    PIDDataTypedef *ctrl;
    switch (obj)
    {
        case CtrlObjectEnumdef::pos_type:
            ctrl = &pos_data;
            break;
        case CtrlObjectEnumdef::vel_type:
            ctrl = &vel_data;
            break;
        case CtrlObjectEnumdef::tau_type:
            ctrl = &tau_data;
            break;
        default:
            break;
    }

    ctrl->_p = pid_param._p;
    ctrl->_i = pid_param._i;
    ctrl->_d = pid_param._d;
    ctrl->i_limit = pid_param.i_limit;
    ctrl->out_limit = pid_param.out_limit;
}

void pid_ctrl::enable(bool yn)
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
