#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include "std_msgs/String.h"

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    ROS_INFO_STREAM("Drive robot");
    // Request a service
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv))
        ROS_ERROR("Failed to call service DriveToTarget");

    // TODO: Request a service and pass the velocities to it to drive the robot
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    bool isthere = false;
    int now_pixel = 0;
    // TODO: Loop through each pixel in the image and check if there's a bright white one

    for (int i = 0; i < img.height * img.step; i += 3) {
        if ((img.data[i]==white_pixel)&&(img.data[i+1]==white_pixel)&&(img.data[i+2]==white_pixel)) {
            isthere = true;
            now_pixel = i;
            ROS_INFO_STREAM("FIND IT");
            break;
        }
    }
    // Then, identify if this pixel falls in the left, mid, or right side of the image
     // Depending on the white ball position, call the drive_bot function and pass velocities to it
    if(isthere == true){
    ROS_INFO_STREAM("width"+std::to_string(now_pixel)); 
    int pixel_posi = now_pixel%(img.width*3)/3;
    ROS_INFO_STREAM("posi"+std::to_string(pixel_posi));

    ROS_INFO_STREAM("width"+std::to_string(img.width)); 
    int first_posi = (img.width/3);
    ROS_INFO_STREAM("first"+std::to_string(first_posi));
    int second_posi = (img.width*2/3);
    ROS_INFO_STREAM("second"+std::to_string(second_posi));
    if(pixel_posi<(first_posi)){
    drive_robot(0,-1);
    ROS_INFO_STREAM("LEFT");
    }
    if(pixel_posi>(second_posi)){
    drive_robot(0,1);
    ROS_INFO_STREAM("RIGHT");
    }
    if((pixel_posi>(first_posi))&&(pixel_posi<(second_posi))){
    drive_robot(1,0);
    ROS_INFO_STREAM("Go");
    }
    
    }
    else{
    drive_robot(0,0);
    ROS_INFO_STREAM("STOP"); 
    
    }   

    // Request a stop when there's no white ball seen by the camera
    
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
