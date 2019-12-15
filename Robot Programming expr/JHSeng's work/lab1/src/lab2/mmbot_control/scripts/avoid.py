#!/usr/bin/env python

import rospy
import math
from geometry_msgs.msg import Twist
from sensor_msgs.msg import LaserScan


class Mmbot:
    def __init__(self):
        rospy.init_node('run_avoid', anonymous=True)
        self.velocity_publisher = rospy.Publisher(
            'cmd_vel', Twist, queue_size=5)
        self.pose_subscriber = rospy.Subscriber(
            '/scan', LaserScan, self.update_ranges)
        self.laser_scan = LaserScan()
        self.rate = rospy.Rate(100)

    def update_ranges(self, data):
        print("This in the callback function: ")
        self.laser_scan = data
        print(data.ranges[0])

    def run(self):
        __rate = rospy.Rate(1)
        __rate.sleep()
        currentMessage = Twist()
        currentMessage.linear.x = 0.2
        while not rospy.is_shutdown():
            r = self.laser_scan.ranges
            if (r[2] < 1 or r[3] < 1):
                currentMessage.angular.z = 2
            elif (r[7] < 1 or r[6] < 1):
                currentMessage.angular.z = -2
            else:
                currentMessage.angular.z = 0
            if (r[4] < 1):
                currentMessage.angular.z = 2
            elif (r[5] < 1):
                currentMessage.angular.z = -2
            else:
                currentMessage.angular.z = 0
            self.velocity_publisher.publish(currentMessage)
            self.rate.sleep()


if __name__ == '__main__':
    try:
        mmbot = Mmbot()
        mmbot.run()
    except rospy.ROSInternalException:
        pass
