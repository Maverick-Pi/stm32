/****************************************************************************/ /**
 * @file   MyRTC.h
 * @brief  Header file for Real-Time Clock (RTC) module for STM32F10x.
 *         Provides timekeeping functionality with timezone support and backup storage.
 * 
 * @author Maverick Pi
 * @date   2025-11-30 22:51:15
 ********************************************************************************/

#ifndef __MYRTC_H__
#define __MYRTC_H__

#include "stm32f10x.h"
#include <time.h>

/* Backup register used for storing timezone information */
#define TIMEZONE_BKP_REGISTER BKP_DR1
/* Magic number to validate stored timezone data */
#define TIMEZONE_MAGIC_NUMBER 0x5A

/**
 * @brief Structure representing calendar date and time
 * 
 */
typedef struct {
    uint16_t year;      // Year (e.g., 2025)
    uint8_t month;      // Month (1-12)
    uint8_t day;        // Day of month (1-31)
    uint8_t hour;       // Hour (0-23)
    uint8_t minute;     // Minute (0-59)
    uint8_t second;     // Second (0-59)
} MyRTC_Time;

/* Function prototypes */

void MyRTC_Init(void);
void MyRTC_SetTime(const MyRTC_Time *myTime);
MyRTC_Time MyRTC_GetTime(void);
void MyRTC_SetTimeZone(int8_t timeZone);
void MyRTC_SaveTimeZoneToBKP(void);
void MyRTC_LoadTimeZoneFromBKP(void);
int8_t MyRTC_GetTimeZone(void);

#endif // !__MYRTC_H__
