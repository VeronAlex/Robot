#pragma once
#include "Definer.h"
#include "Drink_control.h"
#include "Order_control.h"
#include "Robot.h"

using namespace std;

enum EState { EWait, EMove, ERotateLeft, ERotateRight, EMoveForward, EFillingGlass, ENotifyTheCustomer, EWaitCustomer};
class Robot_control {
    Definer *df;
    Robot *bot;
    Order_control *ord_control;
    Drink_control *dr_control;

    EState state;

    bool GlassFull = false;
    bool MoveToHome = false;

    void ControlBot();

public:
    Robot_control(Definer *d, Robot *robot, Order_control *order, Drink_control *dr);
    void start();
};