

/**
 * main.c
 */
#include"GPIO_INT.h"
//#include <util/delay.h>

volatile u32 * RCC = (u32*) 0x400FE060;

void main(void)
{
    *RCC |= (1<<11);
    *RCC &= ~(1<<22);
    *RCC |= (1<<4);
    *RCC &= ~(1<<5);
    *RCC |= (1<<13);

    GPIO_vidBusSet(PORTF, AHB);
    GPIO_vidClkSet(PORTF, ENABLE);
    GPIO_vidDirectionModeSet(PORTF, 0b00010001, MODE_IN);
    GPIO_vidDirectionModeSet(PORTF, 0x0E, MODE_OUT);
    GPIO_vidPadSet(PORTF, 0x0E, PAD_NPU_NPD, DRIVE_8_m, DIGITAL);
    GPIO_vidPadSet(PORTF, 0b00010001, PAD_PU, DRIVE_8_m, DIGITAL);

    while (1)
    {
        GPIO_vidWrite(PORTF, 0x02, 0xFF);
//        _delay_ms(200);

//        if (GPIO_u8Read(PORTF, 0x01) == 1)
//            GPIO_vidWrite(PORTF, 0x03, 0xFF);
//        else
//            GPIO_vidWrite(PORTF, 0x03, 0x00);

    }
}
