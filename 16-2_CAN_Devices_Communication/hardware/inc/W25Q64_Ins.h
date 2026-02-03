/****************************************************************************/ /**
 * @file   W25Q64_Ins.h
 * @brief  W25Q64 Flash Memory instruction set definitions
 * 
 * @author Maverick Pi
 * @date   2025-11-20 21:46:27
 ********************************************************************************/

#ifndef __W25Q64_INS_H__
#define __W25Q64_INS_H__

// Standard SPI Instructions for W25Q64
#define W25Q64_WRITE_ENABLE                 0x06
#define W25Q64_WRITE_DISABLE                0x04
#define W25Q64_READ_STATUS_REG1             0x05
#define W25Q64_READ_STATUS_REG2             0x35
#define W25Q64_WRITE_STATUS_REG             0x01
#define W25Q64_PAGE_PROGRAM                 0x02
#define W25Q64_SECTOR_ERASE                 0x20
#define W25Q64_BLOCK_ERASE_32K              0x52
#define W25Q64_BLOCK_ERASE_64K              0xD8
#define W25Q64_CHIP_ERASE                   0xC7
#define W25Q64_POWER_DOWN                   0xB9
#define W25Q64_RELEASE_POWER_DOWN           0xAB
#define W25Q64_DEVICE_ID                    0x90
#define W25Q64_JEDEC_ID                     0x9F
#define W25Q64_READ_DATA                    0x03
#define W25Q64_FAST_READ                    0x0B
#define W25Q64_ERASE_PROGRAM_SUSPEND        0x75
#define W25Q64_ERASE_PROGRAM_RESUME         0x7A
#define W25Q64_READ_UNIQUE_ID               0x4B
#define W25Q64_READ_SFDP_REGISTER           0x5A
#define W25Q64_ERASE_SECURITY_REGS          0x44
#define W25Q64_PROGRAM_SECURITY_REGS        0x42
#define W25Q64_READ_SECURITY_REGS           0x48
#define W25Q64_ENABLE_QPI                   0x38
#define W25Q64_ENABLE_RESET                 0x66
#define W25Q64_RESET                        0x99

// Dual SPI Instructions for W25Q64
#define W25Q64_FAST_READ_DUAL_OUTPUT        0x3B
#define W25Q64_FAST_READ_DUAL_IO            0xBB

// Quad SPI Instructions for W25Q64
#define W25Q64_QUAD_PAGE_PROGRAM            0x32
#define W25Q64_FAST_READ_QUAD_OUTPUT        0x6B
#define W25Q64_FAST_READ_QUAD_IO            0xEB
#define W25Q64_WORD_READ_QUAD_IO            0xE7
#define W25Q64_OCTAL_WORD_READ_QUAD_IO      0xE3
#define W25Q64_SET_BURST_WITH_WRAP          0x77

#define W25Q64_DUMMY_BYTE                   0xFF

#endif // !__W25Q64_INS_H__
