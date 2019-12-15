# 《机器人基础编程》课程报告任务书

# 1. 目的

1）锻炼源代码阅读能力

2）锻炼软件分析能力

3）锻炼文档编写能力



# 2. 任务内容

选择以下一个机器人相关软件，阅读和分析其源代码，编写出系统分析报告，阐述系统结构和实现原理。

1）ROS系统

分析ROS系统的核心框架和实现。

ROS的核心通信库源代码 https://github.com/ros/ros_comm.git



2）Arduino软件

分析Arduino UNO板的实现原理。

以 arduino-1.8.10软件版本为例，重点分析 arduino-1.8.10/hardware/arduino/avr/cores/arduino下的文件。



3）Raspberry Pi的linux内核移植

分析Linux通用内核如何移植到Raspberry Pi，部分驱动如何实现。

代码地址 https://github.com/raspberrypi/linux



4）Raspberry Pi的WiringPi-Python库

分析WringPi库（C语言）如何在linux环境下操作硬件； 

分析WringPi-python如何依赖WringPi库，实现python操作硬件。

代码地址 https://github.com/WiringPi/WiringPi-Python



# 3. 文档要求

1）文档模板参考实验报告模板。
封面的标题改为：**《机器人编程基础》课程报告**。
**部分核心源代码和报告一起提交**。

2）文档以“**图文表**”展示分析过程和结果。尽量用“图”展示软件架构模型、函数调用过程、流程图、类图等，用“表格”展示核心代码，用“文”对图和表进行文字描述。



