# Make your own Board Definition

Visual Studio Code allows to create a new board. 
Let's create a new board.

1) Start Visual Studio Code and open the welcome page. 
2) Click on "Create a new board..."
3) Enter all fields:
   - name (e.g. "MyBoard"). 
   - Notice that the Board ID is derived from the entered name. 
   - Select the SoC. 
   - Enter a vendor name.
   - Select a root folder for the board (e.g. c:/MyWorkspace/blinky). Note that within this folder a "boards" folder will be created. 
4) click "Create Board" button
5) Use a file explorer to take a closer look into the generated files. You should see following files:
   - board.cmake
   - Kconfig.board
   - Kconfig.defconfig
   - myboard.dts
   - myboard.yaml
   - myboard_defconfig.

6) copy the blinky project (../zephyr/samples/basic/blinky) and open it. Select the new created board for this project.
7) build the project. There will be an error. What is causing the error?
8) Check if the GPIO device driver is included in the project. Why is it or is it not included?
9) Add the GPIO device driver enabling to your board definition (myboard_defconfig.) and remove enabling of GPIO driver in prj.conf.
10) Build the project and check if GPIO device driver is enabled. 
11) Let's add the following lines to myboards.dts file. Do this within Visual Studio Code!

        / {
            ...
            
               leds {
                    compatible = "gpio-leds";
                    led0: led_0 {
                                 gpios = <&gpio0 13 GPIO_ACTIVE_LOW>;
                                 label = "Green LED 0";
                                };

               aliases {
                        led0 = &led0;
                       };
          
          ...
          };
        
12) Move the mouse-pointer over the "&gpio0" name in the line  "gptios = <&gpio0 13 GPIO_ACTIVE_LOW>;". What is the setting for "status"?
13) Add following lines in myboards.dts (outside the tree!):

          &gpio0 {
	               status = "okay";
          };

14) Move the mouse-pointer again over the "&gpio0" name. What is the setting for "status" now?
15) build and flash the project. Test it on your development kit.
16) Instead of LED0 toggle LED1. 
   
