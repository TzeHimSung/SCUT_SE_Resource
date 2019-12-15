// c++ basic header
#include <iostream>
// ros basic header
#include <ros/ros.h>
#include <std_srvs/Empty.h>
#include <geometry_msgs/Twist.h>
// turtlesim node header
#include <turtlesim/Pose.h>
#include <turtlesim/Color.h>

geometry_msgs::Twist msg;

// print message information to stdIO
void printInfo() {
    ROS_INFO_STREAM(std::setprecision(2) << std::fixed << "msg.linear.x = " << msg.linear.x << " , msg.angular.z = " << msg.angular.z);
}

// let turtle go straight
void goForward(ros::Publisher &move_publisher) {
    for (int i = 1; i <= 50000; i++) {
        msg.linear.x = 1;
        msg.angular.z = 0;
        move_publisher.publish(msg);
        printInfo();
    }
}

// let turtle draw a ractangle
void drawRectangle(ros::Publisher &move_publisher) {
    goForward(move_publisher);
    // the limit of i is not equal
    for (int i = 1; i <= 42325; i++) {
        msg.linear.x = 0;
        msg.angular.z = 1;
        move_publisher.publish(msg);
        printInfo();
    }
    goForward(move_publisher);
    for (int i = 1; i <= 42310; i++) {
        msg.linear.x = 0;
        msg.angular.z = 1;
        move_publisher.publish(msg);
        printInfo();
    }
    goForward(move_publisher);
    for (int i = 1; i <= 42200; i++) {
        msg.linear.x = 0;
        msg.angular.z = 1;
        move_publisher.publish(msg);
        printInfo();
    }
    goForward(move_publisher);
    for (int i = 1; i <= 42190; i++) {
        msg.linear.x = 0;
        msg.angular.z = 1;
        move_publisher.publish(msg);
        printInfo();
    }
    for (int i = 1; i <= 20000; i++) {
        msg.linear.x = 1;
        msg.angular.z = 0;
        move_publisher.publish(msg);
        printInfo();
    }
}

// change background color of the program
void changeBackgroundColor(ros::NodeHandle &nodeHandle) {
    // rand a color
    int Red = 255 * double(rand()) / double(RAND_MAX);
    int Green = 255 * double(rand()) / double(RAND_MAX);
    int Bule = 255 * double(rand()) / double(RAND_MAX);
    // set param of background
    ros::param::set("background_r", Red);
    ros::param::set("background_g", Green);
    ros::param::set("background_b", Bule);
    // make a service client
    ros::ServiceClient clearClient = nodeHandle.serviceClient<std_srvs::Empty>("/clear");
    std_srvs::Empty srv;
    clearClient.call(srv);
}

// let turtle draw a circle and change background color within some time
void drawCircleAndChangeColor(ros::NodeHandle &nodeHandle, ros::Publisher &move_publisher) {
    for (int i = 1; i <= 150000; i++) {
        // judge change background color
        if (i % 30000 == 0) changeBackgroundColor(nodeHandle);
        msg.linear.x = 1;
        msg.angular.z = 1;
        move_publisher.publish(msg);
        printInfo();
    }
}

// let turtle draw a circle
void drawCircle(ros::Publisher &move_publisher) {
    for (int i = 1; i <= 150000; i++) {
        msg.linear.x = 1;
        msg.angular.z = 1;
        move_publisher.publish(msg);
        printInfo();
    }
}

// main
int main(int argc, char **argv) {
    // ros init
    ros::init(argc, argv, "main");
    ros::NodeHandle nodeHandle;
    srand(time(0));

    // publish signal to topic
    ros::Publisher move_publisher = nodeHandle.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1000);

    // draw ractangle
    drawRectangle(move_publisher);

    // draw a circle
    drawCircle(move_publisher);

    // draw a circle with background color changed
    while (ros::ok()) {
        drawCircleAndChangeColor(nodeHandle, move_publisher);
        rate.sleep();
    }
    return 0;
}