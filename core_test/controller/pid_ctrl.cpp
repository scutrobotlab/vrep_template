#include "pid_ctrl.h"


pid_ctrl::pid_ctrl(JointDataTypedef &joint, CtrlObjectEnumdef obj)
{   
    _joint = &joint;
    _obj = obj;
}
pid_ctrl::~pid_ctrl()
{}
float pid_ctrl::ctrl()
{   
    if (_obj == CtrlObjectEnumdef::pos_type)
        return ctrl(tau_data, ctrl(vel_data, ctrl(pos_data, _joint->pos_d, _joint->pos_fb), _joint->vel_fb), _joint->tau_fb);
    else if (_obj == CtrlObjectEnumdef::vel_type)
        return ctrl(tau_data, ctrl(vel_data, _joint->vel_d, _joint->vel_fb), _joint->tau_fb);
    return ctrl(tau_data, _joint->tau_d, _joint->tau_fb);
}

float pid_ctrl::ctrl(PIDDataTypedef &_ctrl, float tar, float curr)
{
    _ctrl.curr_err = tar - curr;
    _ctrl.p_term = _ctrl._p * _ctrl.curr_err;

    _ctrl.all_err += _ctrl.curr_err; 
    _ctrl.i_term = _ctrl._i * _ctrl.all_err;
    _ctrl.i_term = (_ctrl.i_term > _ctrl.i_limit)?(_ctrl.i_limit):(_ctrl.i_term);
    
    _ctrl.d_term = _ctrl._d * (_ctrl.curr_err - _ctrl.last_err);
    _ctrl.last_err = _ctrl.curr_err;

    _ctrl.out = _ctrl.p_term + _ctrl.i_term + _ctrl.d_term;
    _ctrl.out = (_ctrl.out > _ctrl.out_limit)?(_ctrl.out_limit):(_ctrl.out);
    return _ctrl.out;
}

void pid_ctrl::set_param(CtrlObjectEnumdef obj, float _p, float _i, float _d, float i_limit, float out_limit)
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

    ctrl->_p = _p;
    ctrl->_i = _i;
    ctrl->_d = _d;
    ctrl->i_limit = i_limit;
    ctrl->out_limit = out_limit;
}

void pid_ctrl::init()
{
    pos_data = { 0 };
    vel_data = { 0 };
    tau_data = { 0 };
}