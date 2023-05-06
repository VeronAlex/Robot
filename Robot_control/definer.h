#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>


using namespace cv;

struct color_val {
    int h;
    int s;
    int v;
    color_val(int h, int s, int v)
    {
        this->h = h;
        this->s = s;
        this->v = v;
    }
};

class Definer {
    Mat src_mat;
    VideoCapture cap;

    color_val ColorBotFront= color_val(0, 0, 0);
    color_val ColorBotBack = color_val(0, 0, 0);
    color_val ColorTarget = color_val(0, 0, 0);
    color_val ColorHome = color_val(0, 0, 0);

    Rect rectBotFront;
    Rect rectBotBack;
    Rect rectTarget;
    Rect rectHome;

    Point RobotCentre;
    Point TargetPoint;
    Point HomePoint;

    int angle_bot;
    int angle_target;
    int distance_target;
    int angle_home;
    int distance_home;

    static Point find_rect_centre(Rect r);
    static Point findMiddle_between_points(Point p1, Point p2);
    static int calc_angle(Point p1, Point p2);
    static int calc_distance(Point p_a, Point p_b);

    void defineCentrePoints();
    void defineAngle_to_target();
    void define_distance_to_target();
    void define_angle_to_home();
    void define_distance_to_home();

    Rect find_rect(color_val color);

public:

    Definer(int id);

    void setColorBotFront(int h, int s, int v);
    void setColorBotBack(int h, int s, int v);
    void setColorTarget(int h, int s, int v);
    void setColorHome(int h, int s, int v);

    void robot_define();
    void show_inf();

    int get_angle_to_target();
    int get_distance_to_target();
    int get_angle_to_home();
    int get_distance_to_home();

    ~Definer();
};