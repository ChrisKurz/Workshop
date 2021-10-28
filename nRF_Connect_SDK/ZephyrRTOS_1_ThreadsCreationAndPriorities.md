# 1. Threads creation and priorities

In this exercise, we will learn about threads creation and how they can affect one another with their priorities.

Download the exercise file ZephyrRTOS_1.zip and extract it somewhere on your hard drive. Open the project by Selecting "Add an existing application" in nRF Connect for VS Code.

Please remember to select the correct board you are using while configuring the project options as done in previous exercises. The main.c looks like below. There are only 
threads and no main() function which is OK as CONFIG_MULTITHREADING is enabled by default by the nRF Connect SDK.

![](images/ZephyrRtos-1.jpg)

1. This is where you find the main.c file in the IDE.
2. K_THREAD_DEFINE is the macro for declaring a thread and plugging its data structures into the RTOS kernel. Here you can see that we have created the two threads named thread0 and thread1.
3. Thread priority used in this exercise. We use the same priority here to start with to demonstrate few features.
4. Stack size for the threads. Even though the threads are simple in this exercise we are using 1K stack size generously. In actual application development, you need to benchmark the threads stack usage if the free stack size is getting too tight. We do not need that here for a simple application like this.
5. Definitions of the threads. They do nothing but loop for ever printing their name to the serial terminal. Important thing to note here is that both the threads have no dependency on each other and they do not yield or sleep, so they will always be in the ready state (not blocked) competing for the CPU resource.

K_THREAD_DEFNE API description:
https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/reference/kernel/threads/index.html#c.K_THREAD_DEFINE

Open any of the serial terminal program and connect to your device. Now go to the nRF Connect Extension (Ctrl + Alt + N), Build (Ctrl + Alt + B) and Flash(Ctrl + Alt +F) your application or you can use below options as well.

![](images/ZephyrRtos-2.jpg)

After you have flashed the firmware, you can see in the terminal program the following output:

![](images/ZephyrRtos-3.jpg)

If you have observed the output of the terminal program, you noticed that it is only thread0 that is running and thread1 never gets to run ever (starved) even though both threads are created with same parameters and priority. The second thread is always starving since thread0 has been selected by the scheduler but this thread0 never yields, blocks or waits on anything.

Let’s see how we can fix this.

## 1.1 Thread yielding
In the previous exercise, the equal priority thread0 is always starving thread1 because thread0 never yields, so in this exercise we will make the thread0 to yield (voluntarily) using k_yield(). A thread normally yields when it either has nothing else to do and wants to give other equal/higher priority threads a chance to run (when time slicing size is limited, you will know more about this in next exercise), so there is always some logic behind when a thread wants to call k_yield(). But to keep this exercise simple, let us make thread0 yield every time after it finishes with the printk message. Change the thread0 definition to below

      void thread0(void)
      {
	      while (1) {
                printk("Hello, I am thread0\n");

                // I'll be nice to other lower or equal priority threads and will yield.
                k_yield();
        }
      }

We just added one line of code here which is k_yield() after printk is processed. Build, Flash and check what you see in the terminal emulator now. It should be something like below.

![](images/ZephyrRtos-4.jpg)

You will see that thread0 is able print one message and yields voluntarily to equal or higher priority threads. Since there is another equal priority thread (thread1) which is ready to get the CPU, this thread1 is now made active by the scheduler and will get the CPU time (executed on the CPU). One interesting thing to notice here is that once thread1 gets active, it runs for ever now starving thread0. This is because even though thread0 cooperatively yielded to thread1, thread1 is now not yielding, hence thread0 is starved indefinitely in this case once thread1 gets active (running). So let us now make thread1 yield as well by adding k_yield() after it prints its message like below.

      void thread1(void)
      {
          while (1) {
              printk("Hello, I am thread1\n");

              // I'll be nice to other lower of equal priority threads and will yield.
              k_yield();
          }   
      }

Build, Flash and check what you see in the terminal emulator now. It should be something like below.

![](images/ZephyrRtos-5.jpg)

And the events on the CPU should look something like below

![](images/ZephyrRtos-6.jpg)

Here one can notice that both threads now yield after printing a message making the scheduler come into the scene and evaluate to see if there are any runnable threads in the queue. The good thing here is that both threads of equal priorities are cooperating, but the disadvantage here could be that each thread calls k_yield() too often invoking the scheduler. Scheduler also needs CPU time to do the book keeping of the kernel resources every time k_yield() is called (remember Rescheduling point section) which will in turn cost power. Good architecture of your system is to design your threads in a way where scheduler takes very little CPU time and gives most of the CPU time to the threads designed with correct priorities and are reasonably considerate (yielding/sleeping/waiting) to other threads. In our example, the scheduler overhead to make other thread active can be more than the overhead of the simple thread just printing a message which is not very power efficient. The better option is from the threads to sleep since it is acceptable for our threads to print less frequently. Sleeping is more power efficient for the threads that wake up, do very little processing and can wait for sometime to repeat this cycle.

## 1.2 Thread sleeping
There are many ways to make the thread relinquish CPU by making it go to “Waiting” state instead of “Ready” and one of them is using k_sleep or the variation of it like k_msleep(). After you finished the previous part of the exercise with thread yielding, replace the k_yield() in both the threads with k_msleep() like below with sleep duration of say 5ms.



