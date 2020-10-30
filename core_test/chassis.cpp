/**
  ******************************************************************************
  * Copyright (c) 2019 - ~, SCUT-RobotLab Development Team
  * @file    chassis.cpp
  * @author  Mentos Seetoo 1356046979@qq.com
  * @brief   Chassis model support file.This file provide object for general
  *          chassis(4 wheels) and a special chassis(Lunar Chassis,6 wheels).
  *          The object contain three modes(Normal-Speed, Normal-Position and
  *          Follow-Position).
  * @date    2019-06-22
  * @version 1.2 (Beta)
  * @par Change Log:
  * <table>
  * <tr><th>Date        	<th>Version  <th>Author    		    <th>Description
  * <tr><td>2019-06-22   	<td> 0.1     <td>Mentos Seetoo    <td>Creator
  * <tr><td>2019-11-27   	<td> 1.0     <td>Kainan.Su        <td>Change macro definition to chassis parameter variable.
  *                                                             Use <stdint.h> instead of self-defined.
  * <tr><td>2019-12-09    <td> 1.1     <td>Mentos Seetoo    <td>Add `Swerve Chassis` to supported model.
  * <tr><td>2020-10-30    <td> 1.2     <td>Mentos Seetoo    <td>Add `Two Legged Wheel` to supported model.
  * </table>
  *																		 
  ==============================================================================
                               How to use this Lib
  ==============================================================================
    @note
      -# 编译前如果要用月球车底盘，则需定义`LUNAR_CHASSIS`
         舵轮底盘：`SWERVE_CHASSIS`
         双足轮:'TWO_LEGGED_WHEEL'
  
      -# 新建"CChassis"对象，构造时设置初始的底盘机动性能与物理参数，选择是否进行力矩
         优化和姿态保护.加载外部的控制器Load_xxxxController().
  
      -# 按照配置的运行频率调用 Chassis_Control().通过Set_xx()设置参数、目标
         使用Update_xx()更新一些当前值（位置模式下使用）
         
    @attention
      - 在整定底盘控制器参数时，请注意通过调节运动加速度大小，使任何一个电机输出不会
        长时间被输出限幅作用（大于电机最大输出时进入饱和区，使底盘运动性能恶化）。
        
      - ARM_MATH & DSP in STM32F4 required!!!

  ******************************************************************************
  * @attention
  * 
  * if you had modified this file, please make sure your code does not have many 
  * bugs, update the version Number, write dowm your name and the date, the most
  * important is make sure the users will have clear and definite understanding 
  * through your new brief.
  *
  * <h2><center>&copy; Copyright (c) 2019 - ~, SCUT-RobotLab Development Team.
  * All rights reserved.</center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "chassis.h"
/* Private define ------------------------------------------------------------*/
#define _ch_dir(x)  (x >= 0 ? 1 : -1)
/* Functions -----------------------------------------------------------------*/
_chassis_Mode CChassis::Chassis_Control()
{
  //底盘位置环的速度输出,无位置环时使用外部速度指令
  position_control();

  //外部速度指令时启动限幅优化
  acce_optimization();
  
  //姿态保护，在优化的基础上叠加输出，并联结构
  attitude_protect();
  
  //关节控制输出计算
  joint_control();
  
  return Mode;
}

/**
* @brief  Resolve the command velocity from command pose in 
          position mode.
* @param  None.
* @return 1:operated.
*         0:aborted.
* @author Mentos Seetoo
*/
inline uint8_t CChassis::position_control()
{
  static _chassis_GlobalPos Target_Pos,Relative_Pos;
  
  /*Check Postion controller*/
  if(position_controller == NULL) return 0;
  
  /* Only in position mode */
  Target_Pos = Command_Pos;
  if(Mode == Normal_Position)
  {
    Relative_Pos = Current_Pos - Zero_Pos;
    //Calculate target velocity by external controller.
    Target_Velocity = *(position_controller(&Relative_Pos, &Target_Pos));
  }
  else if(Mode == Follow_Position)
  {
    //Current_Pos & Zero_Pos is disable in this mode.
    memset(&Relative_Pos,0,sizeof(_chassis_GlobalPos));
    
    //Calculate target velocity by external controller.
    Target_Velocity = *(position_controller(&Relative_Pos, &Target_Pos));
  }
  else{};
  
  return 1;
}

/**
* @brief  Optimize the acceleration during launching or breaking.
* @param  None.
* @return 1:operated.
*         0:aborted.
* @author Mentos_Seetoo
*/
inline uint8_t CChassis::acce_optimization()
{
  static _chassis_Velocity Last_Target;
  static float dspeed,max_accceleration;
  
  /* Only in speed mode */
  if(Mode == Normal_Speed)
  {
    if(Param.acceOptimize_flag == 1)
    {
      /*
        Launch.(Acceleration = rpm / time)
        If the acceleration(torque) for staring process is too large, the
        chassis is easy to skid. When the chassis doesn't skid, startup distance 
        will decrease. Below code firstly change 'Last_Target', because 
        'Target_Velocity' may be changed elsewhere.
      */
      /* X Axis */
      if(_Chassis_Abs(Command_Velocity.x_speed) > _Chassis_Abs(Last_Target.x_speed))
      { //accelerate
        if(_Chassis_Abs(Command_Velocity.x_speed) <= Param.launch_speed)
          max_accceleration = Param.max_launch_acceleration;
        else
          max_accceleration = Param.max_normal_acceleration;
      }
      else//decelerate
        max_accceleration = Param.max_brake_acceleration;
      
      dspeed = max_accceleration*((float)Param.RUN_INTERVAL);
      
      if(_Chassis_Abs(Command_Velocity.x_speed - Last_Target.x_speed) >= dspeed)
      {
        if(Command_Velocity.x_speed >= Last_Target.x_speed)
          Last_Target.x_speed += dspeed;
        else Last_Target.x_speed -= dspeed;
      }
      else
        Last_Target.x_speed = Command_Velocity.x_speed;
      
      /* Y Axis */
      if(_Chassis_Abs(Command_Velocity.y_speed) > _Chassis_Abs(Last_Target.y_speed))
      { //accelerate
        if(_Chassis_Abs(Command_Velocity.y_speed) <= Param.launch_speed)
          max_accceleration = Param.max_launch_acceleration;
        else
          max_accceleration = Param.max_normal_acceleration;
      }
      else//decelerate
        max_accceleration = Param.max_brake_acceleration;
      
      dspeed = max_accceleration*((float)Param.RUN_INTERVAL);
      
      if(_Chassis_Abs(Command_Velocity.y_speed - Last_Target.y_speed) >= dspeed)
      {
        if(Command_Velocity.y_speed >= Last_Target.y_speed)
          Last_Target.y_speed += dspeed;
        else Last_Target.y_speed -= dspeed;
      }
      else
        Last_Target.y_speed = Command_Velocity.y_speed;
      
      //Z don't limit acceleration
      Last_Target.z_speed = Command_Velocity.z_speed;
    }
    else
      Last_Target = Command_Velocity;
    
    Target_Velocity = Last_Target;
    return 1;
  }
  return 0;
}

/**
* @brief  Protect the chassis from turning over.Unit: degree.
* @param  None.
* @return 1:operated.
*         0:aborted.
* @author Mentos Seetoo
*/
uint8_t CChassis::attitude_protect()
{
  static float locked_yaw = 0;
  static _chassis_Velocity* Attitude_Compensation;
  
  /*Check Postion controller*/
  if(position_controller == NULL) return 0;
  
  if(Target_Velocity.z_speed != 0)
    locked_yaw = Current_Pos.yaw;
  
  /* Only in speed mode */
  if(Mode == Normal_Speed && Param.attitudeOptimize_flag == 1)
  {
    /* Yaw Axis stabilize */
    Command_Pos.yaw = locked_yaw;
    /* Pitch Axis stabilize */
    
    Attitude_Compensation = attitude_controller(&Current_Pos, &Command_Pos);
    Target_Velocity = Target_Velocity + (*Attitude_Compensation);
    return 1;
  }
  else return 0;
}

/**
* @brief  Resolve the speed & torque for chassis & calculate output.
* @param  None.
* @return 1:operated.
*         0:aborted.
* @author Mentos Seetoo
*/
uint8_t CChassis::joint_control()
{
  static int16_t  TempBuff[6] = {0};
  static float scale = 0;
  
  /* Check Speed Controller */
  if(speed_controller == NULL) return 0;
  
  /*
    Resolve wheel speed(steer angle).
  */
  if(Mode == Halt)
  {
    Target_Velocity.x_speed = 0;
    Target_Velocity.y_speed = 0;
    Target_Velocity.z_speed = 0;
  }
  model_resolve();

#if defined(TWO_LEGGED_WHEEL)
  /*
    Calculate control values for joints by external controller.
  */
  memcpy(joint_Out,legWheel_controller(&Current_Pos, joint_angle, &Target_Velocity),
         sizeof(int32_t)*JOINT_NUM);
#else
  /*
    Constrain for motor rpm.For the chassis based on composition of velocity,
    if there any joint output speed is larger than the "MOTOR_MAX_SPEED", other
    joint output will be constrain proportionally to keep the right scale, 
    otherwise the chassis will get an incorrect movement direction.
  */
  for (uint8_t k = 0; k < JOINT_NUM; ++k )
     TempBuff[k] = _Chassis_Abs(joint_rpmOut[k]);
       
  _Chassis_BubbleSort(TempBuff,JOINT_NUM);
  if(TempBuff[JOINT_NUM - 1] >= Param.MAX_MOTOR_SPEED)
      scale = ((float)Param.MAX_MOTOR_SPEED)/((float)TempBuff[JOINT_NUM - 1]);
  else
      scale = 1.0f;
  
  for (uint8_t k = 0; k < JOINT_NUM; ++k )
     joint_rpmOut[k] = (int16_t)((float)joint_rpmOut[k] * scale);

  /*
    Calculate control values for joints by external controller.
  */
  if(speed_controller != NULL)
    memcpy(joint_Out, speed_controller(joint_rpm, joint_rpmOut), sizeof(int32_t)*JOINT_NUM);
#endif

  /*
    Constrain for joint output to protect the motor.
  */
  for (uint8_t k = 0; k < JOINT_NUM; ++k )
     joint_Out[k] = _Chassis_Constrain(joint_Out[k], (int32_t)-Param.MAX_MOTOR_OUTPUT, (int32_t)Param.MAX_MOTOR_OUTPUT);
  
  return 1;
}


/**
* @brief  Set the chassis gear ,pose target or speed coefficient 
          of three axis.
          (x_Speed = wheel_max_speed * x_co * gear_limit_Co)
* @param  target_X: 1.from -1 to 1 （Speed Mode）
                    2.no limit     (Position Mode)
*         targetGear: slow,normal,fast
* @return None.
* @author Mentos_Seetoo
*/
void CChassis::Set_Target(float target_X, float target_Y ,float target_Z)
{
  static float Co_limit = 0;
  
  if(current_gear == NORMAL_GEAR)    Co_limit = Param.coefficient_normal;
  else if(current_gear == FAST_GEAR) Co_limit = Param.coefficient_fast;
  else Co_limit = Param.coefficient_slow;
  
  /* Calculate velocity(Only in 'Normal-Speed') .*/
  if(Mode == Normal_Speed)
  {
    Command_Velocity.x_speed = _Chassis_Constrain(target_X,-1.0f,1.0f)*Co_limit*Param.MAX_MOTOR_SPEED;
    Command_Velocity.y_speed = _Chassis_Constrain(target_Y,-1.0f,1.0f)*Co_limit*Param.MAX_MOTOR_SPEED;
    Command_Velocity.z_speed = _Chassis_Constrain(target_Z,-1.0f,1.0f)*Param.coefficient_z*Param.MAX_MOTOR_SPEED;
  }
  
  /* Set command pose */
  else if((Mode == Normal_Position) || (Mode == Follow_Position))
  {
    Command_Pos.x   = target_X;
    Command_Pos.y   = target_Y;
    Command_Pos.yaw = target_Z;
  }
}

void CChassis::Set_SpeedGear(_chassis_SpeedGears targetGear)
{
  if(targetGear < 3)
    current_gear = targetGear;
}

void CChassis::Set_SpeedParam(float slow, float normal, float fast, float z)
{
  Param.coefficient_slow   = _Chassis_Constrain(slow, 0.0f, 1.0f);
  Param.coefficient_normal = _Chassis_Constrain(normal, 0.0f, 1.0f);
  Param.coefficient_fast   = _Chassis_Constrain(fast, 0.0f, 1.0f);
  Param.coefficient_z      = _Chassis_Constrain(z, 0.0f, 1.0f);
}

void CChassis::Set_AccelerationParam(int16_t launch, int16_t normal, uint16_t brake)
{
	launch = _Chassis_Constrain((int16_t)launch, (int16_t)-32765, (int16_t)32765);
	normal = _Chassis_Constrain((int16_t)normal, (int16_t)-32765, (int16_t)32765);
	brake = _Chassis_Constrain((int16_t)normal, (int16_t)0, (int16_t)65535);
	
  Param.max_launch_acceleration = _Chassis_Abs(launch);
  Param.max_normal_acceleration = _Chassis_Abs(normal);
	Param.max_brake_acceleration  = _Chassis_Abs(brake);
}

void CChassis::Set_TorqueOptimizeFlag(bool flag)
{
  Param.acceOptimize_flag = flag;
}

void CChassis::Set_AttitudeOptimizeFlag(bool flag)
{
  Param.attitudeOptimize_flag = flag;
}

uint8_t CChassis::Switch_Mode(_chassis_Mode target_mode)
{
  /*
    Reserve for checking parameter.
  */
  Mode = target_mode;
  switch(target_mode)
  {
    case Normal_Speed:
      //Set the zero point at first time.
      if(Mode != Normal_Speed){
        Command_Pos.yaw = Current_Pos.yaw;
      }
      break;
    default:
      break;
  }
  return 1;
}

_chassis_Mode CChassis::Get_Mode()
{
  return Mode;
}

/**
* @brief  Load the controllers
          Attention:pFunc can not return temporary address!
* @param  Current, Target
* @return _chassis_Velocity: calculate the velocity from position loop.
          int  Output array for operator.
* @author Mentos Seetoo
*/
void CChassis::Load_AttitudeController(_chassis_Velocity*(*pFunc)(const _chassis_GlobalPos* Current, const _chassis_GlobalPos* Target))
{
  attitude_controller = pFunc;
}

void CChassis::Load_PositionController(_chassis_Velocity*(*pFunc)(const _chassis_GlobalPos* Current, const _chassis_GlobalPos* Target))
{
  position_controller = pFunc;
}

void CChassis::Load_SpeedController(int32_t* (*pFunc)(const int16_t* current, const int16_t* target))
{
  speed_controller = pFunc;
}
void CChassis::Load_LegWheelController(int32_t*(*pFunc)(const _chassis_GlobalPos* fb_pos, const int32_t* fb_joint_ang, const _chassis_Velocity* fb_vel))
{
  legWheel_controller = pFunc;
}
/**
* @brief  Update zero pose or current pose of chassis.
* @param  None.
* @return None.
* @author Mentos Seetoo
*/
void CChassis::Update_ZeroPose()
{
  Zero_Pos.x = Current_Pos.x;
  Zero_Pos.y = Current_Pos.y;
  Zero_Pos.roll = Current_Pos.roll;
  Zero_Pos.yaw  = Current_Pos.yaw;
  Zero_Pos.pitch= Current_Pos.pitch;
}

void CChassis::Update_CurrentPosition(float current_x, float current_y )
{
  Current_Pos.x = current_x;
  Current_Pos.y = current_y;
}

void CChassis::Update_CurrentAttitude(float roll, float pitch, float yaw)
{
  Current_Pos.roll  = roll;
  Current_Pos.pitch = pitch;
  Current_Pos.yaw   = yaw;
}


/**
 * @brief  Resolve wheel speed for different chassis model
 * @param  Value: speed of every axis.(Unit:rpm)
 * @param  wheels_speed*: wheel speed to output(Unit:rpm)
 * @retval None.
 * @author 
 */
inline void CChassis::model_resolve()
{
  /* Normal 4 Mecanum wheeels & Lunar chassis */
#if defined(SWERVE_CHASSIS)
/* Swerve Chassis */
  /*!< Angel between X-Axis and chassis diagonal line*/
  static const float co_cos = Param.CHASSIS_WHEEL_TRACK/sqrtf(powf(Param.CHASSIS_WHEEL_TRACK,2) + powf(Param.CHASSIS_WHEEL_BASE,2)),\
                     co_sin = Param.CHASSIS_WHEEL_BASE/sqrtf(powf(Param.CHASSIS_WHEEL_TRACK,2) + powf(Param.CHASSIS_WHEEL_BASE,2)),
                     dead_zone_high = 0.5f*Param.STEER_RANGE, dead_zone_low = PI - 0.5f*Param.STEER_RANGE;
  static float jointSpeed_x[JOINT_NUM],jointSpeed_y[JOINT_NUM],cal_temp, delta_angle;
  
  /* Orthogonal decomposition of chassis velocity */
  jointSpeed_x[LF] = Target_Velocity.x_speed - Target_Velocity.z_speed * co_cos;
  jointSpeed_x[RF] = jointSpeed_x[LF];
  jointSpeed_x[RB] = Target_Velocity.x_speed + Target_Velocity.z_speed * co_cos;
  jointSpeed_x[LB] = jointSpeed_x[RB];
  
  jointSpeed_y[LF] = Target_Velocity.y_speed - Target_Velocity.z_speed * co_sin;
  jointSpeed_y[RF] = Target_Velocity.y_speed + Target_Velocity.z_speed * co_sin;
  jointSpeed_y[RB] = jointSpeed_y[LF];
  jointSpeed_y[LB] = jointSpeed_y[RF];
  
  /*Composition of joint velocity*/
  for(uint8_t cnt = 0; cnt < JOINT_NUM; cnt ++)
  {
    /*
      Calculate speed.
    */
  #ifdef ARM_MATH_USED
    arm_sqrt_f32(powf(jointSpeed_x[cnt],2) + powf(jointSpeed_y[cnt],2),&cal_temp);
    joint_rpmOut[cnt] = cal_temp;
  #else
    joint_rpmOut[cnt] = sqrt(powf(jointSpeed_x[cnt],2) + powf(jointSpeed_y[cnt],2));
  #endif
    
    /*
      Calculate polar angle.
    */
    if(joint_rpmOut[cnt] != 0)
    {
      /* Minimum delta angle pinciple */
      delta_angle = atan2f(jointSpeed_y[cnt], jointSpeed_x[cnt]) - steer_Set[cnt].vect_angleAbs;

      if(_Chassis_Abs(delta_angle) > PI)
        steer_Set[cnt].vect_angle -= _ch_dir(delta_angle)*(2*PI - _Chassis_Abs(delta_angle));
      else
        steer_Set[cnt].vect_angle += delta_angle;
      
      steer_Set[cnt].vect_angleAbs = fmodf(steer_Set[cnt].vect_angle,2*PI);
      steer_Set[cnt].vect_angleAbs -= _Chassis_Abs(steer_Set[cnt].vect_angleAbs) > PI ? _ch_dir(steer_Set[cnt].vect_angleAbs)*2*PI : 0;
    }
    else{/*Do nothing for angle*/}
    
    /*
      Calculate steer angle.
    */
    if(Param.STEER_RANGE == 2*PI)
    { /*No limitation*/
      steer_Set[cnt].str_angle = steer_Set[cnt].vect_angle;
    }
    else
    { /*Steer angle is limited*/
      cal_temp = _Chassis_Abs(steer_Set[cnt].vect_angleAbs);
      
      if(cal_temp > dead_zone_high)
      {
        steer_Set[cnt].str_angle = steer_Set[cnt].vect_angleAbs - _ch_dir(steer_Set[cnt].vect_angleAbs)*PI;
        steer_Set[cnt].speed_direction = NEGATIVE;
      }
      else if(cal_temp > dead_zone_low)
      {
        if(steer_Set[cnt].speed_direction == POSITIVE)
          steer_Set[cnt].str_angle = steer_Set[cnt].vect_angleAbs;
        if(steer_Set[cnt].speed_direction == NEGATIVE)
          steer_Set[cnt].str_angle = steer_Set[cnt].vect_angleAbs - _ch_dir(steer_Set[cnt].vect_angleAbs)*PI;
      }
      else
      {
        steer_Set[cnt].str_angle = steer_Set[cnt].vect_angleAbs;
        steer_Set[cnt].speed_direction = POSITIVE;
      }
      
      joint_rpmOut[cnt] *= steer_Set[cnt].speed_direction;
    }
  }
#elif defined(TWO_LEGGED_WHEEL)
  
#else
  joint_rpmOut[LF] =   Target_Velocity.x_speed + Target_Velocity.y_speed - Target_Velocity.z_speed;
  joint_rpmOut[RF] = - Target_Velocity.x_speed + Target_Velocity.y_speed + Target_Velocity.z_speed;
  joint_rpmOut[RB] =   Target_Velocity.x_speed + Target_Velocity.y_speed + Target_Velocity.z_speed;
  joint_rpmOut[LB] = - Target_Velocity.x_speed + Target_Velocity.y_speed - Target_Velocity.z_speed;
  #ifdef LUNAR_CHASSIS
  static const float z_scale = Param.CHASSIS_WHEEL_TRACK/(Param.CHASSIS_WHEEL_TRACK + Param.CHASSIS_WHEEL_BASE);
  joint_rpmOut[LM] = Target_Velocity.y_speed - z_scale * Target_Velocity.z_speed;
  joint_rpmOut[RM] = Target_Velocity.y_speed + z_scale * Target_Velocity.z_speed;
  #endif
#endif
}


/************************ COPYRIGHT(C) SCUT-ROBOTLAB **************************/
