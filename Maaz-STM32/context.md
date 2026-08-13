# STM32 Blue Pill / STM32CubeIDE Debugging — Conversation Summary

## 1. Hardware setup

* Board: **Blue Pill-style STM32 development board**.
* Programmer/debugger: **ST-LINK V2 Blue-body dongle**.
* Connection/interface: **SWD**.
* The MCU's exact marking is unreadable.
* The user initially used **Google Lens** on a photo of the MCU, which suggested `STM32F103C8T6`, but this has **not been independently confirmed from the chip marking**.
* The board itself only has a readable `"STM32"` marking.

---

## 2. STM32CubeIDE project configuration

STM32CubeIDE version:

```text
STM32CubeIDE 2.2.0
```

Project:

```text
STM32-Third
```

Workspace:

```text
C:\Users\HP PC 30\STM32CubeIDE\workspace_2.2.0\
```

The project does **not** contain an `.ioc` file.

Project directory contains:

```text
.cproject
.project
.settings\
Debug\
Inc\
Src\
Startup\
STM32-Third Debug.launch
STM32F103C8TX_FLASH.ld
```

The `.cproject` file explicitly configures the MCU as:

```text
STM32F103C8Tx
```

This occurs in both Debug and Release configurations.

Relevant `.cproject` evidence:

```text
value="STM32F103C8Tx"
```

and:

```text
STM32 | STM32F1 | STM32F103C8Tx
```

The linker script is:

```text
STM32F103C8TX_FLASH.ld
```

The generated Makefile also explicitly uses:

```text
-T"...\STM32F103C8TX_FLASH.ld"
```

and the startup object is:

```text
./Startup/startup_stm32f103c8tx.o
```

Therefore, **the project is definitely configured/building as `STM32F103C8Tx`**.

---

## 3. Initial STM32CubeProgrammer target information

STM32CubeProgrammer successfully connects to the board through the ST-LINK.

The target information shown in the original screenshot was:

```text
Device:       STM32F101/F103 High-density MCU
Device ID:    0x414
Revision ID:  Rev Z
NVM size:     256 KB
CPU:          ARM Cortex-M3
```

This is from the **same Blue Pill currently being debugged**.

Important: the target reports:

```text
Device ID = 0x414
Flash = 256 KB
High-density F101/F103 family
```

This is inconsistent with the expected identity of a genuine `STM32F103C8T6`, which is expected to be a **64-KB medium-density** device and generally associated with device ID `0x410`.

Do **not** automatically conclude that the physical chip is counterfeit or identify it as a particular high-density STM32F103 part. The exact MCU is still unknown.

---

## 4. STM32CubeIDE debug error

When attempting to debug from STM32CubeIDE, the ST-LINK GDB server reports:

```text
STMicroelectronics ST-LINK GDB server. Version 7.14.0

Starting server with the following options:

Persistent Mode            : Disabled
Logging Level              : 1
Listen Port Number         : 61234
Status Refresh Delay      : 15s
Verbose Mode               : Disabled
SWD Debug                  : Enabled
InitWhile                  : Enabled

Error in initializing ST-LINK device.

Reason: ST-LINK: Could not verify ST device! Abort connection.
```

CubeIDE GDB connection settings:

```text
Autostart local GDB Server
Interface: SWD
Frequency: Auto
mReset Behaviour: Connect under Reset
```

These settings have **not been changed during the investigation**.

---

## 5. Very important: CubeProgrammer can program and verify the MCU

The user opened:

```text
STM32-Third.elf
```

from the project's `Debug` directory in STM32CubeProgrammer and performed **Download**.

CubeProgrammer log:

```text
14:27:21 : Opening and parsing file: STM32-Third.elf
14:27:21 : Memory Programming ...
14:27:21 : File : STM32-Third.elf
14:27:21 : Size : 56400 B
14:27:21 : Address : 0x08000000
14:27:21 : Erasing memory corresponding to segment 0:
14:27:21 : Erasing internal memory sector 0
14:27:21 : Download in Progress:
14:27:21 : Time elapsed during download operation: 00:00:00.107
```

Then CubeProgrammer reported:

```text
Verification Successfully Done
```

Interpretation:

* The ELF was successfully parsed.
* Its program contents were written into the MCU's internal Flash.
* Programming started at `0x08000000`.
* Verification succeeded.
* This proves **ST-LINK ↔ SWD ↔ MCU communication works**.
* It also proves that the MCU can be erased/programmed/verified through CubeProgrammer.
* The ELF itself is not stored as an ELF file on the MCU; its loadable sections are programmed into Flash.

Therefore, **basic ST-LINK connectivity and SWD programming are functioning**.

---

## 6. Flash-size register investigation

A memory read was performed around:

```text
0x1FFFF7E0
```

The OCR output was messy, but the important interpreted value was:

```text
0x1FFFF7E0 → 0x0100
```

`0x0100` decimal = **256 KB**.

This independently agrees with CubeProgrammer's:

```text
NVM size: 256 KB
```

The OCR output for a read beginning around `0x1FFFF7E2` included values such as:

```text
00000000
5CCA0000
00001A5A
2C2B0000
```

These values should **not currently be used for MCU identification**. The important evidence is the Flash-size value at `0x1FFFF7E0`.

---

## 7. Current evidence table

| Item                      | Result                                          |
| ------------------------- | ----------------------------------------------- |
| Board                     | Blue Pill-style board                           |
| Debugger                  | ST-LINK V2 Blue-body dongle                     |
| Debug interface           | SWD                                             |
| CubeIDE                   | 2.2.0                                           |
| GDB Server                | 7.14.0                                          |
| Project MCU configuration | **STM32F103C8Tx**                               |
| `.ioc` file               | **None present**                                |
| Linker script             | `STM32F103C8TX_FLASH.ld`                        |
| Startup file              | `startup_stm32f103c8tx.o`                       |
| CubeProgrammer connection | **Successful**                                  |
| SWD communication         | **Working**                                     |
| MCU erase                 | **Working**                                     |
| MCU programming           | **Working**                                     |
| MCU verification          | **Working**                                     |
| Target Device ID          | **0x414**                                       |
| Target reported family    | **STM32F101/F103 High-density MCU**             |
| Target reported Flash     | **256 KB**                                      |
| Target revision           | **Rev Z**                                       |
| Flash-size register       | **0x0100 = 256 KB**                             |
| CubeIDE debugging         | **Fails**                                       |
| GDB error                 | **Could not verify ST device**                  |
| Physical MCU exact part   | **Unknown**                                     |
| `STM32F103C8T6` identity  | **Only inferred by Google Lens, not confirmed** |

---

# 8. Most important discrepancy

The central issue currently under investigation is:

```text
PROJECT EXPECTS:
STM32F103C8Tx
64-KB medium-density class
        │
        │
        ▼
ACTUAL TARGET REPORTS:
STM32F101/F103 High-density MCU
Device ID = 0x414
Flash = 256 KB
Revision = Rev Z
```

This discrepancy is much more significant than the ST-LINK wiring because CubeProgrammer demonstrably communicates with and programs the target successfully.

The current leading hypothesis is therefore:

> **CubeIDE's GDB server may be rejecting the target because the project/debug configuration expects an STM32F103C8Tx while the connected MCU identifies as a high-density 256-KB F1 device.**

This remains a **hypothesis**, not a confirmed root cause.

---

# 9. Things NOT yet established

Do **not** assume any of the following:

### The physical MCU is definitely STM32F103C8T6

No. Google Lens suggested it, but the actual chip marking is unreadable.

### The MCU is definitely counterfeit

No. There is insufficient evidence for that conclusion.

### The MCU is definitely STM32F103RC/RE/etc.

No. `0x414` + 256 KB establishes the high-density class, but not the exact part number.

### ST-LINK is defective

Unlikely based on current evidence. CubeProgrammer can connect, erase, program and verify.

### The firmware is invalid

Not established. CubeProgrammer successfully verified the ELF after programming it.

---

# 10. Last requested investigation step

The next useful piece of evidence requested was the `MEMORY` section of:

```text
STM32F103C8TX_FLASH.ld
```

Specifically:

```text
MEMORY
{
    ...
}
```

The purpose is to determine the exact Flash/RAM sizes assumed by the linker script.

Expected outcome is that the C8 linker script will specify approximately **64 KB Flash**, which would further establish the mismatch:

```text
Build/linker expects ~64 KB
       vs.
physical MCU reports 256 KB
```

**Do not change the CubeIDE MCU configuration yet.** The investigation should first establish exactly what the linker script and build are configured to use.

---

# 11. Recommended next diagnostic sequence

1. Inspect the `MEMORY` section of:

   ```text
   STM32F103C8TX_FLASH.ld
   ```

2. Determine whether it specifies:

   ```text
   FLASH ORIGIN = 0x08000000
   FLASH LENGTH = 64K
   ```

   or something different.

3. If necessary, inspect the generated `.map` file to establish the actual memory layout of `STM32-Third.elf`.

4. Only after establishing the build's memory model should the project target/debug configuration be changed.

5. Do not assume that simply changing the project from `STM32F103C8Tx` to a high-density STM32F1 part is safe until the exact physical MCU is identified.

---

## One-sentence handoff summary

**A Blue Pill connected via an ST-LINK V2 can be successfully connected/programmed/verified by STM32CubeProgrammer, but the physical target reports STM32F101/F103 High-density, Device ID `0x414`, Rev Z, and 256 KB Flash, while the STM32CubeIDE 2.2.0 project is explicitly built for `STM32F103C8Tx` using `STM32F103C8TX_FLASH.ld`; CubeIDE's ST-LINK GDB Server 7.14.0 then fails at initialization with `ST-LINK: Could not verify ST device!`, so the key unresolved issue is the mismatch between the project's expected MCU and the actual MCU identity.**
