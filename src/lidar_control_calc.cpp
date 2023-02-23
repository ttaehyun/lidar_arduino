#include <ros/ros.h>
#include "sensor_msgs/LaserScan.h"
#include "lidar_arduino/control.h"
#include "std_msgs/Int64.h"
class LidarScan
{
public:
  LidarScan()
  {
    //퍼블리쉬 할 토픽 선언
    pub_ = n_.advertise<std_msgs::Int64>("ardu_topic", 1);

    //섭스크라이브 할 토픽 선언
    sub_ = n_.subscribe("/scan", 1, &LidarScan::scan_callback, this);
  }
  std_msgs::Int64 except_zero_calc(int angle_start, int angle_finish, const sensor_msgs::LaserScan& lidar_angle) {
    std_msgs::Int64 control_msg;
    control_msg.data = 60;
    //1. 임시배열 생성 및 저장
    _Float32 tmp_ranges[15];
    int index_num[] = {};
    float zero_count = 0.0;
    int all_count = 15;
    for (int i = angle_start; i <= angle_finish; i++) {
      tmp_ranges[i-angle_start] = lidar_angle.ranges[i];
    }
    //2. 0.1 보다 작은지 판별후 0.1 판별된 횟수 저장
    for (int i = 0; i<15; i++) {
      if (tmp_ranges[i] <0.1) {
        zero_count++;
        index_num[i] = i;
      }
      else index_num[i] = 22;
    }
    //3. 판별된 횟수가 특정 비율보다 작으면 센서값이 튀었다는 뜻, 그 반대는 정상적인 센서값이다
    float rate_result = zero_count / all_count;
    if (rate_result <0.5) {   //센서값 튀었다 
      for (int i = 0; i< 15; i++) {
        if (index_num[i] != 22) {
          if (tmp_ranges[i] <=0.2) {
            if (angle_start>252) {
              control_msg.data =110;
            }
            else if (angle_start<=252) {
              control_msg.data = 10;
            }
          }
          else control_msg.data = 60;
        }
      }
    }
    else {        //센서값 튀지 않았따
      for (int i = 0; i<15; i++) {
        if (tmp_ranges[i] <=0.2) {
          if (angle_start>252) {
              control_msg.data =110;
            }
            else if (angle_start<=252) {
              control_msg.data = 10;
            }
        }
        else control_msg.data = 60;
      }
    }
    ROS_INFO("zero_count(%d) [%lf]",angle_start, zero_count);
    ROS_INFO("rate_rusult(%d) [%lf]",angle_start, rate_result);
    return control_msg;
  }
  void scan_callback(const sensor_msgs::LaserScan& lidar_angle)
  {
    
    std_msgs::Int64 control_msg;
    control_msg.data = 60;

    if (control_msg.data == 60) {
      control_msg = except_zero_calc(14,28,lidar_angle);
    }
    if (control_msg.data == 60) {
      control_msg = except_zero_calc(476, 490, lidar_angle);
    }
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