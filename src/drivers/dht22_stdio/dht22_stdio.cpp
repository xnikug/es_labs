#include "dht22_stdio.h"
#include "../../app_config.h"
#include <DHT.h>

static DHT s_dht(APP_DHT22_PIN, DHT22);

void dht22_stdio_init(uint8_t pin)
{
    (void)pin; /* pin is fixed to APP_DHT22_PIN at compile time */
    s_dht.begin();
}

uint8_t dht22_read(float *temperature_c, float *humidity_pct)
{
    float t = s_dht.readTemperature();
    float h = s_dht.readHumidity();

    if (isnan(t) || isnan(h)) return 0U;

    *temperature_c = t;
    *humidity_pct  = h;
    return 1U;
}
