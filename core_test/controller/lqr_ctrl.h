#pragma once
#include "controller.h"
#include "common/common.h"

class lqr_ctrl:public Controller
{
private:
    /* data */
public:
    void init();
    void task();
    lqr_ctrl(/* args */);
    ~lqr_ctrl();
};


