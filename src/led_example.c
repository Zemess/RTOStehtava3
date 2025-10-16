#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include "led_example.h"

K_SEM_DEFINE(red_sem, 0, 1);
K_SEM_DEFINE(green_sem, 0, 1);
K_SEM_DEFINE(yellow_sem, 0, 1);
K_SEM_DEFINE(release_sem, 1, 1);

// Led pin configurations
static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec blue = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);

// Thread function prototypes
static void red_task(void *, void *, void *);
static void green_task(void *, void *, void *);
static void yellow_task(void *, void *, void *);




// Initialize leds
int init_led() {
    int ret = gpio_pin_configure_dt(&red, GPIO_OUTPUT_INACTIVE);
	if (ret) return ret;
	ret = gpio_pin_configure_dt(&green, GPIO_OUTPUT_INACTIVE);
	if (ret) return ret;
	ret = gpio_pin_configure_dt(&blue, GPIO_OUTPUT_INACTIVE);
	if (ret) return ret;
    printk("Led initialization ok\n");
    return 0;
}

static void set_leds(int r, int g, int b) {
	gpio_pin_set_dt(&red, r);
	gpio_pin_set_dt(&green, g);
	gpio_pin_set_dt(&blue, b);
}
static void red_task(void *p1, void *p2, void *p3) {
	while (1) {
		k_sem_take(&red_sem, K_FOREVER);
		set_leds(1, 0, 0);
		printk("Red on\n");
		k_sleep(K_SECONDS(1));
		set_leds(0, 0, 0);
		printk("Red off\n");
		k_sem_give(&release_sem);
	}
}
static void green_task(void *p1, void *p2, void *p3) {
	while (1) {
		k_sem_take(&green_sem, K_FOREVER);
		set_leds(0, 1, 0);
		printk("Green on\n");
		k_sleep(K_SECONDS(1));
		set_leds(0, 0, 0);
		printk("Green off\n");
		k_sem_give(&release_sem);
	}
}
static void yellow_task(void *p1, void *p2, void *p3) {
	while (1) {
		k_sem_take(&yellow_sem, K_FOREVER);
		set_leds(1, 1, 0);
		printk("Blue on\n");
		k_sleep(K_SECONDS(1));
		set_leds(0, 0, 0);
		printk("Blue off\n");
		k_sem_give(&release_sem);
	}
}

K_THREAD_DEFINE(red_thread, 1024, red_task, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(green_thread, 1024, green_task, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(yellow_thread, 1024, yellow_task, NULL, NULL, NULL, 5, 0, 0);
