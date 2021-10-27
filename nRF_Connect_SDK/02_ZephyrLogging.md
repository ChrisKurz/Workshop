# Logging

## Introduction

Zephyr consists of very many software modules, which can be added individually to the own project. The adding is done with the help of CONFIG settings. 
So you can use config - or let's call it kconfig - to add the different Zephyr software modules into your own project. 

In this chapter we will look at the logging library, which is included in Zephyr. The implemented logging solution is very extensive. 
So here we will only look at the very basic functionality. 

In previous chapter we have already seen a simple way to send a message via UART; we used the printk instruction. This is a simple method, but Zephyr Logging offers more functionality. 
Logging is very common in the Zephyr project. It is used in its software modules and it can be individually be enabled. These debug messages can consist of text strings or data like measurement results or content of varialbes. 

The Zephyr Logging allows debugging during runtime, which is very useful for runtime critical applications like RTOS or debugging Bluetooth connections.
And it can also be used for your own application. We will have a closer look at this in the next example project.

## Using Zephyr Logging in an own project

1) Create a new project folder.  For example,   c:/MyWorkspace/Logging
2) Add CMakeLists.txt, prj.conf, and main.c.

    _CMakeLists.txt_
    
       cmake_minimum_required(VERSION 3.13.4)

       # Find external Zephyr project, and load its settings:
       find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

       # Set project name:
       project (MyApp)

       # add sources
       target_sources(app 
           PRIVATE 
              src/main.c)

   _prj.conf_
   
       # Enable Serial 
       CONFIG_SERIAL=y
   
       # Enable Logging
       CONFIG_LOG=y
   
   __NOTE: When setting a CONFIG value ensure that there are no spaces around the equal sign!__
   
   _src/main.c_
   
       #include <zephyr.h>

       void main(void)
       {
	         printk("Testing Zephyr Logging!\n");
       }
  
  Note: The Zephyr Logging software module was added to the project by enabling CONFIG_LOG. The next steps are to enable the logging in our application software.
   
3) Start Segger Embedded Studio and open the Logging project.
4) We need to include the logging header file to get access to the logging functionality. This is done by adding following line in main.c:

       #include <logging/log.h>
5) Before we can use the logging instructions, we have to register the logging for this new software module (the new software module is our own application). This is done by adding following lines before the main function:

       /** LOG MODULE REGISTRATION:
           - First parameter: module name
           - Second Parameter (optional): log level
                Following Log-Levels can be used:
                    LOG_LEVEL_NONE  => OFF:     log will not be active. 
                    LOG_LEVEL_ERR   => ERROR:   write only LOG_ERR messages
                    LOG_LEVEL_WRN   => WARNING: write LOG_WRN and LOG_ERR messages
                    LOG_LEVEL_INF   => INFO:    write LOG_INF, LOG_WRN, and LOG_ERR messages
                    LOG_LEVEL_DBG   => DEBUG:   write LOG_DBF, LOG_INF, LOG_WRN, and LOG_ERR messages
       **/
       LOG_MODULE_REGISTER(MyApp,LOG_LEVEL_WRN);
6) Now we are read to create debug outputs in our own application. For test purpose we add the following lines in our main function:

       void main(void)
       {
           printk("Testing Zephyr Logging!\n");

           LOG_INF("1-This is a test");
           LOG_ERR("1-ERROR: test");
           LOG_WRN("1-WARNING: test");
           LOG_DBG("1-DEBUG: test");
       }
7) Build the project, download the code to a development kit, and check the output via the virtual COM port (Settings: 115200 baud, 8 data bits, 1 stop bit, no partiy, no flow control).
8) Change the log level in the LOG_MODULE_REGISTER instruction and check the debug messages seen on your computer terminal. For example, you can use "nRF Terminal" extension within Visual Studio Code (see previous chapter).       
 

## Using RTT backend for Logging

The debug messages can we sent via different interfaces or also called backends. Zephyr allows to select UART or RTT. By default both backends are enabled. Let's try to use the RTT backend. 

9) Open command palette (Ctrl+Shift+P) and enter "nRF Connect: Connect to RTT in nRF Terminal". Instead of using command palette, you can also click the play icon in the CONNECTED DEVICES tab. Note that the play icon behind the serial number and the device name will open nRF Terminal using RTT, while the play icon behind the VCOM line opens nRF Terminal using UART. (move mouse on play icon and wait. Context helpd should open.)

10) After selecting "nRF Connect: Connect to RTT in nRF Terminal" you have to enter a few parameters:
- "Enter device": use the device name here. There is a certain nomenclature. In case you enter an unknown device name, you will get a list of valid device names. For example, enter "nRF52833_xxAA".
- "Enter serial number": this is the number that is shown in the CONNECTED DEVICES tab. You find this number also on the sticker on the board controller of your development kit.
- "Choose memory address strategy": us "Automatic search" here.

11) Now, the terminal should run and all RTT messages that are sent via Debug Interface are shown. You are seeing messages, because the default setting in your project is enabled UART and RTT. 
12) Let's change the default settings and do some testing (using UART or RTT for nRF Terminal and enable/disable Logging backends). The Logging backends can be changed by clicking on "Guiconfig". Search for possible "Logging" configs by clicking on "Jumpt to...". Or open directly the folder __Sub Systems and OS Services / Logging / Backends__. Enable/disable the "Enable UART backen" or "Enable Segger J-Link RTT backen". After changing the settings press "Save" button. Note the "Configuration saved to ..." message in the status bar. The settings are stored in the build directory. Take care, because doing a "Pristine build" will erase this folder before it is build again. So using this way means you should only use the "Build" in the ACTIONS tab. 
13) Don't forget to flash the build project before checking the output in the nRF Terminal.


## Minimal Logging 

Let’s try another feature of the logger module.

The CONFIG_LOG_MINIMAL enable minimal logging implementation. This is very useful when you have very limited memory, and you are NOT in need for 
the deferred feature of the logger. It has very little footprint overhead on top of the printk() implementation for standard logging macros. 
Hexdump macros are also supported, with a small amount of code pulled in if used. Build time filtering is supported, but not runtime filtering. 
There are no timestamps, prefixes, colors, or asynchronous logging, and all messages are processed in-place which simply means it is sent to the 
console just like we did in the printk().

Once CONFIG_LOG_MINIMAL is enabled, most other logger configurations will be unavailable (will have no effect!).

Add the configuration line below into the application configuration file prj.conf: 

    CONFIG_LOG_MINIMAL=y

14) Build your project.
15) Informational step! Let’s confirm the new configurations by opening the graphical configuration interface (guiconfig). Look into __Sub Systems and OS Services / Logging__ and check if "Enable minimal-footpring looking" is enabled. Close guiconfig.
16) Flash the project to your board and check the console output.

You should notice that the output of the logger module is significantly different when the minimal mode is activated. Log levels are now indicated by letters (D=Debug, I = Info, W=Warning, E=Error). Timestamps and prefixing module name are no longer available in minimal mode.
