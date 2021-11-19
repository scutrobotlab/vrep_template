#pragma once
#include "pid_ctrl.h"
#include "lqr_ctrl.h"

class Controller
{
    private:
        /* data */
        CtrlMethodEnumdef ctrl_method;
    public:
        pid_ctrl _pid;
        lqr_ctrl _lqr;
        Controller(CtrlMethodEnumdef ctrlMethod);
        ~Controller();
        void init();
        void task();

};


