#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

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
/*
 * The following CONFIGs from prj.conf control the log messages:
 * - CONFIG_MYAPP_LOG_ENABLED 
 *          0: logging messages are disabled. Use Log_Level None
 *          1: logging messages are enabled
 * - CONFIG_MYAPP_LOG_LEVEL 
 *          severity level of the logging messages that are to be output
 */
#if IS_ENABLED(CONFIG_MYAPP_LOG_ENABLED) 
    #define LOG_LEVEL CONFIG_MYAPP_LOG_LEVEL
#else
    #define LOG_LEVEL 0
#endif
LOG_MODULE_REGISTER(MyApp, LOG_LEVEL);

void main(void)
{
    printk("Testing Zephyr Logging!\n");

    LOG_INF("1-This is a test");
    LOG_ERR("1-ERROR: test");
    LOG_WRN("1-WARNING: test");
    LOG_DBG("1-DEBUG: test");
}


/*----- Zephyr Shell Functions -----*/
static int cmd_led_on(const struct shell *shell, size_t argc, char **argv)
{
   ARG_UNUSED(argc);   
   ARG_UNUSED(argv);

   printk("switch on LED0\n");
//   gpio_pin_set_dt(&led,1);
   return 0;
}


static int cmd_led_off(const struct shell *shell, size_t argc, char **argv)
{
    ARG_UNUSED(argc);   
    ARG_UNUSED(argv);

    printk("switch off LED0\n");
//    gpio_pin_set_dt(&led,0);
    return 0;
}

/* Creating subcommands (level 1 command) array for command "led". */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_led,
        SHELL_CMD(on, NULL, "Switch on LED0", cmd_led_on),
        SHELL_CMD(off, NULL, "Switch off LED0", cmd_led_off),
        SHELL_SUBCMD_SET_END
);
/* Creating root (level 0) command "led" */
SHELL_CMD_REGISTER(led, &sub_led, "LED commands", NULL);