#pragma once
#include "common/common.h"
#include "controller.h"

typedef struct PIDData
{
    float curr_err;
    float last_err;
    float all_err;

    float _p;
    float _i;
    float _d;

    float p_term;
    float i_term;
    float d_term;

    float i_band;//for integral separated
    float i_limit;//for integral limited

    float out;//pid output
    float out_limit;//for output limited
}PIDDataTypedef;

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
    float ctrl();
    float ctrl(PIDDataTypedef &_ctrl, float tar, float curr);

    void enable(bool yn);
    void set_ctrlobj(CtrlObjectEnumdef obj);
    void set_joint(JointDataTypedef &joint);
    void set_param(CtrlObjectEnumdef obj, PIDParamTypedef &pid_param);
    ~pid_ctrl();
};

