#include <zephyr/kernel.h>       // sisältää K_FOREVER, K_FIFO_DEFINE, k_sem jne.
#include <zephyr/sys/printk.h>
#include <string.h>
#include "led_example.h"
#include "dispatcher.h"

// Viittaukset LED-taskien semafooreihin (määritelty led_example.c:ssä)
extern struct k_sem red_sem;
extern struct k_sem green_sem;
extern struct k_sem yellow_sem;
extern struct k_sem release_sem;

// FIFO määrittely dispatcherille
K_FIFO_DEFINE(dispatcher_fifo);

// Dispatcherille tapahtuman lähetys FIFOon
void dispatcher_post(dispatcher_event_t *evt) {
    dispatcher_event_t *copy = k_malloc(sizeof(dispatcher_event_t));
    if (copy) {
        memcpy(copy, evt, sizeof(dispatcher_event_t));
        printk("Dispatcher: received event '%s'\n", evt->msg);
        k_fifo_put(&dispatcher_fifo, copy);
    } else {
        printk("Dispatcher: memory alloc failed!\n");
    }
}

// Dispatcher-task, joka lukee FIFOa ja ohjaa valotaskien semafooreja
static void dispatcher_task(void *p1, void *p2, void *p3) {
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    printk("Dispatcher task started\n");

    while (1) {
        dispatcher_event_t *evt = k_fifo_get(&dispatcher_fifo, K_FOREVER);
        if (!evt) {
            continue;
        }

        printk("Dispatcher: got event -> '%s'\n", evt->msg);

        switch (evt->msg[0]) {
        case 'r':
            printk("Dispatcher: activating RED\n");
            k_sem_take(&release_sem, K_FOREVER);
            k_sem_give(&red_sem);
            break;
        case 'y':
            printk("Dispatcher: activating YELLOW\n");
            k_sem_take(&release_sem, K_FOREVER);
            k_sem_give(&yellow_sem);
            break;
        case 'g':
            printk("Dispatcher: activating GREEN\n");
            k_sem_take(&release_sem, K_FOREVER);
            k_sem_give(&green_sem);
            break;
        default:
            printk("Dispatcher: unknown symbol '%c'\n", evt->msg[0]);
            break;
        }

        k_free(evt);
    }
}

// Dispatcher-säikeen määrittely
K_THREAD_DEFINE(dispatch_thread, 1024, dispatcher_task, NULL, NULL, NULL, 5, 0, 0);

// Dispatcher-moduulin alustus
void dispatcher_init(void) {
    printk("Dispatcher initialized\n");
}
