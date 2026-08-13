#include <stdint.h>

#include "stm32f103c8t6.h"

static void delay_s(uint8_t sec)
{
    volatile uint32_t i;

    for (i = 0; i < 800000U * sec; i++)
    {
        __asm volatile("nop");
    }
}

static void delay_ms(uint32_t ms)
{
    volatile uint32_t i;

    for (i = 0; i < 800U * ms; i++)
    {
        __asm volatile("nop");
    }
}

int main(void)
{
    /* Enable GPIOC clock */
    RCC->APB2ENR |= (1U << 4);

    /* Configure PC13 as 2 MHz push-pull output */
    GPIOC->CRH &= ~(0xFU << 20);
    GPIOC->CRH |= (0x2U << 20);

    while (1)
    {
        /* LED ON */
        GPIOC->BRR = (1U << 13);
        delay_s(1);

        /* LED OFF */
        GPIOC->BSRR = (1U << 13);
        delay_s(1);

        /* LED ON */
        GPIOC->BRR = (1U << 13);
        delay_s(3);

        /* LED OFF */
        GPIOC->BSRR = (1U << 13);
        delay_s(1);

        /* LED ON */
        GPIOC->BRR = (1U << 13);
        delay_s(1);

        /* LED OFF */
        GPIOC->BSRR = (1U << 13);
        delay_ms(500);
    }
}
