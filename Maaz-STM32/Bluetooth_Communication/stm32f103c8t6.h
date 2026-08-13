#include <stddef.h>
#include <stdint.h>

/* Peripheral base addresses */

#define RCC_BASE 0x40021000U
#define GPIOA_BASE 0x40010800U
#define GPIOB_BASE 0x40010C00U
#define GPIOC_BASE 0x40011000U
#define GPIOD_BASE 0x40011400U

/* Peripheral structures */

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

_Static_assert(offsetof(RCC_TypeDef, CR) == 0x00, "RCC CR offset");
_Static_assert(offsetof(RCC_TypeDef, CFGR) == 0x04, "RCC CFGR offset");
_Static_assert(offsetof(RCC_TypeDef, CIR) == 0x08, "RCC CIR offset");
_Static_assert(offsetof(RCC_TypeDef, APB2RSTR) == 0x0C, "RCC APB2RSTR offset");
_Static_assert(offsetof(RCC_TypeDef, APB1RSTR) == 0x10, "RCC APB1RSTR offset");
_Static_assert(offsetof(RCC_TypeDef, AHBENR) == 0x14, "RCC AHBENR offset");
_Static_assert(offsetof(RCC_TypeDef, APB2ENR) == 0x18, "RCC APB2ENR offset");
_Static_assert(offsetof(RCC_TypeDef, APB1ENR) == 0x1C, "RCC APB1ENR offset");
_Static_assert(offsetof(RCC_TypeDef, BDCR) == 0x20, "RCC BDCR offset");
_Static_assert(offsetof(RCC_TypeDef, CSR) == 0x24, "RCC CSR offset");

typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

_Static_assert(offsetof(GPIO_TypeDef, CRL) == 0x00,
               "GPIO_TypeDef: CRL offset incorrect");

_Static_assert(offsetof(GPIO_TypeDef, CRH) == 0x04,
               "GPIO_TypeDef: CRH offset incorrect");

_Static_assert(offsetof(GPIO_TypeDef, IDR) == 0x08,
               "GPIO_TypeDef: IDR offset incorrect");

_Static_assert(offsetof(GPIO_TypeDef, ODR) == 0x0C,
               "GPIO_TypeDef: ODR offset incorrect");

_Static_assert(offsetof(GPIO_TypeDef, BSRR) == 0x10,
               "GPIO_TypeDef: BSRR offset incorrect");

_Static_assert(offsetof(GPIO_TypeDef, BRR) == 0x14,
               "GPIO_TypeDef: BRR offset incorrect");

_Static_assert(offsetof(GPIO_TypeDef, LCKR) == 0x18,
               "GPIO_TypeDef: LCKR offset incorrect");

_Static_assert(sizeof(GPIO_TypeDef) == 0x1C,
               "GPIO_TypeDef: size incorrect");

/* Peripheral instances */

#define RCC ((RCC_TypeDef *)RCC_BASE)

#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD ((GPIO_TypeDef *)GPIOD_BASE)

/* ---------- AFIO ---------- */
#define AFIO_BASE 0x40010000U

typedef struct
{
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR[4];
    volatile uint32_t RESERVED;
    volatile uint32_t MAPR2;
} AFIO_TypeDef;

_Static_assert(offsetof(AFIO_TypeDef, EVCR) == 0x00, "AFIO EVCR offset");
_Static_assert(offsetof(AFIO_TypeDef, MAPR) == 0x04, "AFIO MAPR offset");
_Static_assert(offsetof(AFIO_TypeDef, EXTICR) == 0x08, "AFIO EXTICR offset");
_Static_assert(offsetof(AFIO_TypeDef, MAPR2) == 0x1C, "AFIO MAPR2 offset");

#define AFIO ((AFIO_TypeDef *)AFIO_BASE)

/* ---------- USART2 ---------- */
#define USART2_BASE 0x40004400U

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

_Static_assert(offsetof(USART_TypeDef, SR) == 0x00, "USART SR offset");
_Static_assert(offsetof(USART_TypeDef, DR) == 0x04, "USART DR offset");
_Static_assert(offsetof(USART_TypeDef, BRR) == 0x08, "USART BRR offset");
_Static_assert(offsetof(USART_TypeDef, CR1) == 0x0C, "USART CR1 offset");
_Static_assert(offsetof(USART_TypeDef, CR2) == 0x10, "USART CR2 offset");
_Static_assert(offsetof(USART_TypeDef, CR3) == 0x14, "USART CR3 offset");
_Static_assert(offsetof(USART_TypeDef, GTPR) == 0x18, "USART GTPR offset");

#define USART2 ((USART_TypeDef *)USART2_BASE)
