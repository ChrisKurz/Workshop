# Defining own KCONFIG symbols


## Introduction

In the previous chapter we had looked at using Zephyr logging. We had seen there how in C-source code files the logger can be activated and the log level can be set.
These settings - logging activation and log level selection - were made in the C source file. However, it would also be interesting to make these settings in the prj.conf file via user-defined KCONFIG switches. 

This is exactly what we will look at in this chapter. 


## Defining own CONFIG symbols 

1) Make a copy of the project from the previous chapter.
2) We want to define own CONIFG symbols. These symbols are:

     _CONFIG_MYAPP_LOG_ENABLE_: 
     This should be a boolean, that activates or deactivates logging. 
     
     _CONFIG_MYAPP_LOG_LEVEL_: 
     You have already seen that the Zephyr Logging allows to define different Log Levels. This user defined symbol is used to allow setting the log level. 

     Defining of these symbols is done in a kconfig. (blank extension!) file. So let's add the following kconfig. file to our project folder:

     _kconfig._
  
       # MyApp configuration
       menu "MyApp Settings"
          menu "Logging"
             config MYAPP_LOG_ENABLED
  	            bool "Enable MyApp Logging"
	            default y

             module = MYAPP
             module-str = MyApp sample
             source "${ZEPHYR_BASE}/subsys/logging/Kconfig.template.log_config"
          endmenu
       endmenu

       menu "Zephyr Kernel"
         source "Kconfig.zephyr"
       endmenu

3) Now let us use these new symbols. In our main.c file we have to replace the LOG_MODULE_REGISTER instruction by following lines:
   
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
4) Now we can define the application logging in the prj.conf file by setting the new CONFIG sympbols. 

   _prj.conf_

       # Generic Logging settings
       CONFIG_LOG=y

       # MyApp Logging settings
       CONFIG_MYAPP_LOG_ENABLED=y
       #CONFIG_MYAPP_LOG_LEVEL_ERR=y
       #CONFIG_MYAPP_LOG_LEVEL_WRN=y
       #CONFIG_MYAPP_LOG_LEVEL_INF=y
       #CONFIG_MYAPP_LOG_LEVEL_DBG=y
Note: prj.conf lines that start with # are comments only! To use one of these CONFIG settings remove the "#". 

5) Open also "Guiconfig" and test the CONFIG settings. 
