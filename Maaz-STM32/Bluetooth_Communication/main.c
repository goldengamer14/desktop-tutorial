#include "stm32f103c8t6.h"

/* ============================================================
 * RCC
 * ============================================================ */

#define RCC_APB2ENR_IOPAEN (1U << 2)
#define RCC_APB2ENR_IOPCEN (1U << 4)

#define RCC_APB1ENR_USART2EN (1U << 17)

/* ============================================================
 * USART2 STATUS REGISTER
 * ============================================================ */

#define USART_SR_RXNE (1U << 5)
#define USART_SR_TXE (1U << 7)

/* ============================================================
 * USART2 CONTROL REGISTER 1
 * ============================================================ */

#define USART_CR1_RE (1U << 2)
#define USART_CR1_TE (1U << 3)
#define USART_CR1_UE (1U << 13)

/* ============================================================
 * LED
 *
 * Blue Pill onboard LED on PC13 is active LOW.
 * ============================================================ */

#define LED_ON() (GPIOC->BRR = (1U << 13))
#define LED_OFF() (GPIOC->BSRR = (1U << 13))
#define LED_TOGGLE() (GPIOC->ODR ^= (1U << 13))

/* ============================================================
 * RX BUFFER
 * ============================================================ */

#define RX_BUFFER_SIZE 128U

static volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile uint32_t rx_count = 0U;

/* ============================================================
 * DELAY
 * ============================================================ */

static void delay(volatile uint32_t count)
{
    while (count--)
    {
        __asm volatile("nop");
    }
}

/* ============================================================
 * LED FLASH
 * ============================================================ */

static void flash(uint32_t count)
{
    while (count--)
    {
        LED_ON();
        delay(150000U);

        LED_OFF();
        delay(150000U);
    }

    delay(400000U);
}

/* ============================================================
 * USART2 WRITE BYTE
 * ============================================================ */

static void usart2_write(uint8_t data)
{
    while (!(USART2->SR & USART_SR_TXE))
    {
    }

    USART2->DR = data;
}

/* ============================================================
 * USART2 WRITE STRING
 * ============================================================ */

static void usart2_write_string(const char *str)
{
    while (*str)
    {
        usart2_write((uint8_t)*str);
        str++;
    }
}

/* ============================================================
 * USART2 INITIALIZATION
 *
 * PA2 = USART2_TX
 * PA3 = USART2_RX
 *
 * 9600 baud
 * 8 data bits
 * No parity
 * 1 stop bit
 *
 * Assumes USART2 clock = 8 MHz.
 * BRR = 8000000 / 9600 ≈ 833
 * ============================================================ */

static void usart2_init(void)
{
    /* Enable GPIOA clock */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    /* Enable GPIOC clock */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    /* Enable USART2 clock */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /*
     * PA2 = USART2_TX
     *
     * MODE = 11
     * CNF  = 10
     *
     * Alternate-function push-pull.
     */

    GPIOA->CRL &= ~(0xFU << 8);
    GPIOA->CRL |= (0xBU << 8);

    /*
     * PA3 = USART2_RX
     *
     * MODE = 00
     * CNF  = 01
     *
     * Floating input.
     */

    GPIOA->CRL &= ~(0xFU << 12);
    GPIOA->CRL |= (0x4U << 12);

    /*
     * 9600 baud.
     */

    USART2->BRR = 833U;

    /*
     * 1 stop bit.
     */

    USART2->CR2 = 0U;

    /*
     * No hardware flow control.
     */

    USART2->CR3 = 0U;

    /*
     * Enable USART, transmitter and receiver.
     */

    USART2->CR1 =
        USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

/* ============================================================
 * CLEAR RX BUFFER
 * ============================================================ */

static void clear_rx_buffer(void)
{
    uint32_t i;

    rx_count = 0U;

    for (i = 0U; i < RX_BUFFER_SIZE; i++)
    {
        rx_buffer[i] = 0U;
    }
}

/* ============================================================
 * SEND RECEIVED MESSAGE
 * ============================================================ */

static void send_received_message(void)
{
    uint32_t i;

    usart2_write_string("\r\nReceived ");

    for (i = 0U; i < rx_count; i++)
    {
        uint8_t c = rx_buffer[i];

        /*
         * Ignore CR and LF.
         *
         * Some Bluetooth terminal applications append
         * these automatically.
         */

        if (c == '\r' || c == '\n')
        {
            continue;
        }

        /*
         * Display normal printable ASCII characters.
         */

        if (c >= 32U && c <= 126U)
        {
            usart2_write(c);
        }
    }

    usart2_write_string("\r\n");

    clear_rx_buffer();
}

/* ============================================================
 * MAIN
 * ============================================================ */

int main(void)
{
    uint32_t idle_count = 0U;

    /* --------------------------------------------------------
     * PC13 LED initialization
     * -------------------------------------------------------- */

    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(0xFU << 20);
    GPIOC->CRH |= (0x1U << 20);

    LED_OFF();

    /* --------------------------------------------------------
     * USART2 initialization
     * -------------------------------------------------------- */

    usart2_init();

    /*
     * Startup indication.
     *
     * 2 flashes = firmware started.
     */

    flash(2U);

    /*
     * Startup message.
     */

    delay(500000U);

    usart2_write_string(
        "\r\nSTM32 USART2 READY\r\n"
        "Send text and press ENTER.\r\n");

    /* --------------------------------------------------------
     * MAIN LOOP
     * -------------------------------------------------------- */

    while (1)
    {
        /*
         * ----------------------------------------------------
         * Check for received character.
         * ----------------------------------------------------
         */

        if (USART2->SR & USART_SR_RXNE)
        {
            uint8_t received;

            /*
             * Read received byte.
             */

            received = (uint8_t)USART2->DR;

            /*
             * Store it if there is space.
             */

            if (rx_count < RX_BUFFER_SIZE)
            {
                rx_buffer[rx_count] = received;
                rx_count++;
            }

            /*
             * Visual indication:
             *
             * LED toggles whenever a byte arrives.
             */

            LED_TOGGLE();

            /*
             * Reset idle timer.
             */

            idle_count = 0U;

            /*
             * If terminal explicitly sent ENTER,
             * immediately process the message.
             */

            if (received == '\r' || received == '\n')
            {
                if (rx_count > 1U)
                {
                    send_received_message();
                }
                else
                {
                    clear_rx_buffer();
                }

                idle_count = 0U;
            }
        }
        else
        {
            /*
             * No character currently waiting.
             *
             * If we have received data, count idle time.
             */

            if (rx_count > 0U)
            {
                idle_count++;

                /*
                 * If no new byte arrives for a while,
                 * consider the current data to be one message.
                 *
                 * This allows the terminal to send:
                 *
                 *     OK
                 *
                 * without requiring CR/LF.
                 */

                if (idle_count >= 50000U)
                {
                    send_received_message();

                    idle_count = 0U;
                }
            }
        }
    }
}
