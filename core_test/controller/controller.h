#pragma once
#include "pid_ctrl.h"
#include "lqr_ctrl.h"

class Controller
{
    private:
        /* data */
    public:
        Controller();
        ~Controller();
        void init();
        void task();
        void pid_ctrl();
        void lqr_ctrl();
        void mpc_ctrl();
};


