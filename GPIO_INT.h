/*
 * GPIO_INT.h
 *
 *  Created on: Feb 14, 2020
 *      Author: yssh
 */

#ifndef GPIO_INT_H_
#define GPIO_INT_H_

#include"GPIO_REG.h"
#include"SYSTEM_CTL.h"
#include"std_types.h"

typedef enum {MODE_IN = 0x00, MODE_OUT= 0xFF, MODE_AF = 0x0F} gpioMode;
typedef enum {PORTA, PORTB, PORTC, PORTD, PORTE, PORTF} gpioPort;
typedef enum {APB, AHB} gpioBus;
typedef enum {DISABLE, ENABLE} ctrl;
typedef enum {DRIVE_2_m, DRIVE_4_m, DRIVE_8_m, DRIVE_8_m_Slew} gpioDrive;
typedef enum {PAD_PU, PAD_PD, PAD_OD, PAD_NPU_NPD} gpioPad;
typedef enum {ANALOG, DIGITAL} gpioSig;

#define MASK_SET(reg, pins, data) ({\
        data = *reg;\
        data &= ~ pins;\
        data |= (0xFF & pins);\
        *reg = data;\
})
#define MASK_CLR(reg, pins, data) ({\
        data = *reg;\
        data &= ~ pins;\
        data |= (0x00 & pins);\
        *reg = data;\
})


u32 GPIO_u32GetPortAdd(gpioPort localPort);

void GPIO_vidBusSet(gpioPort localPort, gpioBus localBus);
gpioBus GPIO_BusGet(gpioPort localPort);

void GPIO_vidClkSet(gpioPort localPort, ctrl localAction);
ctrl GPIO_clkGet(gpioPort localPort);

void GPIO_vidDirectionModeSet(gpioPort localPort, u8 localPins, gpioMode localMode);
u8 GPIO_u8DirectionGet(gpioPort localPort, u8 localPins);
u8 GPIO_u8ModeGet(gpioPort localPort, u8 localPins);


/*     -------------------------------------------------       */


void GPIO_vidPadSet(gpioPort localPort, u8 localPins, gpioPad localPad, gpioDrive localstr, gpioSig localSignal);
u8 GPIO_u8PadDriveStrGet(gpioPort localPort, u8 localPins, gpioDrive localstr);
u8 GPIO_u8PadOpenDrainGet(gpioPort localPort, u8 localPins);
u8 GPIO_u8PadPullUpGet(gpioPort localPort, u8 localPins);
u8 GPIO_u8PadPullDownGet(gpioPort localPort, u8 localPins);


void GPIO_vidWrite(gpioPort localPort, u8 localPins, u8 localData);
u8 GPIO_u8Read(gpioPort localPort, u8 localPins);

#endif /* GPIO_INT_H_ */
