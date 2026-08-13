# STM32F103C8T6 + HC-05 Project Context / Current State

## 1. Project Objective

The current learning/project path is focused on learning STM32F103C8T6 from an absolute beginner level, using **full Bare-Metal C** rather than HAL/LL abstractions.

The immediate hardware objective is:

> Connect an HC-05 Bluetooth module to the STM32F103C8T6 through USART2, receive Bluetooth/AT-command data, and process the received bytes directly through register-level firmware.

The longer-term project context is a wireless/Wi-Fi bridge/router-like system, but that is NOT the current implementation target. The current work is deliberately restricted to establishing reliable low-level STM32 UART communication with the HC-05.

---

## 2. User's STM32 Knowledge / Learning Approach

The STM32F103C8T6 is being learned from the ground up.

The intended approach is:

- Bare-Metal C
- Direct memory-mapped peripheral registers
- No HAL
- No STM32 peripheral abstraction libraries
- Understand:
  - peripheral base addresses
  - register structures
  - register offsets
  - RCC peripheral clocks
  - GPIO configuration
  - USART configuration
  - GPIO alternate functions
  - UART/USART TX/RX operation
  - status registers
  - memory-mapped RAM diagnostics
  - linker scripts
  - firmware testing through observable LED patterns
  - STM32CubeProgrammer for programming and memory inspection

STM32CubeIDE is installed and used primarily as the development/build environment.

STM32CubeProgrammer is used to:
1. Download `.elf`
2. Verify flash
3. Inspect RAM/register-related diagnostic data

---

## 3. Hardware

## STM32

Board:

- STM32F103C8T6
- Common "Blue Pill"-style board
- Device identity was verified through register behavior/writes rather than relying only on board labeling.

The board has only some pins soldered to headers.

Known available/soldered pins include:

- GND
- 3.3V
- R
- B11
- B10
- B1
- B0
- PA7 through PA0
- PC15 through PC13
- VB

This initially caused physical connection problems because not all board pins had headers soldered.

A soldering/pin-contact problem was eventually identified and corrected.

---

## 4. HC-05

Bluetooth module:

- HC-05
- Has an onboard button/KEY-related control
- Has onboard status LED

The HC-05 was previously successfully placed into AT mode.

A previous test established communication behavior associated with:

- AT mode
- 38400 baud

Current observation:

> HC-05 LED is slow blinking when in the current setup.

This is being treated as consistent with the intended AT-command state, but exact LED behavior can vary between HC-05 firmware/module variants.

---

## 5. Power

The STM32 is powered through the ST-LINK:

- ST-LINK connected to PC
- ST-LINK 3.3V → STM32 3.3V
- ST-LINK GND → STM32 GND

The HC-05 is supplied separately.

Earlier, an old ESP32 was considered as a possible 5V supply source for the HC-05, but that ESP32 was found to be old and to heat up quickly.

IMPORTANT:

> Do NOT rely on that ESP32 as a power supply going forward.

The HC-05's power arrangement has been handled separately from the STM32 supply.

A common ground is required between STM32 and HC-05 for UART communication.

---

## 6. USART2 Pins

The STM32F103C8T6 USART2 pins being used are:

- PA2 = USART2_TX
- PA3 = USART2_RX

Correct UART crossing:

```
STM32 PA2 / USART2_TX  -> HC-05 RXD
STM32 PA3 / USART2_RX  <- HC-05 TXD
STM32 GND               -> HC-05 GND

Do NOT connect:

PA2 -> HC-05 TXD
PA3 -> HC-05 RXD

TX and RX are crossed.
```

## 7. Initial GPIO/LED Debugging

Initially, LED patterns were used to communicate test results.

A problem occurred where the expected external LED behavior did not work correctly.

An *external LED* was tested with:

```
LED + -> PA2
LED - -> GND
```

The firmware configured `PA2` incorrectly at one point because:

`PA2` is in `GPIOA->CRL`, not `CRH`.

The correct `PA2` configuration location is:

`GPIOA->CRL`

not:

`GPIOA->CRH`

The LED issue was ultimately treated as an LED/hardware issue rather than continuing to use it as the primary diagnostic.

The onboard *PC13 LED* was then used instead.

## 8. PC13 Onboard LED

The common *Blue Pill PC13 LED* is treated as *active `LOW`*.

**Definitions used:**

```
#define LED_ON()    (GPIOC->BRR  = (1U << 13))
#define LED_OFF()   (GPIOC->BSRR = (1U << 13))
```

**PC13 configuration:**

```
GPIOC->CRH &= ~(0xFU << 20);
GPIOC->CRH |=  (0x1U << 20);
```

**This means:**

```
MODE = 01
CNF = 00
General-purpose push-pull output
10 MHz
```
## 9. RCC / GPIO Register Definitions

The project uses manually defined peripheral base addresses.

```
#define RCC_BASE       0x40021000U

#define GPIOA_BASE     0x40010800U
#define GPIOB_BASE     0x40010C00U
#define GPIOC_BASE     0x40011000U
#define GPIOD_BASE     0x40011400U
```

**RCC structure:**

```
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
```

**GPIO structure:**

```
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
```

**Peripheral instances:**

```
#define RCC     ((RCC_TypeDef *)RCC_BASE)

#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)
```

Static assertions were used to verify structure offsets.

## 10. AFIO

`AFIO` was also manually defined.

**Base:**

```
#define AFIO_BASE   0x40010000U
```

Structure:

```
typedef struct
{
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR[4];
    volatile uint32_t RESERVED;
    volatile uint32_t MAPR2;
} AFIO_TypeDef;
```

Instance:

```
#define AFIO ((AFIO_TypeDef *)AFIO_BASE)
```

`AFIO` is relevant to alternate-function routing, although the current USART2 setup uses the default PA2/PA3 mapping.

## 11. USART2 Register Definitions

**USART2 base:**

```
#define USART2_BASE   0x40004400U
```

**Structure:**

```
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
```

**Instance:**

```
#define USART2  ((USART_TypeDef *)USART2_BASE)
```

**Verified offsets:**

```
SR    0x00
DR    0x04
BRR   0x08
CR1   0x0C
CR2   0x10
CR3   0x14
GTPR  0x18
```
## 12. RCC Clock Enables Used

**GPIOA:**

```
#define RCC_APB2ENR_IOPAEN      (1U << 2)
```

**GPIOC:**

```
#define RCC_APB2ENR_IOPCEN      (1U << 4)
```

**USART2:**

```
#define RCC_APB1ENR_USART2EN    (1U << 17)
```

`USART2 clock` is therefore explicitly enabled through:

```
RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
```
## 13. USART2 Configuration

Current intended USART2 configuration:

```
Baud = 38400
Data = 8 bits
Parity = none
Stop = 1
Flow control = none
```

The project currently assumes an `8 MHz USART2 peripheral clock`.

**BRR used:**

```
USART2->BRR = 208U;
```

This was initially questioned because another value such as 833 had been tried.

The important resolution:

`BRR = 208` is correct for the current `8 MHz / 38400` configuration being used.

A previous attempt changing:

```
USART2->BRR = 833U;
```

did not solve the problem.

## 14. USART2 GPIO Configuration

**PA2:**

```
GPIOA->CRL &= ~(0xFU << 8);
GPIOA->CRL |=  (0xBU << 8);
```

This gives:

```
MODE = 11
CNF  = 10
```

i.e. alternate-function push-pull output.

**PA3:**

```
GPIOA->CRL &= ~(0xFU << 12);
GPIOA->CRL |=  (0x4U << 12);
```

This gives:

```
MODE = 00
CNF  = 01
```

i.e. floating input.

**USART2 CR1:**

```
USART2->CR1 =
      USART_CR1_TE
    | USART_CR1_RE
    | USART_CR1_UE;
```

with:

```
#define USART_CR1_RE  (1U << 2)
#define USART_CR1_TE  (1U << 3)
#define USART_CR1_UE  (1U << 13)
```
## 15. Important Debugging Milestone: PA2/PA3 Loopback

A major test was performed before involving the HC-05.

Physical connection:

`PA2 -> PA3`

directly.

The STM32 was programmed with a USART2 loopback test.

The firmware transmitted:

```
55 AA 33 CC
```

and read them back.

The result was:

```
TX = 55 AA 33 CC
RX = 55 AA 33 CC
```

The LED produced:

`3 flashes`

meaning success.

This is one of the most important achievements in the project.

It proves:

```
PA2 can transmit
PA3 can receive
USART2 is operating
USART2 register definitions are working
GPIO configuration is working
USART baud configuration is working sufficiently for loopback
the MCU clock assumptions used by the UART are valid enough
the direct register-level implementation is functional
the memory-mapped register structures are functioning as intended
```

Therefore the STM32 USART2 peripheral itself is no longer the primary suspect.

## 16. Fixed RAM Diagnostic Buffer

A fixed RAM section was added to the linker script.

Current memory:

```
RAM   ORIGIN = 0x20000000, LENGTH = 20K
FLASH ORIGIN = 0x08000000, LENGTH = 64K
```

Fixed diagnostic section:

```
.fixed_buf (NOLOAD) :
{
    . = ALIGN(4);
    KEEP(*(.fixed_buf))
} > RAM
```

Firmware places the diagnostic structure in it using:

```
__attribute__((section(".fixed_buf"), used))
volatile ...
```

This allows the firmware to stop while preserving diagnostic information that can then be inspected using STM32CubeProgrammer.

This has become the project's primary diagnostic method.

## 17. Linker Script / Fixed RAM Context

Relevant linker definitions:

```
ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

_Min_Heap_Size = 0x200;
_Min_Stack_Size = 0x400;

MEMORY
{
  RAM    (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
  FLASH  (rx)  : ORIGIN = 0x8000000,  LENGTH = 64K
}
```

The `.fixed_buf` section is placed before .isr_vector in the linker script and uses:

`NOLOAD`

The section is retained with:

```
KEEP(*(.fixed_buf))
```

This fixed RAM mechanism is now an established part of the project's debugging infrastructure.

## 18. STM32 Programming/Verification Procedure

The firmware workflow currently used is:

```
Build the project in Debug or Release mode.
Open the generated .elf in STM32CubeProgrammer.
Press Download.
Confirm:
File download complete
Press Verify.
Confirm:
Verification successful, no data mismatch found
Remove/reapply supply/reset as needed.
Observe LED result.
Inspect fixed RAM starting at:
0x20000000
```

This procedure has successfully programmed and executed the firmware.

## 19. First HC-05 AT Test

A firmware test was created to send:

`AT\r\n`

which is:

`41 54 0D 0A`

The firmware used:

`USART2->BRR = 208U;`

and sent:

```
usart2_write('A');
usart2_write('T');
usart2_write('\r');
usart2_write('\n');
```

The HC-05 was expected to answer with something equivalent to:

`OK\r\n`
## 20. Current HC-05 Test Result

The latest firmware produced:

5 LED flashes

which the firmware defines as:

`STATUS_TIMEOUT`

The fixed RAM dump was:

```
0x20000000
55415254 00000000 00000001 00002F83

0x20000010
00280000 00020000 000000D0 0000200C

0x20000020
00000000 000000D0 00000041 00000054

0x20000030
0000000D 0000000A 00000000 00000000
```

OCR formatting was noted as potentially imperfect, but the values correspond to the intended structure.

Interpretation:

`+0x00 = 0x55415254`

Magic value:

`"UART" +0x04 = 0`

Received byte count:

`0 +0x08 = 1`

Status:

`STATUS_TIMEOUT`

The transmit buffer contains:

```
0x41 = 'A'
0x54 = 'T'
0x0D = '\r'
0x0A = '\n'
```

Therefore the STM32 definitely transmitted:

`AT\r\n`

but detected:

`RX count = 0`

No byte was received from the HC-05.

## 21. Interpretation of USART Status

The latest final USART status observed was:

`0x000000D0`

Bits:

```
0x80 = TXE
0x40 = TC
0x10 = IDLE
```

Important receive/error bits were not set:

```
RXNE = 0
ORE  = 0
FE   = 0
NE   = 0
PE   = 0
```

Therefore the STM32 is not receiving corrupted data.

It is receiving nothing.

This distinction is important.

The current failure is NOT:

`"USART is receiving garbage"`

It is:

`"USART2 RX never sees a byte from the HC-05."`
## 22. Current Proven/Unproven State
Proven
STM32 side
STM32F103C8T6 is functioning.
GPIOA is functioning.
GPIOC is functioning.
PC13 onboard LED diagnostic is functioning.
PA2 works.
PA3 works.
USART2 works.
USART2 TX works.
USART2 RX works.
PA2→PA3 loopback works.
Register structures and offsets work.
Direct register manipulation works.
Fixed RAM diagnostics work.
STM32 programming/verification process works.
Current 38400 configuration works for USART2 loopback.
BRR = 208 works for the current configuration.
Hardware soldering

A physical/soldering issue affecting pin behavior was encountered and resolved.

After resolving it, the PA2 GPIO test succeeded:

`3 flashes = PASS HC-05`

**HC-05** has been previously placed into **AT mode**.
**38400** baud has previously been associated with its **AT mode**.
Current **HC-05 LED** is slow blinking.
**HC-05** has an onboard button/KEY mechanism.
## 23. Not Yet Proven

The following are NOT yet proven:

```
STM32 PA2 → HC-05 RXD
HC-05 TXD → STM32 PA3
HC-05 actually receiving the STM32's AT\r\n
HC-05 actually transmitting its OK\r\n
HC-05's current exact AT-mode state
HC-05's exact firmware/variant
HC-05's current UART configuration
HC-05's actual RX/TX electrical levels in the current physical setup
Whether the HC-05 is ready when the STM32 sends the command
Whether the HC-05 requires a different startup/key procedure in this particular module
```
## 24. Current Physical Setup

**The user reports:**

*All connections are proper.*

**Current intended wiring:**

```
STM32 PA2 / USART2_TX  -> HC-05 RXD
STM32 PA3 / USART2_RX  <- HC-05 TXD
STM32 GND              -> HC-05 GND
```

**HC-05** is separately powered.

The user reports that the **HC-05 LED** is:

*slow blinking*

The **HC-05** button exists on the corner of the board.

## 25. Important Power-Supply Decision

An old ESP32 was previously considered for supplying the **HC-05**, but the user reported:

The ESP is actually too old and gets hot too early.

**Therefore:**

***
Do NOT use that ESP32 as the HC-05 supply in further testing.

The HC-05 should use an appropriate external supply.

STM32 and HC-05 must share GND for UART.
***
## 26. Problems Encountered and Solutions
**Problem:** *LED did not behave as expected*

Initial external LED testing was confusing.

Cause included incorrect assumptions about **GPIO register location/configuration** and/or **LED hardware**.

**Resolution:**

***
*Stop relying on external LED.*

*Use PC13 onboard LED.*

*Use fixed RAM as the primary diagnostic mechanism.*
***

**Problem:** *PA2/PA3 communication appeared to fail*

**Solution:**

***
*Run direct physical loopback:*

`PA2 -> PA3`

*Result:*

`55 AA 33 CC`

was transmitted and received identically.
***

**Conclusion:**

*USART2 itself works.*

**Problem:** *USART baud register was questioned*

A value of:

`USART2->BRR = 833U;`

was tried.

It did not solve the HC-05 communication issue.

Current known-good loopback configuration uses:

`USART2->BRR = 208U;`

at the current **8 MHz USART2 clock** assumption and **38400** baud.

Do not change this randomly.

**Problem:** *HC-05 receives no apparent response*

Current result:

`RX count = 0`

STM32 sends:

`AT\r\n`

but receives nothing.

The STM32's final status does not show UART errors.

**Current hypothesis space:**

***
*HC-05 not actually in the expected AT state.*

*HC-05 startup timing issue.*

*KEY/button startup procedure differs for this module.*

*Physical TX/RX connection issue despite reported wiring correctness.*

*HC-05 firmware variant behaves differently.*

*HC-05 UART configuration differs from expected.*

*HC-05 is not receiving the command.*

*HC-05 receives it but does not respond because it is not in command mode.*
***

No single one of these should be assumed as the cause yet.

## 27. Methodology Going Forward

The project should continue using a systematic isolation process.

Do NOT repeatedly modify unrelated STM32 USART registers because the loopback has already proven the STM32 USART path.

Current boundary:

```
STM32 USART2
      |
      | PA2 / PA3
      |
      v
   HC-05
```

The STM32 side before this boundary is considered working.

The next tests should focus on the HC-05 interface.

## 28. Planned Next Test

The next intended firmware experiment is to eliminate startup timing as a variable.

Instead of:

```
wait
send AT once
wait for response
stop
```

the STM32 should repeatedly send:

`AT\r\n`

at **38400** baud.

The firmware should:

```
Initialize USART2.
Wait for HC-05 startup.
Send AT\r\n.
Wait for response.
If no response, send AT\r\n again.
Repeat several times.
Capture all incoming bytes.
Capture USART status for every received byte.
Store everything in .fixed_buf.
Stop and signal result using PC13.
```

This will distinguish:

`"HC-05 never responds"`

from:

`"first command was sent before HC-05 was ready"`
## 29. Diagnostic Expectations for Successful AT Test

If the HC-05 responds normally, expected response should contain something equivalent to:

`OK\r\n`

ASCII:

`4F 4B 0D 0A`

where:

```
4F = O
4B = K
0D = CR
0A = LF
```

The firmware should capture those bytes in fixed RAM.

Corresponding USART status values should have no:

```
PE
FE
NE
ORE
```

errors.

## 30. Current LED Result Convention

Current diagnostic convention:

```
3 flashes = SUCCESS / PASS

2 flashes = DATA RECEIVED BUT NOT EXPECTED "OK"

5 flashes = TIMEOUT / NOTHING RECEIVED
```

Earlier firmware also used:

`5 flashes = FAIL`

depending on the specific test.

Therefore, when writing future firmware, the meaning of LED patterns must be explicitly documented in that firmware.

## 31. Current Key Lesson

The most important debugging milestone so far is:

`PA2 ───── PA3`

loopback passed with:

`55 AA 33 CC`

This means the following should no longer be treated as the primary cause of the current HC-05 timeout:

1. PA2 itself
2. PA3 itself
3. USART2 peripheral
4. USART2 register mapping
5. basic GPIO configuration
6. USART2 TX
7. USART2 RX
8. current 38400 baud configuration
9. basic STM32 clock configuration

The current investigation should now concentrate on:

```
HC-05 state
        +
HC-05 UART side
        +
physical STM32↔HC-05 interface
        +
startup timing
```
## 32. Current Exact State — Short Handoff

If another assistant needs to continue this project, the essential current state is:

STM32F103C8T6 is being programmed entirely with Bare-Metal C. USART2 uses PA2 TX and PA3 RX. GPIO and USART registers are manually mapped. PA2→PA3 direct loopback has been proven with TX/RX = 55 AA 33 CC, producing 3 LED flashes. Therefore USART2 itself is working. A fixed .fixed_buf section at 0x20000000 in RAM is used for diagnostics and inspected through STM32CubeProgrammer. Current USART2 configuration is 38400 baud using BRR = 208, with the current 8 MHz clock configuration, 8N1 and no flow control.

The HC-05 is now connected with PA2→HC-05 RXD, PA3←HC-05 TXD, and common GND. HC-05 is intended to be in AT mode and its LED is currently slow blinking. A test sent AT\r\n (41 54 0D 0A) from STM32. Fixed RAM showed TX bytes correctly, but RX count = 0, and the firmware produced 5 flashes for timeout. Final USART status was 0x000000D0, with TXE/TC/IDLE set and no RX/error flags. Thus STM32 is transmitting but receiving absolutely no byte from the HC-05. The next step is to eliminate HC-05 startup timing as a variable by repeatedly sending AT\r\n and capturing any response in .fixed_buf. Do not modify the known-good STM32 USART configuration randomly.
