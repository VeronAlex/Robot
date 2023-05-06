#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <vector>
#define M_PI 3.14159265358979323846
#include "definer.h"

using namespace cv;
using namespace std;

//---------Установка цветов--------------
void Definer::setColorBotFront(int h, int s, int v) {
    ColorBotFront = color_val(h, s, v);
}
void Definer::setColorBotBack(int h, int s, int v) {
    ColorBotBack = color_val(h, s, v);
}
void Definer::setColorTarget(int h, int s, int v) {
    ColorTarget = color_val(h, s, v);
}
void Definer::setColorHome(int h, int s, int v) {
    ColorHome = color_val(h, s, v);
}

//Формулы нахождения центра, угла, расстояния
Point Definer::find_rect_centre(Rect rect) {
    int x = rect.x + (rect.width / 2);
    int y = rect.y + (rect.height / 2);

    return Point(x, y);
}
Point Definer::findMiddle_between_points(Point p1, Point p2) {
    int x = (p1.x + p2.x) / 2;
    int y = (p1.y + p2.y) / 2;

    return Point(x, y);
}

int Definer::calc_angle(Point a, Point b) {
    double angle = atan2((b.x - a.x), (b.y - a.y)) / M_PI * 180;

    if (angle >= 0)
        return angle;
    else
        return 360 + angle;
}
int Definer::calc_distance(Point p_a, Point p_b) {
    return sqrt((p_b.x - p_a.x) * (p_b.x - p_a.x) + (p_b.y - p_a.y) * (p_b.y - p_a.y));
}

//----------------Определение углов, расстояний--------------------

void Definer::defineCentrePoints() {

    this->rectBotFront = this->find_rect(this->ColorBotFront);
    this->rectBotBack = this->find_rect(this->ColorBotBack);
    this->rectTarget = this->find_rect(this->ColorTarget);
    this->rectHome = this->find_rect(this->ColorHome);

    this->TargetPoint = find_rect_centre(rectTarget);
    this->HomePoint = find_rect_centre(rectHome);
    this->RobotCentre = findMiddle_between_points(find_rect_centre(rectBotFront), find_rect_centre(rectBotBack));
    this->angle_bot = calc_angle(find_rect_centre(rectBotFront), find_rect_centre(rectBotBack));
}

void Definer::defineAngle_to_target() {
    int angle_bot_centre_to_target = calc_angle(this->RobotCentre, this->TargetPoint);
    this->angle_target = angle_bot_centre_to_target - this->angle_bot;
}

void Definer::define_distance_to_target() {
    this->distance_target = calc_distance(this->RobotCentre, this->TargetPoint);
}

void Definer::define_angle_to_home() {
    int angle_bot_centre_to_home = calc_angle(this->RobotCentre, this->HomePoint);
    this->angle_home = angle_bot_centre_to_home - this->angle_bot;
}
void Definer::define_distance_to_home() {
    this->distance_home = calc_distance(this->RobotCentre, this->HomePoint);
}

//---------------Получение углов, расстояний--------------
int Definer::get_angle_to_target() {
    return this->angle_target;
}
int Definer::get_distance_to_target() {
    return this->distance_target;
}
int Definer::get_angle_to_home() {
    return this->angle_home;
}
int Definer::get_distance_to_home() {
    return this->distance_home;
}

//-----------------Определение контуров----------------
Rect Definer::find_rect(color_val color) {
    Mat hsv_mat;
    vector< vector< cv::Point> > contours;
    cvtColor(src_mat, hsv_mat, COLOR_BGR2HSV);
    cv::Mat cop_mat(src_mat.size(), CV_8U);
    cv::inRange(hsv_mat, cv::Scalar(color.h - 10, color.s - 25, color.v - 25), cv::Scalar(color.h + 10, color.s + 25, color.v + 257), cop_mat);
    cv::dilate(cop_mat, cop_mat, cv::Mat(), cv::Point(-1, -1), 3);
    cv::erode(cop_mat, cop_mat, cv::Mat(), cv::Point(-1, -1), 1);
    findContours(cop_mat, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    cv::Rect max_rect(0, 0, 0, 0);
    for (uint i = 0; i < contours.size(); ++i)
    {
        Rect rect;
        rect = boundingRect(contours[i]);
        if (rect.height * rect.width > max_rect.height * max_rect.width) {
            max_rect = rect;
        }
    }
    cv::rectangle(src_mat, max_rect, Scalar(255, 0, 0), 2);
    return max_rect;
}

//---------------Открытие камеры------------------
Definer::Definer(int id) {
    cap.open(id);
    if (!cap.isOpened())
        cout << "Error opening video stream or file" << endl;
    for (;;)
    {
        cap.read(src_mat);
        if (src_mat.empty()) {
            cout << "ERROR!\n";
            break;
        }
        if (waitKey(5) >= 0)
            break;
    }
}

void Definer::robot_define() {
    this->defineCentrePoints();
    this->defineAngle_to_target();
    this->define_distance_to_target();
    this->define_angle_to_home();
    this->define_distance_to_home();
}


void Definer::show_inf() {

    Mat res_image = src_mat;
    int font_size = 1;
    Scalar font_Color(90, 100, 0);
    Scalar font_Color1(255, 255, 255);
    int font_weight = 2;

    // До цели
    Point text_position(30, 30);
    putText(res_image, "Target", text_position, FONT_HERSHEY_COMPLEX, font_size, font_Color1, font_weight);
    text_position.x = 20;
    text_position.y = 60;
    putText(res_image, "Angle:"+to_string(this->get_angle_to_target()), text_position, FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);
    text_position.x = 20;
    text_position.y = 90;
    putText(res_image, "Distance:"+to_string(this->get_distance_to_target()), text_position, FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);

    // До дома
    text_position.x = 410;
    text_position.y = 30;
    putText(res_image, "Home", text_position, FONT_HERSHEY_COMPLEX, font_size, font_Color1, font_weight);
    text_position.x = 400;
    text_position.y = 60;
    putText(res_image, "Angle:" + to_string(this->get_angle_to_home()), text_position, FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);
    text_position.x = 400;
    text_position.y = 90;
    putText(res_image, "Distance:"+to_string(this->get_distance_to_home()), text_position, FONT_HERSHEY_COMPLEX, font_size, font_Color, font_weight);
    imshow("Find rectangle", res_image);
}

Definer::~Definer() {
    cap.release();
    destroyAllWindows();
}