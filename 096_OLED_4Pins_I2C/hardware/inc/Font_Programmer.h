/****************************************************************************/ /**
 * @file   Font_Programmer.h
 * @brief  
 * 
 * @author Maverick Pi
 * @date   2025-12-16 16:56:19
 ********************************************************************************/

#ifndef __FONT_PROGRAMMER_H__
#define __FONT_PROGRAMMER_H__

#include "W25Q64.h"
#include "Serial.h"

#define FONT_PROGRAMMER_W25Q64_START_ADDR       0x000000
#define FONT_PROGRAMMER_BUFFER_SIZE             256

void Font_Programmer_CH(void);

#endif // !__FONT_PROGRAMMER_H__