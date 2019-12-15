#!/usr/bin/env python
# Copyright(c) 2017, SCUT RIS
# author : Jinhui Zhu

import rospy

from nav_msgs.msg import Odometry


def odom_callback(data):
    rx = data.pose.pose.position.x
    ry = data.pose.pose.position.y
    rospy.loginfo(rospy.get_caller_id()+": %.2f %.2f", rx, ry)


if __name__ == "__main__":
    rospy.init_node("where")
    sub = rospy.Subscriber('/odom', Odometry, odom_callback)
    rospy.spin()
