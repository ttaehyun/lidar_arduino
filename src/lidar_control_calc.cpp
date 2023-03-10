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
    pub_ = n_.advertise<lidar_arduino::control>("ardu_topic", 1);
    //섭스크라이브 할 토픽 선언
    sub_ = n_.subscribe("/scan", 1, &LidarScan::scan_callback, this);
  }

  bool except_zero_bool(int angle_start, int angle_finish, const sensor_msgs::LaserScan& lidar_angle) {
    //1. 임시배열 생성 및 저장
    int tmp_index = angle_finish - angle_start + 1;
    _Float32 tmp_ranges[tmp_index];
    int index_num[] = {};
    float zero_count = 0.0;
    //int all_count = tmp_index;
    
    for (int i = angle_start; i <= angle_finish; i++) {
      tmp_ranges[i-angle_start] = lidar_angle.ranges[i];
    }
    //2. 0.1 보다 작은지 판별후 0.1 판별된 횟수 저장
    for (int i = 0; i<tmp_index; i++) {
      if (tmp_ranges[i] <0.1) {
        zero_count++;
        index_num[i] = i;
      }
      else index_num[i] = tmp_index + 10; //0.1 보다 작은 값이 도출된 인덱스 번호를 저장
    }
    //3. 판별된 횟수가 특정 비율보다 작으면 센서값이 튀었다는 뜻, 그 반대는 정상적인 센서값이다
    float rate_result = zero_count / tmp_index; // tmp_index -> 전체 도출된 값의 개수
    ROS_INFO("zero_count(%d) [%lf]",angle_start, zero_count);
    ROS_INFO("rate_rusult(%d) [%lf]",angle_start, rate_result);
    if (rate_result <0.5) {   //센서값 튀었다 
      for (int i = 0; i< tmp_index; i++) {
        if (index_num[i] != 22) {
          if (tmp_ranges[i] <=0.2) {  //angle_start -> angle_finish 각의 거리 측정값 0.2작은지
            return true;
          }
          else return false;
        }
      }
    }
    else {        //센서값 튀지 않았따
      for (int i = 0; i<tmp_index; i++) {
        if (tmp_ranges[i] <=0.2) {  //angle_start -> angle_finish 각의 거리 측정값 0.2작은지
          return true;
        }
        else return false;
      }
    }
  }

  void scan_callback(const sensor_msgs::LaserScan& lidar_angle)
  {
    
    lidar_arduino::control control_msg;
    control_msg.direction = 60;
    control_msg.esc_motor = 23;
    bool Left_front = except_zero_bool(454,473,lidar_angle); //325도 338도
    bool Right_front = except_zero_bool(31,50,lidar_angle);
    if (Left_front && Right_front){
      control_msg.direction = 60;
      control_msg.esc_motor = 19;
    }
    else if(Left_front) {
      control_msg.direction = 110;
      control_msg.esc_motor = 21;
    }
    else if(Right_front) {
      control_msg.direction = 10;
      control_msg.esc_motor = 21;
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


// Master