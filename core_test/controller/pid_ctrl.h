#pragma once
#include "common/common.h"

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

class pid_ctrl
{
private:
    /* data */
    CtrlObjectEnumdef _obj;
    PIDDataTypedef pos_data;
    PIDDataTypedef vel_data;
    PIDDataTypedef tau_data;
    JointDataTypedef *_joint; 
public:
    pid_ctrl(JointDataTypedef &joint, CtrlObjectEnumdef obj);
    void init();
    float ctrl();
    float ctrl(PIDDataTypedef &_ctrl, float tar, float curr);

    void set_param(CtrlObjectEnumdef obj, float _p, float _i, float _d, float i_limit, float out_limit);
    ~pid_ctrl();
};

