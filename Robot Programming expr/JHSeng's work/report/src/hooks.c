// 库作者打算使用此函数来构建支持协作线程的库或草图。
// 它定义为弱符号，可以重新定义以实现真正的协作调度程序。
static void __empty() {
}
void yield(void) __attribute__ ((weak, alias("__empty")));
