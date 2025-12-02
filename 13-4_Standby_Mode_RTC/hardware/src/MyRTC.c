/****************************************************************************/ /**
 * @file   MyRTC.c
 * @brief  Implementation of Real-Time Clock (RTC) functions for STM32F10x
 * 
 * @author Maverick Pi
 * @date   2025-11-30 22:51:32
 ********************************************************************************/

#include "MyRTC.h"

/* Current timezone offset (default: UTC+8) */
static int8_t current_timezone = 8;

/**
 * @brief  Initialize RTC peripheral and backup domain
 *         Configures LSE as RTC clock source and sets up prescaler
 *         Restores timezone from backup register or uses default
 */
void MyRTC_Init(void)
{
    /* Enable power and backup interface clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    /* Enable write access to backup registers */
    PWR_BackupAccessCmd(ENABLE);

    if (((BKP_ReadBackupRegister(TIMEZONE_BKP_REGISTER) >> 8) & 0xFF) != TIMEZONE_MAGIC_NUMBER) {
        RCC_LSEConfig(RCC_LSE_ON);
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);

        RTC_WaitForSynchro();
        RTC_WaitForLastTask();

        RTC_SetPrescaler(32767);    // Set RTC prescaler for 1Hz clock (32768Hz LSE / 32767 = ~1Hz)
        RTC_WaitForLastTask();

        MyRTC_Time setTime = { 2025, 12, 1, 0, 0, 0 };
        MyRTC_SetTime(&setTime);
    } else {
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
    }

    /* Load timezone from backup register */
    MyRTC_LoadTimeZoneFromBKP();
}

/**
 * @brief  Set RTC to specified date and time
 * @param  myTime Pointer to MyRTC_Time structure containing target time
 */
void MyRTC_SetTime(const MyRTC_Time *myTime)
{
    time_t timeCnt;
    struct tm dateTime;

    /* Convert MyRTC_Time to standard struct tm */
    dateTime.tm_year = myTime->year - 1900;     // tm_year is years since 1900
    dateTime.tm_mon  = myTime->month - 1;   // tm_mon is 0-11
    dateTime.tm_mday = myTime->day;
    dateTime.tm_hour = myTime->hour;
    dateTime.tm_min  = myTime->minute;
    dateTime.tm_sec  = myTime->second;

    /* Convert to Unix timestamp and adjust for timezone */
    timeCnt = mktime(&dateTime) - current_timezone * 3600;

    /* Set RTC counter value */
    RTC_SetCounter((uint32_t)timeCnt);
    RTC_WaitForLastTask();
}

/**
 * @brief  Get current date and time from RTC
 * @return MyRTC_Time structure containing current date and time
 */
MyRTC_Time MyRTC_GetTime(void)
{
    time_t timeCnt;
    struct tm *dateTime;
    MyRTC_Time myTime;

    /* Get RTC counter value and adjust for timezone */
    timeCnt = (time_t)RTC_GetCounter() + current_timezone * 3600;
    /* Convert to local time structure */
    dateTime = localtime(&timeCnt);

    /* Convert from struct tm to MyRTC_Time */
    myTime.year   = dateTime->tm_year + 1900;
    myTime.month  = dateTime->tm_mon + 1;
    myTime.day    = dateTime->tm_mday;
    myTime.hour   = dateTime->tm_hour;
    myTime.minute = dateTime->tm_min;
    myTime.second = dateTime->tm_sec;

    return myTime;
}

/**
 * @brief  Save current timezone to backup register with validation magic number
 */
void MyRTC_SaveTimeZoneToBKP(void)
{
    /* Pack magic number and timezone into 16-bit value */
    uint16_t bkpZoneData = (TIMEZONE_MAGIC_NUMBER << 8) | (current_timezone & 0xFF);
    /* Write to backup register */
    BKP_WriteBackupRegister(TIMEZONE_BKP_REGISTER, bkpZoneData);
}

/**
 * @brief  Load timezone from backup register, validate with magic number
 *         If invalid, set to default timezone (UTC+8) and save to backup
 */
void MyRTC_LoadTimeZoneFromBKP(void)
{
    uint16_t bkpZoneData = BKP_ReadBackupRegister(TIMEZONE_BKP_REGISTER);
    uint8_t magic = (bkpZoneData >> 8) & 0xFF;
    int8_t zone = (int8_t)(bkpZoneData & 0xFF);

    /* Validate magic number and timezone range */
    if (magic == TIMEZONE_MAGIC_NUMBER && zone >= -12 && zone <= 14) {
        current_timezone = zone;
    } else {
        /* Invalid data - use default timezone (UTC+8) */
        current_timezone = 8; // Default timezone
        /* Save default to backup register */
        MyRTC_SaveTimeZoneToBKP();
    }
}

/**
 * @brief  Set current timezone offset and save to backup register
 * @param  timeZone Timezone offset in hours (-12 to +14)
 */
void MyRTC_SetTimeZone(int8_t timeZone)
{
    if (timeZone < -12 || timeZone > 14) {
        return;
    }
    current_timezone = timeZone;
    MyRTC_SaveTimeZoneToBKP();
}

/**
 * @brief  Get current timezone offset
 * @return Current timezone offset in hours
 */
int8_t MyRTC_GetTimeZone(void)
{
    return current_timezone;
}
