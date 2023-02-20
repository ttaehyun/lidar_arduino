#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include "lidar_arduino/ardu_scan.h"
#include "lidar_arduino/control.h"

lidar_arduino::ardu_scan ardu_scan;
ldiar_arduino::control control_msg;

void automove() {
    for (i=126; i<252; i++) {
        if (ardu_scan.ranges[i] <=0.4) {
            control_msg.direction = 40;     //계산 라이다 각도 값과 서보모터 움직이는 각도 계산식 만들어서 자동으로 서보모터 출력값 생성되게 하기
        }
        ardu_pub.publish(control_msg)
    }
}

void scan_callback(const sensor_msgs::LaserScan scan_msg) {
    ardu_scan.ranges = scan_msgs.ranges;
}

int main(int argc, char **argv) {
    ros::init(argc, argv,"Cal_factory");
    ros::NodeHandle nh;
    //Publisher send to arduino
    ros::Publisher ardu_pub = nh.advertise<lidar_arduino::ardu_scan>("/topic",10);

    //Subscriber
    ros::Subscriber scan_sub = nh.subscribe("/scan", 10, scan_callback);

    while(ros::ok()) {
        automove();
        ros::spinOnce();
    }
    ros::spin();
    return 0;
}