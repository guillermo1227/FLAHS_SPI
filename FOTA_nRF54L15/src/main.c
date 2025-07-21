#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/settings/settings.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
 
#define DEVICE_NAME "L4SEC2"
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
#define LED0_NODE DT_ALIAS(led0)
 
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
    BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};
 
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
 
void main(void)
{
    int err;
 
    printk("Iniciando BLE para DFU OTA con nombre y parpadeo en P0.29\n");
 
    if (!device_is_ready(led.port)) {
        printk("LED no disponible\n");
        return;
    }
 
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
 
    err = bt_enable(NULL);
    if (err) {
        printk("Fallo al iniciar Bluetooth (err %d)\n", err);
        return;
    }
 
    if (IS_ENABLED(CONFIG_SETTINGS)) {
        settings_load();
    }
 
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        printk("Fallo al iniciar publicidad (err %d)\n", err);
        return;
    }
 
    printk("Publicidad BLE iniciada\n");
 
    while (1) {
        gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(500));
    }
}