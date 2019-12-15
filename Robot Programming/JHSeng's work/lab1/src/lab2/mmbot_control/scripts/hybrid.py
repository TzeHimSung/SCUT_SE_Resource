#!/usr/bin/env python

import rospy
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist
from math import pow, atan2, sqrt
from tf.transformations import euler_from_quaternion
from sensor_msgs.msg import LaserScan


class Mmbot:

    def __init__(self):

        rospy.init_node('goto', anonymous=True)

        self.velocity_publisher = rospy.Publisher(
            'cmd_vel', Twist, queue_size=5)

        self.pose_subscriber = rospy.Subscriber(
            '/odom', Odometry, self.update_pose)

        self.laser_subscriber = rospy.Subscriber(
            '/scan', LaserScan, self.update_ranges)

        self.odometry = Odometry()

        self.laser_scan = LaserScan()

        self.rate = rospy.Rate(1)

    def update_ranges(self, data):

        self.laser_scan = data

    def update_pose(self, data):

        self.odometry = data
        self.odometry.pose.pose.position.x = round(
            self.odometry.pose.pose.position.x, 4)
        self.odometry.pose.pose.position.y = round(
            self.odometry.pose.pose.position.y, 4)

    def euclidean_distance(self, goal_pose):
        return sqrt(pow((goal_pose.pose.pose.position.x - self.odometry.pose.pose.position.x), 2) +
                    pow((goal_pose. pose.pose.position.y - self.odometry.pose.pose.position.y), 2))

    def linear_vel(self, goal_pose, constant=0.2):
        return constant * self.euclidean_distance(goal_pose)

    def steering_angle(self, goal_pose):
        return atan2(goal_pose.pose.pose.position.y - self.odometry.pose.pose.position.y, goal_pose.pose.pose.position.x - self.odometry.pose.pose.position.x)

    def angular_vel(self, goal_pose, constant=1):
        p = euler_from_quaternion((self.odometry.pose.pose.orientation.x,
                                   self.odometry.pose.pose.orientation.y,
                                   self.odometry.pose.pose.orientation.z,
                                   self.odometry.pose.pose.orientation.w))

        print constant * (self.steering_angle(goal_pose) - p[2])
        return constant * (self.steering_angle(goal_pose) - p[2])

    def move2goal(self):
        self.rate.sleep()
        goal_pose = Odometry()
        goal_pose.pose.pose.position.x = input("Set your x goal: ")
        goal_pose.pose.pose.position.y = input("Set your y goal: ")
        distance_tolerance = input("Set your tolerance: ")
        currentMessage = Twist()
        while self.euclidean_distance(goal_pose) >= distance_tolerance:
            __range = self.laser_scan.ranges
            currentMessage.linear.x = self.linear_vel(goal_pose)
            currentMessage.linear.y = 0
            currentMessage.linear.z = 0
            currentMessage.angular.x = 0
            currentMessage.angular.y = 0
            currentMessage.angular.z = -self.angular_vel(goal_pose)
            if (__range[4] < 1):
                currentMessage.angular.z = 2
            elif(__range[5] < 1):
                currentMessage.angular.z = -2
            self.velocity_publisher.publish(currentMessage)
            self.rate.sleep()
        currentMessage.linear.x = 0
        currentMessage.angular.z = 0
        self.velocity_publisher.publish(currentMessage)
        rospy.spin()


if __name__ == '__main__':
    try:
        mmbot = Mmbot()
        mmbot.move2goal()
    except rospy.ROSInterruptException:
        pass
