

/****************************************************************************
 * myled/main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>

#include <arch/board/board.h>
#include <sched.h>

/****************************************************************************
 * Defines
 ****************************************************************************/
#define DEFAULT_PRIORITY 	100
#define DEFAULT_STACK_SIZE 	1024

#define LED_NUMBER				4
#define LED_NUMBER_MAX			8
#define LED_PATTERN_NUMBER		6

#define CONTINUOUS				0

#define OFF			0b000
#define BLUE		0b001
#define GREEN		0b010
#define SKY			0b011
#define RED			0b100
#define MAGENTA		0b101
#define YELLOW		0b110
#define WHITE		0b111

#define BRIGHTNESS_DEFAULT	10
#define BRIGHTNESS_MIN		1
#define BRIGHTNESS_MAX		0xFF

#define CONFIG_EXAMPLES_WS2812_DEVNAME "/dev/leddrv0"
#define US_DELAY  1000*20
#define LED_SEM_NAME	"ledSem"
#define LPSPI1				1

#ifndef MAX_NSEC
#define MAX_NSEC	 			999999999
#endif

#define COLOR_TO_RGB(COLOR, BRIGHTNESS)	\
	((((0b100&COLOR)>>2)*BRIGHTNESS) << 16)+	\
	((((0b010&COLOR)>>1)*BRIGHTNESS) << 8)+	\
	((0b001&COLOR)*BRIGHTNESS)

#define MSEC_TO_SEC(MS) MS/1000
#define MSEC_TO_NSEC(MS) (MS%1000)*1000*1000
/****************************************************************************
 * Types
 ****************************************************************************/
typedef enum
{
	NO_REQUEST, BATT_REM, FAULT, SELF_DISCHARGE, LED_OFF, CHARGING
} ledState_t;
ledState_t gRequest, gMainState = NO_REQUEST;
ledState_t gOldState = LED_OFF;

pthread_mutex_t gLEDTaskLock;

uint32_t gRGBBuf[LED_NUMBER];
bool ledTaskInitialized = false;

struct ledPattern_s
{
	uint8_t color[LED_NUMBER];
	bool blink[LED_NUMBER];
	uint32_t durationMs;
	uint32_t onTimeMs;
	uint32_t offTimeMs;
	bool nextToggle

} gLEDPattern;
typedef struct ledPattern_s ledPattern_t;
ledPattern_t gLEDPattern, ledPattern[LED_PATTERN_NUMBER];

struct spi_dev_s *ledSPI;
int fd;
struct itimerspec blinkTimerSpec, timeoutTimerSpec;
timer_t blinkTimerId, timeoutTimerId;

int gButton;

/****************************************************************************
 * Public Functions
 ****************************************************************************/
//static int ledTaskFunc(int argc, char *argv[]);

// void setOneLEDSigHdlr(int signo, siginfo_t *siginfo, void *arg);

void pushBuffer(uint32_t * buf);

// int calculateToggleTime(void);

// int checkNeedToggle(int ledIndex);

// int checkTimeout(struct timespec c, struct timespec w);

// void addMsToClock(int ms, struct timespec* c);

// int setSteadyLED(uint8_t* c, bool* b, uint32_t ont, uint32_t offt);

// int setTimedLED(uint8_t* c, bool* b, uint32_t ont, uint32_t offt, uint32_t d);
void initLEDPattern(void);

void setLEDPattern(ledState_t ls);

void packLEDPattern(uint32_t* buf, ledPattern_t lp);

int disarmTimer(timer_t tid, struct itimerspec ts);

int setTimer(ledPattern_t lp);

int armTimerOnce(timer_t tid, struct itimerspec ts, uint32_t ms);

int led_setLED(ledState_t r);

static void blinkSignalHandlerFunc(int signo, siginfo_t *siginfo, void *arg);

static void timeoutSignalHandlerFunc(int signo, siginfo_t *siginfo, void *arg);

int sw2ISR(int irq, FAR void *context, FAR void *arg);