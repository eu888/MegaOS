#include "io.h"
#include "pit.h"
#include "print.h"

#define PIT_FREQ 1193182
#define PIT_CHANNEL0 0x40
#define PIT_COMMAND  0x43

void init_pit(uint32_t freq) {
    print_str("Initializeing pit...\n");
    uint16_t divisor = PIT_FREQ / freq;
    outb(PIT_COMMAND, 0x36);
    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
    print_str("Initialized pit.\n");
}
