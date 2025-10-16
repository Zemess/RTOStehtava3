#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include "led_example.h"
#include "button_example.h"
#include "dispatcher.h"
#include "uartDef.h"

#define	STACKSIZE	1024
#define	PRIORITY	5

// K_THREAD_DEFINE(liikennevalot_thread, STACKSIZE, valotila_task, NULL, NULL, NULL, PRIORITY, 0, 0);

// Main program
int main(void) {
    printk("Koitetaan rullaa softaa.\n");
    uartDef_init();
    dispatcher_init();   
    init_button();
    init_led();

    return 0;
}


// valkoinen valo 
/*
K_THREAD_DEFINE(red_thread, STACKSIZE, red_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(green_thread, STACKSIZE, green_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
K_THREAD_DEFINE(blue_thread, STACKSIZE, blue_led_task, NULL, NULL, NULL, PRIORITY, 0, 0);
*/
