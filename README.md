# ch32v003_tm1638
Library for TM1638 module for CH32V003 MCU. Should work with other CH32 MCU as well with minor modifications or even without.


# Usage example

MounRiver studio example.

```c
#include "tm1638.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
    SDI_Printf_Enable();
#else
    USART_Printf_Init(115200);
#endif
    printf("SystemClk:%d\r\n",SystemCoreClock);
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );

    USARTx_CFG();
    /* Initialize GPIOC clock if GPIOC pins used for STB/CLK/DIO. */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    tm1638_init();
    tm1638_displayBrightness(1);
    uint8_t i = 0;
    uint32_t n = 0;
    while(1)
    {
        n++;
        tm1638_printDec(0, n, 4, TM1638_PRINT_DOT | TM1638_PRINT_LEADING_ZERO);
        tm1638_printHex(4, n, 4, TM1638_PRINT_DOT );
        tm1638_displayOn();
        printf("Btn: %02X\n", tm1638_getButtons());
        tm1638_displayBrightness(i);
        tm1638_setLed(i, 1);
        Delay_Ms(10);
        tm1638_setLed(i, 0);
        Delay_Ms(10);
        i = (i + 1) & 0x7;
    }
}
```
