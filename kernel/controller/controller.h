#pragma once
#include "common/common.h"
class Controller
{
    private:
        /* data */
    public:
        JointDataTypedef *_joint; 

        Controller();
        ~Controller();
        virtual void init() = 0;
        virtual void task() = 0;
        virtual void adjust() = 0;
};


