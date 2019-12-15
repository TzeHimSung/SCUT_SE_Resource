#!/usr/bin/env python

import rospy

from geometry_msgs.msg import Twist


def solve():
    rospy.init_node('test')
    currentMessage = Twist()
    messagePublisher = rospy.Publisher("/cmd_vel", Twist, queue_size=1000)
    currentMessage.linear.x = 0.2
    rate = rospy.Rate(100)
    while not rospy.is_shutdown():
        messagePublisher.publish(currentMessage)
        rospy.loginfo("published")
        rate.sleep()


if __name__ == '__main__':
    try:
        solve()
    except rospy.ROSInterruptException:
        pass
