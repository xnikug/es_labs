#include "motor_stdio.h"
#include "../../edRelay/edRelay.h"

void motor_stdio_init(uint8_t pin)
{
    (void)pin; /* pin is set by ED_RELAY_PIN in edRelay.h */
    edRelayInit();
}

void motor_set_state(uint8_t on)
{
    if (on) edRelayOn(); else edRelayOff();
}
