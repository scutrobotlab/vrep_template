#include "controller.h"

void Controller::init()
{
    switch (ctrl_method)
    {
    case CtrlMethodEnumdef::pid_mode:
        _pid.init();
        break;
    case CtrlMethodEnumdef::lqr_mode:
        _lqr.init();
    default:
        break;
    }
}

void Controller::pid_ctrl()
{
    
}

Controller::Controller(CtrlMethodEnumdef ctrlMethod)
{
    ctrl_method = ctrl_method；
    init();
}

Controller::~Controller()
{
}