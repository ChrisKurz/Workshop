# printk() function

For printing basic messages on a console, we can use the printk() method. Its syntax is similar to the standard C printf(). 
You can provide either a string literal or a format string followed by variable(s) to be printed.

A basic set of specifiers are supported:

- Signed decimal: %d, %i and its subcategories
- Unsigned decimal: %u and its subcategories
- Unsigned hexadecimal: %x (%X is treated as %x)
- Pointer: %p
- String: %s
- Character: %c
- Percent: %%
- New line: %n
- Carriage return: %r

Field width (with or without leading zeroes) is supported. Length attributes h, hh, l, ll and z are supported. 
However, integral values with lld and lli are only printed if they fit in a long otherwise ‘ERR’ is printed. 
Full 64-bit values may be printed with llx. Flags and precision attributes (float and double) are not supported 
by default. However, there are ways to make printk() support float and double covered in following lessons.

For example, the following line:

    printk("Button 1 was pressed!\n\r");

Will print the string Button 1 was pressed! on the console (including a new line and carriage return).

While the following:

    int x = 44;
    printk("The value of x is %d\n\r",x);

Will print the formatted string "The value of x is 44" on the console (including a new line and carriage return).

Using printk() is straightforward, all you have to do is :

1. Include the console drivers. This is done by enabling the configuration option (CONFIG_CONSOLE=y) in the application configuration file. This step is not necessary if it was already done in the board configuration file.   
2. Select the console. There are few options available such as UART console CONFIG_UART_CONSOLE, RTT backend CONFIG_RTT_CONSOLE. In this lesson, we will focus on the UART console which can be easily captured using a serial terminal program like PuTTY or nRF Terminal. The default console set in the board configuration file is the UART console. This step is not necessary if it was already done in the board configuration file.   
3. Include the header file <sys/printk.h> in your application source code. Note: zephyr.h already includes sys/printk.h! If you use #include "zephyr.h" you do not need to include sys/printk.h. 

One important note to remember is that the output of the printk()is NOT deferred. It is sent immediately to the console without any mutual exclusion or buffering. This could limit the use of this method in some time-critical situations. This is also known as synchronous logging or in-place logging or blocking logging (Logs are sent immediately as they are issued).
