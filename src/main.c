/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */

#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/* Define RGB LEDs using direct GPIO specification */
#define RGB_R_PIN     31
#define RGB_G_PIN     30
#define RGB_B_PIN     29
#define GPIO0_NODE    DT_NODELABEL(gpio0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec rgb_r = {
    .port = DEVICE_DT_GET(GPIO0_NODE),
    .pin = RGB_R_PIN,
    .dt_flags = GPIO_ACTIVE_LOW
};

static const struct gpio_dt_spec rgb_g = {
    .port = DEVICE_DT_GET(GPIO0_NODE),
    .pin = RGB_G_PIN,
    .dt_flags = GPIO_ACTIVE_LOW
};

static const struct gpio_dt_spec rgb_b = {
    .port = DEVICE_DT_GET(GPIO0_NODE),
    .pin = RGB_B_PIN,
    .dt_flags = GPIO_ACTIVE_LOW
};

int main(void)
{
	/* Initialize the LED GPIO pin */

	int ret;

	/*if (!device_is_ready(led.port)) {
		return -1;
	} */

	/* ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return -1;
	} */

	/* Configure RGB LEDs */
	if (!device_is_ready(rgb_r.port)) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&rgb_r, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&rgb_g, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return -1;
	}

	ret = gpio_pin_configure_dt(&rgb_b, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return -1;
	}

	printk("DFU External Test - Firmware cargado correctamente! v1.0\n");
	printk("Iniciando test simple de LEDs RGB...\n");

	// Secuencia de inicio - 3 parpadeos para confirmar boot
	for (int i = 0; i < 3; i++) {
		gpio_pin_set_dt(&rgb_r, 1);
		k_msleep(100);
		gpio_pin_set_dt(&rgb_r, 0);
		k_msleep(100);
	}

	int counter = 0;
	while (1) {
		// Patrón más simple y lento
		switch (counter % 3) {
			case 0:
				gpio_pin_set_dt(&rgb_r, 1); // Rojo ON
				gpio_pin_set_dt(&rgb_g, 0); // Verde OFF
				gpio_pin_set_dt(&rgb_b, 0); // Azul OFF
				printk("LED: ROJO\n");
				break;
			case 1:
			
				gpio_pin_set_dt(&rgb_r, 0);
				gpio_pin_set_dt(&rgb_g, 1); // Verde ON
				gpio_pin_set_dt(&rgb_b, 0);
				printk("LED: VERDE\n");
				break;
			case 2:
				gpio_pin_set_dt(&rgb_r, 0);
				gpio_pin_set_dt(&rgb_g, 0);
				gpio_pin_set_dt(&rgb_b, 1); // Azul ON
				printk("LED: AZUL\n");
				break;
		}
		counter++;
		k_msleep(2000); // 2 segundos entre cambios
	}
}
