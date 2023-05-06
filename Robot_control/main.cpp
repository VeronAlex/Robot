#include "robot_control.h"
#include "definer.h"
#include "robot.h"
#include "order_control.h"
#include "drink_control.h"
#include "mqtt.h"
using namespace std;
using namespace cv;

#define port 1883


int main() {
    setlocale(LC_ALL, "Russian");
    Definer *df=new Definer(0);
    Robot *robot=new Robot_mqtt(port);
    Drink_control *drink_control=new Drink_control;
    Order_control *order;

    Robot_control *rob_control(new Robot_control(df, robot, order, drink_control));

    df->setColorBotFront(34, 65, 255);
    df->setColorBotBack(176, 130, 170);
    df->setColorTarget(90, 249, 68);
    df->setColorHome(130, 120, 65);


    rob_control->start();

    return 0;
}