#pragma once
#include "common/common.h"
#include "controller.h"

class pid_ctrl :public Controller
{
private:
    /* data */
    CtrlObjectEnumdef _obj;
    PIDDataTypedef pos_data;
    PIDDataTypedef vel_data;
    PIDDataTypedef tau_data;
    bool is_tau_loop = true;
public:
    pid_ctrl();

    void init();
    void task();
    void adjust();
    float ctrl();
    float ctrl(PIDDataTypedef &_ctrl, float tar, float curr);

    void tau_loop_enable(bool yn);
    void set_ctrlobj(CtrlObjectEnumdef obj);
    void set_joint(JointDataTypedef &joint);
    void set_param(CtrlObjectEnumdef obj, PIDDataTypedef &pid_param);
    ~pid_ctrl();
};

