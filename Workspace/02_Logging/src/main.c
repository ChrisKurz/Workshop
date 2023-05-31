#include <zephyr/kernel.h>
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
LOG_MODULE_REGISTER(MyApp,LOG_LEVEL_WRN);

void main(void)
{
    printk("Testing Zephyr Logging!\n");

    LOG_INF("1-This is a test");
    LOG_ERR("1-ERROR: test");
    LOG_WRN("1-WARNING: test");
    LOG_DBG("1-DEBUG: test");
}