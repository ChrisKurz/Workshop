# Bluetooth Low Energy: Peripheral with a user-define Service (Custom Service)

### Prepare the project

1) Make a copy of the previous project (Peripheral with Device Information Service). 
We will add a custom service and charactersitic to this project.
2) Add new folder "services" to the project. Create the files CustomService.c and CustomService.h in this new folder.
3) Add CustomSerice.c file to your project by changing the CMakeLists.txt file:
	
	  _CMakeLists.txt_
	  
       cmake_minimum_required(VERSION 3.17.3)

       find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
       project(MyPeripheralCusSer)

       target_sources(app PRIVATE src/main.c
                                  services/CustomService.c)
				
4) We need a transmission buffer for transmitting data. Add following lines to CustomService.c:

       #include "CustomService.h"
       #include <bluetooth/bluetooth.h>

       #define MAX_TRANSMIT_SIZE 240
       uint8_t data_buffer[MAX_TRANSMIT_SIZE];

       int CustomService_init(void)
       {
           int err = 0;
   
           memset(&data_buffer, 0, MAX_TRANSMIT_SIZE);
           return err;
       }

### Adding Custom Service

5) Add the declaration of CustomService_init() to CustomService.h header file:

       int CustomService_init(void); 

6) We need for the Custom Service a UUID and also for the Custom Characteristic. Create two UUIDs on https://www.uuidgenerator.net.
And add it to CusomtService.h:

       /*Note that UUIDs have Least Significant Byte ordering */
       #define CUSTOM_SERVICE_UUID 0xd4, 0x86, 0x48, 0x24, 0x54, 0xB3, 0x43, 0xA1, \
                                   0xBC, 0x20, 0x97, 0x8F, 0xC3, 0x76, 0xC2, 0x75                       

       #define CUSTOM_CHARACTERISTIC_UUID 0xED, 0xAA, 0x20, 0x11, 0x92, 0xE7, 0x43, 0x5A, \
                                          0xAA, 0xE9, 0x94, 0x43, 0x35, 0x6A, 0xD4, 0xD3

7) The custom UUIDs have to be declared. Add following lines in CustomService.c:

       # include <bluetooth/uuid.h>

       #define BT_UUID_CUSTOM_SERIVCE         BT_UUID_DECLARE_128(CUSTOM_SERVICE_UUID)
       #define BT_UUID_CUSTOM_CHARACTERISTIC  BT_UUID_DECLARE_128(CUSTOM_CHARACTERISTIC_UUID)

8) We need to define and register our service and its characteristics. By using the following helper macro we statically register our Service in our BLE host stack.
Add the following to my_service.c:

       #include <bluetooth/gatt.h>
       
       /* Custom Service Declaration and Registration */
       BT_GATT_SERVICE_DEFINE(CustomService,
                       BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERIVCE),
                       BT_GATT_CHARACTERISTIC(BT_UUID_CUSTOM_CHARACTERISTIC,
                                              BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                                              BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, 
                                              NULL, on_receive, NULL),
       );

### Add data transfer (Write without Response) to the project

9) Now add the function that takes care about getting a trigger if data is received. Add the following to my_service.c:

       /* This function is called whenever the RX Characteristic has been written to by a Client */
       static ssize_t on_receive(struct bt_conn *conn,
                                 const struct bt_gatt_attr *attr,
                                 const void *buf,
                                 uint16_t len,
                                 uint16_t offset,
                                 uint8_t flags)
       {
          const uint8_t * buffer = buf;
    
          printk("Received data: handle=%d, conn=%p, data=0x", attr->handle, conn);
          for(uint8_t i = 0; i < len; i++){
               printk("%02X", buffer[i]);
          }
          printk("\n");
          return len;
       }

10) We initialize our service by adding the following after enabling the Bluetooth stack in main.c:

        //Initalize services
        err = CustomService_init();
		 
11) Finally, build the project, start debugger, and test. Use Terminal program to check debug messages from the board.
