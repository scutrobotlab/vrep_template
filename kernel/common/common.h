#pragma once
#include "iostream"
#include <iomanip>
#include <conio.h>
#include <windows.h>
typedef struct PIDData
{
    float _p;
    float _i;
    float _d;

    float i_limit;//for integral limited
    float out_limit;//for output limited

    float i_band;//for integral separated

    float curr_err;
    float last_err;
    float all_err;

    float p_term;
    float i_term;
    float d_term;
    float out;//pid output
    
}PIDDataTypedef;

typedef struct JointData
{
        /*---command---*/
        float pos_d;
        float vel_d;
        float acc_d;
        float tau_d;

        /*---feedback---*/
        float pos_fb;
        float vel_fb;
        float acc_fb;
        float tau_fb;
}JointDataTypedef;

typedef enum ctrl_method_type
{
    pid_mode,
    lqr_mode,
    mpc_mode,
    adrc_mode,
    smc_mode,
    wbc_mode
}CtrlMethodEnumdef;

typedef enum ctrl_object_type
{
    pos_type,
    vel_type,
    tau_type
}CtrlObjectEnumdef;