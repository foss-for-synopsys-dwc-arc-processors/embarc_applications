/* -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


* File Name : writeFtdi.cpp

* Purpose :

* Creation Date : 2018-05-12

* Last Modified : Thu 17 May 2018 09:00:14 PM CST

* Created By :  

*_._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._._.*/

#include <ros/ros.h>
#include <ftdi.h>
#include <skeleton_markers/Skeleton.h>
#include <vector>

struct ftdi_context *ftdi;
int vid = 0x403;
int pid = 0x6011;
int baudrate = 115200;
int interface = INTERFACE_A;


void cb_ftdi(const skeleton_markers::SkeletonConstPtr msg)
{
  static int test_flag = 0;
  static int print_flag = 0;
  // std::cout << "entry ftdi callback" << std::endl;
  float ftdi_sent[(3+4)*15];

  // decode msg
  // if user id is 1
  // TODO reference confidence

  if (test_flag%2 == 0)
  {
    for (int i = 0; i < 15; ++i) 
    {
      // ftdi_sent[7*i+0] = msg->position.at(i).x;
      // ftdi_sent[7*i+1] = msg->position.at(i).y;
      // ftdi_sent[7*i+2] = msg->position.at(i).z;
      // ftdi_sent[7*i+3] = msg->orientation.at(i).w;
      // ftdi_sent[7*i+4] = msg->orientation.at(i).x;
      // ftdi_sent[7*i+5] = msg->orientation.at(i).y;
      // ftdi_sent[7*i+6] = msg->orientation.at(i).z;
      ftdi_sent[7*i+0] = msg->position.at(i).x;
      ftdi_sent[7*i+1] = msg->position.at(i).y;
      ftdi_sent[7*i+2] = msg->position.at(i).z;
      ftdi_sent[7*i+3] = msg->orientation.at(i).w;
      ftdi_sent[7*i+4] = msg->orientation.at(i).x;
      ftdi_sent[7*i+5] = msg->orientation.at(i).y;
      ftdi_sent[7*i+6] = msg->orientation.at(i).z;
    }

    ftdi_write_data(ftdi, (unsigned char*)ftdi_sent, sizeof(ftdi_sent));

    if (print_flag % 10 == 0) {
      // fdti
      std::cout << "sent size" << sizeof(ftdi_sent) << std::endl;
      std::cout << "===========================================" << std::endl;
      std::cout << "send msg index 0" << std::endl;
      // std::cout << ftdi_sent[0] << " " << ftdi_sent[1] << " " << ftdi_sent[2] << std::endl;
      std::cout << ftdi_sent[3] << " " << ftdi_sent[4] << " " << ftdi_sent[5] << " " << ftdi_sent[6] << std::endl;

      std::cout << ftdi_sent[10] << " " << ftdi_sent[11] << " " << ftdi_sent[12] << " " << ftdi_sent[13] << std::endl;

      std::cout << ftdi_sent[17] << " " << ftdi_sent[18] << " " << ftdi_sent[19] << " " << ftdi_sent[20] << std::endl;

      // std::cout << ftdi_sent[98] << " " << ftdi_sent[99] << " " << ftdi_sent[100] << std::endl;
      std::cout << ftdi_sent[101] << " " << ftdi_sent[102] << " " << ftdi_sent[103] << " " << ftdi_sent[104] << std::endl;
      std::cout << "===========================================" << std::endl;
      print_flag = 0;
    }
    test_flag = 0;
  }
  test_flag++;
  print_flag++;
}

int main(int argc, char *argv[])
{
  int f;
  // ftdi init
  if ((ftdi = ftdi_new()) == 0) 
  {
    std::cout << "ftdi_new failed" << std::endl;
    return -1;
  }

  ftdi_set_interface(ftdi, INTERFACE_A);
  f = ftdi_usb_open_desc(ftdi, vid, pid, "vlsilab", NULL);
  if (f < 0) {
    std::cout << "error" << ftdi_get_error_string(ftdi) << std::endl;
    return -1;
  }

  f = ftdi_set_baudrate(ftdi, baudrate);
  if (f < 0) {
    std::cout << "error" << ftdi_get_error_string(ftdi) << std::endl;
    return -1;
  }

  // ros init
  ros::init(argc, argv, "usb_ftdi");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/skeleton", 5, cb_ftdi);

  ros::spin();
  return 0;
}
