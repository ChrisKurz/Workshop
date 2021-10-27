# Adding Device Tree support for LEDs (from scratch)

1) copy hello_world project (../zephyr/samples/hello_world) into your own workspace folder.
2) Open the project in Visual Studio Code and "Add Build Configuration" (select your nRF52 dev kit board)
3) Define the symbol that will represent the LED on the board. This is done by calling the appropriate devicetree APIs. In main.c , copy and paste the following lines as shown in the illustration below:

       #include <devicetree.h>
       #include <device.h>
       #include <drivers/gpio.h>

       /* The devicetree node identifier for the "led0" alias. */
       #define LED0_NODE DT_ALIAS(led0)

       #if DT_NODE_HAS_STATUS(LED0_NODE, okay)
       #define LED0   DT_GPIO_LABEL(LED0_NODE, gpios)
       #define PIN    DT_GPIO_PIN(LED0_NODE, gpios)
       #define FLAGS  DT_GPIO_FLAGS(LED0_NODE, gpios)
       #else
       /* A build error here means your board isn't set up to blink an LED. */
       #error "Unsupported board: led0 devicetree alias is not defined"
       #define LED0   ""
       #define PIN    0
       #define FLAGS  0
       #endif

4) In main function we have to add following lines: 

       const struct device *dev;
    
       dev = device_get_binding(LED0);   
       if (dev == NULL) {
          return;   
       }

5) Now we use the symbols that were read from DTS file to configure the GPIO module:

       int ret;

       ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);   
       if (ret < 0) {   
          return;  
       }

6) and we add an entire loop in main function:

       while (1) {  
          gpio_pin_set(dev, PIN, (int)led_is_on);  
          led_is_on = !led_is_on;
          k_msleep(SLEEP_TIME_MS); 
       }
