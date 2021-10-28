# Bluetooth Low Energy: Peripheral with Device Information Service
## Introduction

The simplest way to use Bluetooth Low Energy is broadcasting and realising a beacon application. But usually a connection between two devices is required. Here we take a look on a peripheral solution, which allows to read its Device Information Service. 


## Peripheral 

Let's create this project from scratch:

1) Create a new project. 

	a) Create the project folder in your own Workspace directory. Example:    c:/MyWorkspace/BLE_Peripheral_DIS
	
	b) Create the needed files for a minimal project setup:
	
	  _CMakeLists.txt_
	  
       cmake_minimum_required(VERSION 3.17.3)

       find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
       project(MyPeripheralDIS)

       target_sources(app PRIVATE src/main.c)
				
	_prj.conf_
	   
       #=== Enable Bluetooth Stack
       CONFIG_BT=y
       CONFIG_BT_DEVICE_NAME="DIS peripheral"

       #------ Define Bluetooth LE Role 
       #CONFIG_BT_BROADCASTER=y   # note: BROADCASTER is activated by default
       #CONFIG_BT_CENTRAL=y       not used in this example
       #CONFIG_BT_OBSERVER=y      not used in this example
       CONFIG_BT_PERIPHERAL=y

       #------ Device Information Service (DIS)
       CONFIG_BT_DIS=y
       CONFIG_BT_DIS_MANUF="Zephyr"
       CONFIG_BT_DIS_MODEL="Zephyr Model"
       CONFIG_BT_DIS_SERIAL_NUMBER=y
       CONFIG_BT_DIS_SERIAL_NUMBER_STR="Zephyr Serial"
       CONFIG_BT_DIS_HW_REV=y
       CONFIG_BT_DIS_HW_REV_STR="Zephyr Hardware"
       CONFIG_BT_DIS_FW_REV=y
       CONFIG_BT_DIS_FW_REV_STR="Zephyr Firmware"
       CONFIG_BT_DIS_SW_REV=y
       CONFIG_BT_DIS_SW_REV_STR="Zephyr Software"
       CONFIG_BT_DIS_PNP=n
       #CONFIG_BT_DIS_PNP_PID=0x00   not used in this example
       #CONFIG_BT_DIS_PNP_VID=0x00   not used in this example
       #CONFIG_BT_DIS_PNP_VID_SRC=1  not used in this example
       #CONFIG_BT_DIS_PNP_VER=0x01   not used in this example
	
	_src/main.c_
		        
       #include <bluetooth/bluetooth.h>
 
       void main (void) {
           printk("Starting MyPeripheral application!");
       }

Notes: 
- The prj.conf file contains lines that are commented out. These have been added to allow a better understanding of the setting options and the used setting. Note that CONFIG_BT_BROADCASTER is by default 'y'. Moreover it is set when CONFIG_BT_PERIPHERAL is enabled.
- Please check the Bluetooth DIS specification and compare to the parameter settings done in prj.conf file. (see https://www.bluetooth.com/de/specifications/specs/)


2) Enable the Bluetooth Stack in the main function:

       void main(void){
           int err;
               ...
           /* Initialize the Bluetooth Subsystem */
           err = bt_enable(NULL);
           if (err) {
               printk("Bluetooth init failed (err %d)\n", err);
               return;
           }
           printk("Bluetooth initialized\n");
       }
	      
3) Define the Callback functions used for a connect or disconnect event:

       #include <bluetooth/conn.h> 

       static void connected(struct bt_conn *conn, uint8_t err)
       {
           if (err) {
               printk("Connection failed (err 0x%02x)\n", err);
           } else {
               printk("Connected\n");
           }
       }

       static void disconnected(struct bt_conn *conn, uint8_t reason)
       {
           printk("Disconnected (reason 0x%02x)\n", reason);
       }

       static struct bt_conn_cb conn_callbacks = {
           .connected = connected,
           .disconnected = disconnected,
       };

4) In main function we have to pass the info about the callback functions to the Bluetooth stack:

       void main(void){
               ...
           bt_conn_cb_register(&conn_callbacks);

       }
	      
5) Start Advertising: First we define the advertising payload:

               ...
       #include <bluetooth/uuid.h>
             
       static const struct bt_data ad[] = {
           BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
           BT_DATA_BYTES(BT_DATA_UUID16_ALL, BT_UUID_16_ENCODE(BT_UUID_DIS_VAL)),
       };


6) Start Advertising: And then we start the advertising in the main function:

       void main(void){
               ... 
           err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
           if (err) {
               printk("Advertising failed to start (err %d)\n", err);
               return;
           }
           printk("Advertising successfully started\n");
       }

7) Build the project and download to an nRF52-DK or nRF52840-DK.
8) Use a smartphone and the nRF Connect app and look for "DIS peripheral" device. Check the GATT database.
