#include <cstdio>
#include <dynamixel_workbench_toolbox/dynamixel_workbench.h>


#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class MinimalDinamixelDriveNode : public rclcpp::Node
{
  public:
    MinimalDinamixelDriveNode()
    : Node("minimal_dynamixel_drive_node")
    {
      const char* log = NULL;
      bool result = false;

      dynamixel_workbench_ = new DynamixelWorkbench;
      // result = dynamixel_workbench_->init("/dev/ttyUSB0", 57600, &log);
      result = dynamixel_workbench_->init("/dev/dxhub", 57600, &log);
      if (result == false )
      {
        RCLCPP_ERROR(this->get_logger(),
         "error init \n %s",log);
        exit(-1);
      }
      RCLCPP_INFO(this->get_logger(), "%s\n",log);

      uint16_t model_number = 0;

      for( int dx_id = 1; dx_id <= 2; dx_id++){
        result = dynamixel_workbench_->ping(dx_id,&model_number, &log);
        if (result == false)
        {
          printf("%s\n", log);
          printf("Failed to ping\n");
        }
        else
        {
          printf("Succeed to ping\n");
          printf("id : %d, model_number : %d\n", dx_id, model_number);
        }


        RCLCPP_INFO(this->get_logger(),"wheelmode ! id %d",dx_id);
        //result = dynamixel_workbench_->torqueOn(1,&log);
        result = dynamixel_workbench_->wheelMode(dx_id, 0, &log);
        RCLCPP_INFO(this->get_logger(),"wheelmode ? id %d",dx_id);
        if (result == false )
        {
          RCLCPP_ERROR(this->get_logger(), "error for torqueOn \n%s",log);
          exit(-1);
        }
        RCLCPP_INFO(this->get_logger(), "id %d Succeeed  to set up %s", dx_id, log);
      }

      RCLCPP_INFO(this->get_logger(),"fin setup");

      subscription_ = this->create_subscription<std_msgs::msg::String>(
        "sanehal",
        10,
        [this](std_msgs::msg::String::UniquePtr msg){
          //const char* log_lamda = NULL;
          RCLCPP_INFO(this->get_logger(),"I heard: '%s'", msg->data.c_str());
          // 適当に速度ONにする
          this->dynamixel_workbench_->goalVelocity(1, (int32_t)-100);
          this->dynamixel_workbench_->goalVelocity(2, (int32_t)100);

          sleep(1);
          this->dynamixel_workbench_->goalVelocity(1, (int32_t)100);
          this->dynamixel_workbench_->goalVelocity(2, (int32_t)-100);

          sleep(1);
          // 速度off
          this->dynamixel_workbench_->goalVelocity(1, (int32_t)0);
          this->dynamixel_workbench_->goalVelocity(2, (int32_t)0);

        });
    }
    ~MinimalDinamixelDriveNode(){
      delete dynamixel_workbench_;
    }

  private:
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_ ;
    DynamixelWorkbench *dynamixel_workbench_ ;
};


int main(int argc, char ** argv)
{
  printf("start dx_practice_pkg package\n");

  rclcpp::init(argc, argv);
  printf("spin node \n");
  rclcpp::spin(std::make_shared<MinimalDinamixelDriveNode>());
  rclcpp::shutdown();
  return 0;
}
