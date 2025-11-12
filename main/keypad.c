#include "keypad.h"
#include "driver/gpio.h"
#include "table.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

T8U keypad_shift = 0;

void PinInput_init()
{
    gpio_config_t io_conf = {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << KEYPADA) | (1ULL << KEYPADB) |
                        (1ULL << KEYPADC) | (1ULL << KEYPADD),
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

T8U Keypad_Scan(void)
{
    vTaskDelay(pdMS_TO_TICKS(20)); // debounce

    if (gpio_get_level(KEYPADA) == 0) return 'A';
    if (gpio_get_level(KEYPADB) == 0) return 'B';
    if (gpio_get_level(KEYPADC) == 0) return 'C';
    if (gpio_get_level(KEYPADD) == 0) return 'D';

    keypad_shift = 0;
    return 0;
}
