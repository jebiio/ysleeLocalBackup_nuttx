# Porting
## Sapog HW
 * STM32F105RCT6
   * 64 pins
   * Flash memory size
   * 256 Kbytes
   * [link](https://docs.google.com/presentation/d/1lS5BnGHvrTP21aQ_1nydH_BrrdOcHPnxlL9_v51UDG8/edit#slide=id.gc0d49197f5_0_0)

## build
 * nuttx build
 * apps build
 
## NuttX
 * 유사한 define 찾기
   * CONFIG_ARCH_CHIP_STM32F105RB (Flash 128Kbyte) 
   * CONFIG_ARCH_CHIP_STM32F107VC (board)

 * define
```
CONFIG_ARCH_CHIP_STM32F105RC
```

 * 추가하기

   * /nuttx/arch/arm/include/stm32/chip.h
```c++

```
   * /nuttx/arch/arm/src/stm32/hardware/stm32_pinmap.h
```c++

```
 * board 추가하기
   * nuttx/boards/arm/stm32/sapog
     * /configs/
     * /include
     * /scripts
     * /src
     * /tools
     * Kconfig
     * README.txt

# /stm32/sapog
  * /configs (config define)
    * /nsh
    * /debug
  * /include (header for sapog)
    * board.h
  * /scripts (ld script)
    * ld.script
    * ld.script.dfu
    * Make.defs
  * /src (stm32 drivers)
    * stm32 drivers
  * /tools (olimex debugger 도구)
    * ocd config
  * Kconfig (interactive kconfig를 위한 script)
  * README.md

# stm32 spec

# sapog 회로도
 * pin map
   * CAN
   * SPI
   * Motor Driver

# ToDo
 * build 확인
 * pin 바꾸기
 * memory map (datasheet)