#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/interrupt.h"

int enable = 0;

void delay()
{
    volatile uint32_t ui32Loop = 0x0FFFFF;//0x1FFFFF;
    while (ui32Loop > 0)
    {
        ui32Loop--;
    }
}

void paso_motor()
{
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0F, 0x00);
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0F, 0x01); //0000 0001
    delay();
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0F, 0x00);
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0F, 0x02); //0000 0010
    delay();
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0F, 0x00);
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0F, 0x04); //0000 0100
    delay();
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0F, 0x00);
    GPIOPinWrite(GPIO_PORTF_BASE, 0x0F, 0x08); //0000 1000
    delay();
}

void GPIOF_Handler_mifuncion()
{
    uint32_t banderas;

    banderas = GPIOIntStatus(GPIO_PORTF_BASE, true);

    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_4);

    banderas = GPIOIntStatus(GPIO_PORTF_BASE, true);

    if (enable == 0){
        enable = 1;
    }
    else
    {
        enable =0;
    }

}

int main(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    GPIOUnlockPin(GPIO_PORTF_BASE, GPIO_PIN_0);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0 );

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 );
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4 );

    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);
    GPIOIntRegister(GPIO_PORTF_BASE, GPIOF_Handler_mifuncion);

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_4);
    IntMasterEnable();



    while(1){

        if (enable == 0){
            paso_motor();
        }

    }

}
