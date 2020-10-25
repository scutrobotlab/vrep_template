#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include "CoppeliaSim.h"

void CoppeliaSim_Service();
void Coppeliasim_Client_Init();
int Get_com_state();
void Set_com_state(int _com_state); /* 1:通信完成 0：开始通信*/

