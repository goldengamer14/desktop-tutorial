# STM32F103 + HC-05 Project Context — Context 3

## Purpose of this file

This is the **third context handoff** for the STM32/HC-05 project.

It is intended to be appended after the existing:

- `context.md`
- `context_2.md`

It therefore focuses on **new progress since the previous context**, especially the completed Bluetooth data-link milestone, the hardware fault that was ultimately found, the tests that led to it, the final working configuration, and the user's accumulated takeaways.

It intentionally does **not** repeat the earlier explanations of basic memory-mapped GPIO, register structures, `volatile`, RCC, GPIO CRL/CRH, BSRR/BRR, etc. Those remain in the previous context files.

---

# 1. Major milestone reached

The fundamental STM32 ↔ HC-05 ↔ phone communication path has now been successfully demonstrated.

The working path is:

```
Phone
  │
  │ Bluetooth Classic / SPP
  ▼
HC-05
  │
  │ UART
  ▼
STM32F103
  │
  │ USART2
  ▼
PA3 / RX
```

and in the reverse direction:

```
STM32 PA2 / USART2_TX
        │
        ▼
      HC-05
        │
        ▼
 Bluetooth Classic / SPP
        │
        ▼
      Phone
```

The user explicitly confirmed that **Bluetooth data sending and response receiving works as expected**.

The final practical failure was traced to **loose jumper wires on the STM32 PA2/PA3 connections**. After fixing the loose connections, the communication path worked reliably.

This is the most important completion milestone of the current project stage.

---

# 2. Major change in the HC-05 hardware

The original HC-05 module was replaced because the original module was suspected of having a hardware problem.

The replacement HC-05 breakout has a different pin layout:

```
STATE
RXD
TXD
VCC
GND
EN
```

For the current normal-data-mode application, only these pins are needed:

```
HC-05 RXD -> STM32 PA2 / USART2_TX
HC-05 TXD -> STM32 PA3 / USART2_RX
HC-05 VCC -> appropriate supply
HC-05 GND -> common ground
```

`STATE` and `EN` are not required for the current basic data test.

`EN` is relevant to HC-05 command/AT-mode control, but the project has now moved beyond the AT-command experiment and into normal Bluetooth data communication.

---

# 3. AT-mode investigation was ultimately bypassed

A substantial part of the previous debugging phase attempted to make the original HC-05 answer:

```
AT\r\n
```

with an expected:

```
OK\r\n
```

The STM32 successfully transmitted the command, and several diagnostic firmware versions stored the received bytes and USART status in the fixed RAM section.

However, the original module did not give a reliable useful AT response.

The project therefore shifted away from treating `OK` as a prerequisite for success.

This was an important change in project strategy:

> The real goal is not proving the HC-05's AT command interface. The real goal is proving the application data path: phone → HC-05 → STM32 and STM32 → HC-05 → phone.

Once a replacement HC-05 was used and the Bluetooth SPP connection was successfully established, the project could move directly to the actual desired communication.

---

# 4. Important USART baud-rate distinction learned

Two baud rates became relevant:

## AT/command mode

The project previously used:

```
USART2->BRR = 208U;
```

for the assumed:

```
USART2 clock = 8 MHz
baud = 38400
```

This configuration passed the STM32 USART2 loopback test.

## Normal HC-05 Bluetooth data mode

For the normal Bluetooth data-mode experiment, the project switched to:

```
USART2->BRR = 833U;
```

corresponding approximately to:

```
USART2 clock = 8 MHz
baud = 9600
```

This distinction became important:

> A failed 833-baud-register test during the earlier AT-mode investigation was not evidence that 833 was wrong in general. It was the wrong context because that earlier test was targeting the AT-mode UART configuration.

Once the project switched to normal Bluetooth data mode, 9600 baud / BRR 833 became the relevant configuration.

---

# 5. STM32 USART2 remained unchanged at the hardware level

The USART2 implementation that was proven by the earlier loopback test remained the foundation.

Current pin mapping:

```
PA2 = USART2_TX
PA3 = USART2_RX
```

Correct external wiring:

```
STM32 PA2 / USART2_TX -> HC-05 RXD
STM32 PA3 / USART2_RX <- HC-05 TXD
STM32 GND              -> HC-05 GND
```

The core USART2 register setup remained:

```
USART2->BRR = 833U;
USART2->CR2 = 0U;
USART2->CR3 = 0U;

USART2->CR1 =
      USART_CR1_UE
    | USART_CR1_TE
    | USART_CR1_RE;
```

for the final normal-data-mode 9600-baud test.

---

# 6. The USART2 loopback test remains a key verification milestone

Before involving Bluetooth, a direct STM32 USART2 loopback was successfully completed.

Physical connection:

```
PA2 ───── PA3
```

The firmware transmitted:

```
55 AA 33 CC
```

and received exactly:

```
55 AA 33 CC
```

The corresponding diagnostic test produced the success LED pattern.

This established that:

- PA2 works as USART2 TX.
- PA3 works as USART2 RX.
- USART2 transmit works.
- USART2 receive works.
- The register definitions are usable.
- The GPIO alternate-function configuration works.
- The 8 MHz clock assumption used for these UART calculations is consistent with the working loopback.
- The current bare-metal USART implementation is fundamentally sound.

Therefore, once Bluetooth communication later failed intermittently, the USART implementation itself was not automatically assumed to be defective.

---

# 7. The physical jumper problem

One of the most important practical lessons from this stage was that the software/debugging symptoms were ultimately caused by **physical jumper-wire connectivity**.

The STM32 PA2/PA3 jumper connections were initially loose.

This caused symptoms that looked like:

- UART failure
- no received bytes
- inconsistent terminal behavior
- apparently unreliable HC-05 communication

A large amount of firmware-level debugging was performed before the physical connection issue was finally recognized and fixed.

After securing the jumper connections:

> The Bluetooth data path worked.

This should be treated as an explicit project lesson:

> Before changing working register-level firmware, verify the physical connection, especially breadboard/jumper continuity.

The previous context already records an earlier soldering/header-contact problem; this stage revealed a second physical-connectivity problem involving loose jumper wires.

---

# 8. Why the earlier diagnostic process still mattered

Although the debugging took longer than necessary, it produced several valuable verified checkpoints.

The investigation repeatedly isolated layers:

```
Physical pin
   ↓
GPIO
   ↓
USART2
   ↓
HC-05 UART
   ↓
Bluetooth SPP
   ↓
Phone
```

Tests were deliberately moved upward one layer at a time.

This eventually allowed the final failure to be separated from:

- STM32 register definitions
- GPIO configuration
- USART2 configuration
- baud-rate calculation
- Bluetooth pairing
- terminal software
- HC-05 hardware
- physical jumper connectivity

This layered-debugging approach should be retained for future projects.

---

# 9. Fixed RAM diagnostics were an important debugging tool

The project developed a practical post-mortem debugging mechanism using the linker script's fixed RAM section:

```
.fixed_buf (NOLOAD) :
{
    . = ALIGN(4);
    KEEP(*(.fixed_buf))
} > RAM
```

Objects were placed in that region using:

```
__attribute__((section(".fixed_buf"), used))
```

The buffer was inspected through STM32CubeProgrammer beginning at:

```
0x20000000
```

This technique was used to inspect:

- number of received bytes
- raw received bytes
- USART status register values
- RCC register values
- USART BRR
- USART CR1/CR2/CR3
- diagnostic status codes

This was particularly useful because no PC serial-debug interface was initially attached to the STM32.

The method taught an important bare-metal debugging principle:

> When there is no console, the MCU's own RAM can act as a post-mortem trace buffer.

---

# 10. Failed AT-mode diagnosis and what it taught

The previous HC-05 produced several unhelpful results during AT-mode testing.

At one point the fixed RAM diagnostic showed:

```
RX count = 0
status = timeout
```

while the STM32 definitely transmitted:

```
41 54 0D 0A
```

Another test captured a small number of unexpected bytes and USART status flags including framing/error-related conditions.

This led to checking:

- USART clock
- APB1 prescaler
- BRR
- USART status flags
- HC-05 AT mode
- HC-05 LED state
- TX/RX wiring
- power
- module behavior

The final replacement of the HC-05 showed that the original hardware/module was indeed a plausible source of the earlier uncertainty.

The project therefore learned:

> If a peripheral module is suspected of hardware failure, replacing it can be more informative than endlessly modifying the microcontroller firmware.

---

# 11. Phone-side Bluetooth testing

The phone-side investigation reached these milestones:

1. The HC-05 became discoverable.
2. Pairing was possible with the Samsung Galaxy J8 using the passkey `1234`.
3. The original module had trouble establishing the SPP terminal connection.
4. A second phone (Nothing phone) behaved differently and did not connect reliably.
5. The HC-05 module was replaced.
6. With the replacement module, the Bluetooth terminal successfully connected.
7. Data could be sent from the phone.
8. The STM32 could generate a response that appeared in the Bluetooth terminal.

The Samsung Galaxy J8 served as the working reference phone for the test.

The phone/terminal layer therefore became a useful independent test of the HC-05 before relying on STM32-side diagnostics.

---

# 12. First successful phone-to-STM32 data experiment

With the replacement HC-05 and normal Bluetooth data mode:

```
Phone
  ↓
Bluetooth Terminal
  ↓
HC-05
  ↓
UART @ 9600
  ↓
STM32 USART2
```

The user sent:

```
OK
```

The STM32 successfully received data.

The STM32 also transmitted a response back through:

```
USART2 TX → HC-05 → Bluetooth → phone
```

This was the first direct demonstration of the actual desired data path.

---

# 13. Final application-style output

The firmware was refined so that the phone's terminal could display a human-readable response rather than raw bytes.

The desired output format became:

```
Received OK
```

The startup message displayed after Bluetooth connection was:

```
STM32 USART2 READY
Send text and press ENTER.
```

The response logic was changed so that received characters are collected and then displayed as:

```
Received {input}
```

For example:

Input:

```
OK
```

Response:

```
Received OK
```

This is the first version that resembles an actual application protocol rather than a raw UART diagnostic.

---

# 14. Intermittent failure after reset

After the first successful data exchange, a subsequent reset sometimes produced only the startup message:

```
STM32 USART2 READY
Send text and press ENTER.
```

while a new message did not produce the expected:

```
Received OK
```

This initially suggested an intermittent firmware/terminal issue.

However, the later investigation determined that the real practical cause was:

> **Loose jumper cables on the STM32 A2/A3 connections.**

After fixing the physical connections, the Bluetooth data sending and response receiving path worked as expected.

Therefore the intermittent post-reset failure should not be treated as evidence that the final UART firmware is fundamentally broken.

---

# 15. Final working architecture

The current proven architecture is:

```
                    ┌────────────────────┐
                    │       PHONE        │
                    │ Bluetooth Terminal │
                    └─────────┬──────────┘
                              │
                       Bluetooth Classic
                              │
                              ▼
                    ┌────────────────────┐
                    │       HC-05        │
                    │                    │
                    │ RXD       TXD      │
                    └───┬────────┬───────┘
                        │        │
                UART TX │        │ UART RX
                        │        │
                        ▼        ▼
                    PA2          PA3
                 USART2_TX    USART2_RX
                        │        │
                        └──┬─────┘
                           │
                    ┌──────▼──────┐
                    │ STM32F103   │
                    │ Bare Metal  │
                    └─────────────┘
```

Normal data-mode UART configuration:

```
9600 baud
8 data bits
No parity
1 stop bit
No hardware flow control
```

---

# 16. Current overall project status

## Hardware

- STM32 board: working
- ST-LINK/SWD: working
- STM32 programming: working
- PC13 diagnostic LED: working
- PA2: working
- PA3: working
- Replacement HC-05: working sufficiently for Bluetooth SPP data
- Bluetooth phone connection: working
- Physical jumper issue: found and fixed

## Firmware

- Bare-metal GPIO: working
- Bare-metal USART2: working
- USART2 register abstraction: working
- 38400 loopback configuration: proven
- 9600 normal-data-mode configuration: working
- RX buffer handling: working
- TX response: working
- Human-readable response: working

## Bluetooth

- HC-05 discoverability: working on the replacement module
- Pairing: working on the tested phone
- Serial Bluetooth Terminal connection: working on the replacement module
- Phone → HC-05 → STM32 data: working
- STM32 → HC-05 → phone response: working

## Overall milestone

> **The first real end-to-end STM32 ↔ HC-05 ↔ phone data link is complete.**

---

# 17. What is intentionally NOT considered solved yet

The AT-command configuration path is not considered an essential completed feature.

We do not yet need to rely on:

- dynamic AT configuration
- STATE pin
- EN/KEY pin
- changing Bluetooth name programmatically
- changing PIN programmatically
- changing baud dynamically

The immediate objective was successfully completed without requiring those features.

These can be learned later as a separate HC-05 configuration topic.

---

# 18. What should happen next

Now that the basic Bluetooth serial link is working, the natural progression is:

## Step 1 — Make the serial protocol reliable

Move from a simple test program toward:

```
RX bytes
   ↓
RX buffer
   ↓
message framing
   ↓
command parser
   ↓
application logic
   ↓
response
```

Learn:

- message boundaries
- buffers
- ring buffers
- overflow handling
- CR/LF handling
- printable/non-printable data
- binary vs text protocols

## Step 2 — USART interrupts

Replace polling:

```
while (!(USART2->SR & USART_SR_RXNE))
{
}
```

with:

- RX interrupt
- NVIC
- ISR
- receive buffer
- main-loop processing

This is the natural next Bare-Metal USART lesson.

## Step 3 — Later DMA

After interrupts are understood:

- USART RX DMA
- USART TX DMA
- circular buffers
- efficient high-throughput communication

## Step 4 — Later FreeRTOS

Only after the bare-metal UART path is comfortable:

- task for Bluetooth communication
- queue for incoming messages
- task for application processing
- synchronization

## Step 5 — Return to the original networking project

The HC-05 project is a **serial/Bluetooth learning milestone**, not the final Wi-Fi bridge.

The original larger project requires a much higher-bandwidth networking interface.

The next eventual technologies to investigate are:

- ESP32 Wi-Fi
- Ethernet
- Wi-Fi bridge/mesh architecture
- TCP/IP
- routing/bridging
- FreeRTOS networking
- eventually Linux networking if a Linux-capable processor is used

---

# 19. User's takeaways and learnings

## 19.1 A pin is not a register

A GPIO pin such as PA2 or PA3 is a physical I/O line represented through bits in peripheral registers.

This was reinforced by the debugging process.

A pin is not its own 32-bit memory location.

---

## 19.2 Register addresses and bit masks are different concepts

An address answers:

> Where is the register?

A mask answers:

> Which bit(s) inside the register do I care about?

Example:

```
GPIOA->CRL
```

is the register.

```
(1U << 2)
```

is a bit mask.

This distinction is foundational to Bare-Metal programming.

---

## 19.3 Peripheral clocks matter

Before using GPIO or USART peripherals, their clocks must be enabled through RCC.

The existence of a peripheral at a memory address does not mean it is immediately usable.

---

## 19.4 GPIO configuration is separate from GPIO state

Configuring a pin as an output and setting it HIGH are two different operations.

For STM32F1:

```
CRL / CRH
```

controls the pin configuration, while:

```
ODR / BSRR / BRR
```

controls the GPIO output state.

---

## 19.5 CRL vs CRH matters

For STM32F1:

```
PA0–PA7   -> CRL
PA8–PA15  -> CRH
```

A mistake here caused misleading GPIO results during early testing.

---

## 19.6 BSRR and BRR are action registers

They should be understood as commands:

```
BSRR = set bit
BRR  = clear bit
```

not as ordinary variables.

Therefore direct assignment is preferred:

```
GPIOC->BSRR = mask;
GPIOC->BRR  = mask;
```

rather than read-modify-write operations.

---

## 19.7 `volatile` matters for hardware registers

Peripheral registers can change because hardware changes them.

The compiler therefore needs to be told not to optimize away or invent assumptions about those accesses.

---

## 19.8 Structures can model hardware registers

The project moved from raw addresses to structures:

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

This made peripheral access readable while remaining fully Bare-Metal.

---

## 19.9 Static assertions can protect hardware mappings

The project used `_Static_assert` and `offsetof()` to verify that structure members actually correspond to the hardware register offsets.

This is a valuable technique for low-level C.

---

## 19.10 UART is simply a physical data path underneath the protocol

The project learned that:

```
Phone
 ↓
Bluetooth
 ↓
HC-05
 ↓
UART
 ↓
STM32
```

does not require the STM32 to understand Bluetooth itself.

The HC-05 handles the Bluetooth side and exposes serial bytes to the MCU.

Similarly, the MCU can transmit bytes back without knowing anything about the Bluetooth radio protocol.

---

## 19.11 AT mode and data mode are different concepts

The HC-05 can expose:

- a command/configuration interface
- normal Bluetooth serial data

The AT command experiment was useful, but it is not the same as actual application data transmission.

---

## 19.12 A working loopback does not prove an external device is configured correctly

The PA2→PA3 loopback proved:

```
USART2 TX works
USART2 RX works
```

but it could not by itself prove that an external HC-05 was configured to the same baud.

This distinction was learned during the AT-mode debugging.

---

## 19.13 Error symptoms must be interpreted carefully

A timeout means:

> No byte was observed.

A framing error means:

> A byte/frame was observed, but the UART timing/framing did not match.

A valid received byte means:

> The physical/data path got far enough for the USART peripheral to decode something.

These are different failure classes.

---

## 19.14 Physical hardware faults can imitate software bugs

This became one of the strongest practical lessons.

The final intermittent Bluetooth failure was ultimately caused by:

> loose jumper wires on PA2/PA3.

The firmware was modified and diagnosed extensively before the physical connection was fixed.

Takeaway:

> Always verify wiring, solder joints, jumper cables, power, and common ground before rewriting working firmware.

---

## 19.15 Layered debugging is extremely effective

The project naturally converged to this testing hierarchy:

```
Physical pin
    ↓
GPIO
    ↓
USART2 loopback
    ↓
HC-05 UART
    ↓
Bluetooth connection
    ↓
Phone terminal
    ↓
Application-level message
```

A layer should be considered working only after a test proves it.

This is now the preferred debugging methodology for future embedded work.

---

# 20. Final achievement statement

The project has progressed from:

> "I have an STM32 board and don't know its pins."

through:

- register mapping
- GPIO
- RCC
- memory-mapped structures
- PC13 LED control
- direct GPIO testing
- PA2/PA3 testing
- USART2 register programming
- USART loopback
- baud-rate configuration
- fixed RAM debugging
- HC-05 AT-mode investigation
- replacement of a suspect HC-05
- Bluetooth pairing
- SPP terminal connection
- phone-to-MCU data reception
- MCU-to-phone response transmission
- physical jumper debugging

to:

```
Phone
  ↓
Bluetooth
  ↓
HC-05
  ↓
USART2
  ↓
STM32F103
  ↓
USART2
  ↓
HC-05
  ↓
Bluetooth
  ↓
Phone
```

with actual application-style messages such as:

```
Received OK
```

working as intended.

**The basic STM32F103 ↔ HC-05 Bluetooth serial communication milestone is therefore complete.**

The next project stage should focus on making the UART communication robust and properly structured using interrupts/ring buffers, rather than continuing to diagnose the already-working Bluetooth link.
