#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include "lidar_arduino/control.h"

// 아래 msg 파일들은 왜 만듬?
// #include "lidar_arduino/ardu_scan.h"
// #include "lidar_arduino/lidar_scan_angle.h"

class LidarScan
{
public:
  LidarScan()
  {
    //퍼블리쉬 할 토픽 선언
    pub_ = n_.advertise<lidar_arduino::control>("/topic", 10);

    //섭스크라이브 할 토픽 선언
    sub_ = n_.subscribe("/scan", 1, &LidarScan::scan_callback, this);
  }

  void scan_callback(const sensor_msgs::LaserScan& lidar_angle)
  {
    lidar_arduino::control control_msg;
    // 여기서 너가 원하는대로 값을 만지세요

    control_msg.direction = 10;

    // ...
    pub_.publish(control_msg);
  }

private: //private으로 NodeHandle과 publisher, subscriber를 선언한다.
  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Subscriber sub_;
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "lidar_driver");
  LidarScan SAPObject; //클래스 객체 선을 하게 되면 모든게 된다.
  ros::spin();
  return 0;
}