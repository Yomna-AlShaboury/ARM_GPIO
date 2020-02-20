/*
 * GPIO_PROG.c
 *
 *  Created on: Feb 19, 2020
 *      Author: yssh
 */

#include "GPIO_INT.h"

/*
 * this array holds the addresses of the ports[A:F] in both aperture APB and AHB;
 * each address is 32 bit
 */
static u32 ports[12] ={
                       GPIO_PORT_A_APB_BASE, GPIO_PORT_A_AHB_BASE,
                       GPIO_PORT_B_APB_BASE, GPIO_PORT_B_AHB_BASE,
                       GPIO_PORT_C_APB_BASE, GPIO_PORT_C_AHB_BASE,
                       GPIO_PORT_D_APB_BASE, GPIO_PORT_D_AHB_BASE,
                       GPIO_PORT_E_APB_BASE, GPIO_PORT_E_AHB_BASE,
                       GPIO_PORT_F_APB_BASE, GPIO_PORT_F_AHB_BASE
};

/*
 * sets the bus [APB - AHB] defined in the type  "gpioBus"   for a port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 */
void GPIO_vidBusSet(gpioPort localPort, gpioBus localBus)
{
    GPIOHBCTL &= ~(1 << localPort);
    GPIOHBCTL |= ( localBus << localPort);
}

/*
 * gets the bus [APB - AHB] defined in the type  "gpioBus"   for a port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 */
gpioBus GPIO_BusGet(gpioPort localPort)
{
    return (gpioBus)((GPIOHBCTL >> localPort) & 1);
}

/*
 * this function is an auxiliary function that gets the address of the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it will be called frequently in this driver to get the address
 */
u32 GPIO_u32GetPortAdd(gpioPort localPort)
{
    gpioBus bus  = GPIO_BusGet(localPort);
    return ports[(2*localPort)+ bus];
}


/*
 * sets or clears the clk for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it sets the clk when "localAction" is [ENABLE] and clears it when "localAction" is
 * [DISABLE] defined in the type "enum ctrl"
 */
void GPIO_vidClkSet(gpioPort localPort, ctrl localAction)
{
    RCGCGPIO &= ~(1 << localPort);
    RCGCGPIO |= ( localAction << localPort);
}


/*
 * gets the clk for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it reurns the control state [DISABLE - ENABLE] defined in the type "enum ctrl"
 */
ctrl GPIO_clkGet(gpioPort localPort)
{
    return (ctrl)((RCGCGPIO >> localPort) & 1);
}



/*
 * sets the mode and direction [MODE_IN- MODE_OUT- MODE_AF] for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 */
void GPIO_vidDirectionModeSet(gpioPort localPort, u8 localPins, gpioMode localMode)
{
    volatile u32* reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOAFSEL);
    u32 data = *reg;
    data &= ~ localPins;
    if (localMode == MODE_AF)
        data |= (0xFF & localPins);
    else
        data |= (0x00 & localPins);
    *reg = data;

    reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIODIR);
    data = *reg;
    if (localMode == MODE_OUT)
        data |= (0xFF & localPins);
    else if (localMode == MODE_IN)
        data |= (0x00 & localPins);
    *reg = data;
}

/*
 * gets the direction for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it reurns the state of the wanted pins [0 --> input      1--> output], unwanted pins are all zeros
 */
u8 GPIO_u8DirectionGet(gpioPort localPort, u8 localPins)
{
    u32 reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIODIR));
    return (u8)(reg & localPins);
}

/*
 * gets the mode for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it reurns the state of the wanted pins [0 --> GPIO      1--> AF], unwanted pins are all zeros
 */
u8 GPIO_u8ModeGet(gpioPort localPort, u8 localPins)
{
    u32 reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIOAFSEL));
    return (u8)(reg & localPins);
}


/*
 * sets the pad and drive ctrls for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * type"enum gpioDrive"  [DRIVE_2_m  -  DRIVE_4_m  -  DRIVE_8_m  -  DRIVE_8_m_Slew]
 * type"enum gpioPad"  [PAD_PU  -  PAD_PD  -  PAD_OD  -  PAD_NPU_NPD]
 */
void GPIO_vidPadSet(gpioPort localPort, u8 localPins, gpioPad localPad, gpioDrive localstr, gpioSig localSignal)
{
    volatile u32 * reg, * regSLR, * regAnlg;

    u32 data;
    reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIODEN);
    regAnlg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOAMSEL);
    if (localSignal == DIGITAL)
    {
        MASK_SET(reg, localPins, data);
        MASK_CLR(regAnlg, localPins, data);
    }
    else
    {
        MASK_CLR(reg, localPins, data);
        MASK_SET(regAnlg, localPins, data);
    }

    switch(localstr)
    {
    case DRIVE_2_m:
        reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIODR2R);
        break;
    case DRIVE_4_m:
        reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIODR4R);
        break;
    case DRIVE_8_m:
        reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIODR8R);
        break;
    case DRIVE_8_m_Slew:
        reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIODR8R);
        regSLR = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOSLR);
        /* Slew Rate */
        MASK_SET(regSLR, localPins, data);
        break;
    }
    MASK_SET(reg, localPins, data);


    reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOPUR);
    MASK_CLR(reg, localPins, data);
    reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOPDR);
    MASK_CLR(reg, localPins, data);
    reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOODR);
    MASK_CLR(reg, localPins, data);
    switch(localPad)
    {
    case PAD_PU:
        reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOPUR);
        MASK_SET(reg, localPins, data);
        break;

    case PAD_PD:
        reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOPDR);
        MASK_SET(reg, localPins, data);
        break;

    case PAD_OD:
        reg = (u32*) (GPIO_u32GetPortAdd(localPort) + GPIOODR);
        MASK_SET(reg, localPins, data);
        break;

    case PAD_NPU_NPD:
        /* nothing else to do  */
        break;
    }
}

/*
 * gets the drive strength state for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it returns the state of the wanted pins due to a selction of the
 * drive strength type "gpioDrive" [1 --> drive set      0--> drive not set]
 * unwanted pins are all zeros
 */
u8 GPIO_u8PadDriveStrGet(gpioPort localPort, u8 localPins, gpioDrive localstr)
{
    u32 reg;
    switch (localstr)
    {
    case DRIVE_2_m:
        reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIODR2R));
        break;

    case DRIVE_4_m:
        reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIODR4R));
        break;

    case DRIVE_8_m:
        reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIODR8R));
        break;
    case DRIVE_8_m_Slew:
        reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIOSLR));
        break;
    }
    return (u8)(reg & localPins);
}


/*
 * gets the open-drain state for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it returns the state of the wanted pins [1 --> open-drain      0--> not open-drain], unwanted pins are all zeros
 */
u8 GPIO_u8PadOpenDrainGet(gpioPort localPort, u8 localPins)
{
    u32 reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIOODR));
    return (u8)(reg & localPins);
}

/*
 * gets the pull-up res state for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it returns the state of the wanted pins [0 --> not pull-up      1--> pull-up], unwanted pins are all zeros
 */
u8 GPIO_u8PadPullUpGet(gpioPort localPort, u8 localPins)
{
    u32 reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIOPUR));
    return (u8)(reg & localPins);
}


/*
 * gets the pull-down res state for the port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * it returns the state of the wanted pins [0 --> not pull-down      1--> pull-down], unwanted pins are all zeros
 */
u8 GPIO_u8PadPullDownGet(gpioPort localPort, u8 localPins)
{
    u32 reg = *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIOPDR));
    return (u8)(reg & localPins);
}


/*
 * writes data to one or more pins in a port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 */
void GPIO_vidWrite(gpioPort localPort, u8 localPins, u8 localData)
{
    volatile u32* reg = (u32*)(GPIO_u32GetPortAdd(localPort) + GPIODATA + (u32)(localPins << 2) );
    *reg = localData;
}


/*
 * reads data from one or more pins in a port named as:
 * [PORTA, PORTB, PORTC, PORTD PORTE, PORTF] defined in the type "enum gpioPort"
 * unwanted pins are all zeros
 */
u8 GPIO_u8Read(gpioPort localPort, u8 localPins)
{
    return *((volatile u32*)(GPIO_u32GetPortAdd(localPort) + GPIODATA + (u32)(localPins << 2)));
}

