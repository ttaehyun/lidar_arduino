#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include "lidar_arduino/ardu_scan.h"
#include "lidar_arduino/control.h"
#include "lidar_arduino/lidar_scan_angle.h"

lidar_arduino::ardu_scan ardu_scan;
lidar_arduino::control control_msg;

// void automove() {
//     for (int i=126; i<252; i++) {
//         if (ardu_scan.ranges[i] <=0.4) {
//             control_msg.direction = 40;     //계산 라이다 각도 값과 서보모터 움직이는 각도 계산식 만들어서 자동으로 서보모터 출력값 생성되게 하기
//         }
//         ardu_pub.publish(control_msg)
//     }
// }

void scan_callback(const lidar_arduino::lidar_scan_angle lidar_angle) {
    //ardu_scan.ranges = lidar_angle.ranges;
    ROS_INFO("Range[%d] = %.10f",126,lidar_angle.ranges[126]); //90 degree -> 오차 범위 있는듯 수정 어케 하냐 또 90도 아닌지점이 측정됨
    if (lidar_angle.ranges[126] <=0.2) {
            control_msg.direction = 40;     //계산 라이다 각도 값과 서보모터 움직이는 각도 계산식 만들어서 자동으로 서보모터 출력값 생성되게 하기
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv,"Cal_factory");
    ros::NodeHandle nh;
    //Publisher send to arduino
    ros::Publisher ardu_pub = nh.advertise<lidar_arduino::control>("/topic",10);

    //Subscriber
    ros::Subscriber scan_sub = nh.subscribe("/lidar_angle", 10, scan_callback);

    //ros::Rate r(30);

    while(ros::ok()) {
        //automove();
        // for (int i=126; i<252; i++) {
            // if (ardu_scan.ranges[126] <=0.4) {
            // control_msg.direction = 40;     //계산 라이다 각도 값과 서보모터 움직이는 각도 계산식 만들어서 자동으로 서보모터 출력값 생성되게 하기
            // }
            ardu_pub.publish(control_msg);
        
        //ardu_pub.publish(control_msg);
        //r.sleep();
        ros::spinOnce();
    }
    
    return 0;
}