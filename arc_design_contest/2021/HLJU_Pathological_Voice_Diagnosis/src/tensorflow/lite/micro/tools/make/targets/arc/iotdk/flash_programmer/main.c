/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>


// =======================================================
//
// IoT Development Kit board defines
//
// =======================================================
#define QSPI_BASE 0xF0003000
#define SRAM_BASE 0x30000000

typedef volatile unsigned int vu32;
#define QSPI_MMDR    (*((vu32*)(QSPI_BASE + 0x0000)))
#define QSPI_MMDW    (*((vu32*)(QSPI_BASE + 0x0004)))
#define QSPI_MMC     (*((vu32*)(QSPI_BASE + 0x0008)))
#define QSPI_MMS     (*((vu32*)(QSPI_BASE + 0x000c)))
#define QSPI_MME     (*((vu32*)(QSPI_BASE + 0x0010)))


// =======================================================
//
// Winbond W25q16 SPI Flash specific defines
//
// =======================================================
#define DUMMY_BYTE              0x00
#define INST_PAGE_PROGRAMM      0x02
#define INST_READ_STATREG_1     0x05
#define INST_WRITE_ENABLE       0x06
#define INST_BLOCK_ERASE_4K     0x20
#define INST_BLOCK_ERASE_32K    0x52
#define INST_BLOCK_ERASE_64K    0xD8
#define INST_CHIP_ERASE         0xC7

#define BLOCK_SIZE              256
#define FLASH_SIZE              0x100000    // 1M


// =======================================================
//
// Helper functions
//
// =======================================================

// Sending byte to flash control logic
//===================================================
void send_byte(uint8_t data) {
    QSPI_MMDW = data;
    while ( (QSPI_MMS & 0x02) != 0x02 )
        _nop();
}

// Finalizing transmit to flash control logic
//===================================================
void release_manual_mode() {
    QSPI_MMS = 0x80;
    while ( (QSPI_MMS & 0x01) != 0x00 )
        _nop();
}

// Wait while memory is busy (erasing/writing in progress)
//=========================================================
void wait_busy() {
    unsigned int rd_data;

    send_byte(INST_READ_STATREG_1);
    do {
        send_byte(DUMMY_BYTE);
        rd_data = QSPI_MMDR;
    } while((rd_data & 0x01) != 0x00);
    release_manual_mode();
}


// =======================================================
//
// Main programm
//
// =======================================================
int main (int argc, char **argv) {
    int ret_val = 0;
    uint32_t rd_data;
    uint8_t data_buf[BLOCK_SIZE];
    uint32_t dst_address = 0;
    uint32_t file_size;

    FILE *filein = NULL;

    // ==========================================
    // Phase 1: Check program input
    // ==========================================
    printf("\n\tSPI FLASH PROGRAMMER\n\n");
    if (argc != 3) {
        printf("Usage: app_name <file_path> <dest_address_hex>, exit\n");
        ret_val = -1; goto free_res;
    }

    rd_data = sscanf(argv[2], "%x", &dst_address);
    if(rd_data != 1 || (dst_address & 0xFFF)) {
        printf("Wrong destination address format (must be hexadecimal value multiple of 0x1000): %s\n", argv[2]);
        ret_val = -1; goto free_res;
    }

    filein = fopen(argv[1], "rb");
    if (filein == NULL) {
        printf("Can't open input file: %s\n", argv[1]);
        ret_val = -1; goto free_res;
    }

    fseek (filein, 0, SEEK_END);
    file_size=ftell (filein);
    rewind(filein);
    printf("Flash Programming parameters :\n"
            "\tInput file: %s\n"
            "\tDestination address: 0x%x \n\tFile size: %d \n"
            "\tEnd writing address: 0x%x\n\tFlash Size: 0x%x\n\n",
            argv[1], dst_address, file_size, dst_address+file_size, FLASH_SIZE);

    if(file_size + dst_address > FLASH_SIZE) {
        printf("Flash programming lead to out-of-memory writing:\n");
        ret_val = -1; goto free_res;
    }


    // ==================================================================
    // Phase 2: Tune communication with flash
    // ==================================================================
    printf("Communication with SPI FLASH is starting!\n\n");
    //MMC.Mannual Mode Chip Select (bit1) = 1'b0, CS# = 1'b0
    //MMC.Mannual Mode Speed Select (bit0) = 1'b0, the SCK output toggle at slow speed

    QSPI_MMC = 0x00;

    //Mannual Mode Enable = 1'b1
    QSPI_MME = 0x01;
    rd_data = QSPI_MME;
    if (rd_data != 0x01) {
        printf("Switch to manual mode fails!\n");
        ret_val = -1; goto free_res;
    }

    // Transmit Instruction -->> WREN (it is necessary before all erase/program command)
    send_byte(INST_WRITE_ENABLE);
    release_manual_mode();

    // Transmit Instruction -->> Read Status Register-1 Command + Dummy byte to receive data
    // Check the value of Read Status Register
    send_byte(INST_READ_STATREG_1);
    send_byte(DUMMY_BYTE);
    rd_data = QSPI_MMDR;
    release_manual_mode();
    if (rd_data != 0x02) {
        printf("Switch to Write fails! \n");
        ret_val = -1; goto free_res;
    }


    // ==================================================================
    // Phase 3: Erase required block
    // ==================================================================
    printf("Erase target memory:\n");
    uint32_t start_address = dst_address;
    uint32_t mem_size = file_size;
    uint32_t erase_bytes = 0;
    while(mem_size > 0) {
        // Assert address
        if(mem_size >= 0x10000) {
            // Can erase 64K block
            erase_bytes = 0x10000;
            send_byte(INST_BLOCK_ERASE_64K);
        } else if (mem_size >= 0x8000) {
            // Can erase 32K block
            erase_bytes = 0x8000;
            send_byte(INST_BLOCK_ERASE_32K);
        } else {
            // Can erase only 4K block
            erase_bytes = 0x1000;
            send_byte(INST_BLOCK_ERASE_4K);
        }

        send_byte((start_address >> 16) & 0xff);  // Address Byte1 A23~A16
        send_byte((start_address >> 8) & 0xff);   // Address Byte2 A15~A8
        send_byte((start_address) & 0xff);        // Byte3 A7~A0 : 0x0
        release_manual_mode();

        printf("\t 0x%.8x - 0x%.8x (%d)\n", start_address, start_address + erase_bytes, erase_bytes);
        start_address += erase_bytes;
        mem_size = (mem_size > erase_bytes)? mem_size - erase_bytes: 0;

        wait_busy();

        // Prepare for next write
        // Transmit Instruction -->> WREN (it is necessary before all erase/program command)
        if(mem_size != 0) {
            send_byte(INST_WRITE_ENABLE);
            release_manual_mode();
        }
    }
    printf("DONE! Bytes are erased: %d \n\n", start_address - dst_address);

    // ==================================================================
    // Phase 4: Sequential reading file / writing to flash
    // ==================================================================
    printf("Write file to memory:\n");
    start_address = dst_address;
    rd_data = BLOCK_SIZE;
    while (rd_data == BLOCK_SIZE) {
        // Read data from file 2 buffer (if it necessary - fill buffer tail by zeroes)
        rd_data = fread(data_buf, sizeof(data_buf[0]), BLOCK_SIZE, filein);
        if(rd_data != BLOCK_SIZE)
            memset((void *)&data_buf[rd_data], 0, BLOCK_SIZE - rd_data);

        send_byte(INST_WRITE_ENABLE);
        release_manual_mode();

        send_byte(INST_PAGE_PROGRAMM);
        send_byte((start_address >> 16) & 0xff);    // Address Byte1 A23~A16
        send_byte((start_address >> 8) & 0xff);     // Address Byte2 A15~A8
        send_byte((start_address) & 0xff);          // Byte3 A7~A0 : 0x0

        // Transmit Data in the same transaction
        for (uint32_t idx = 0 ; idx < BLOCK_SIZE; ++idx )
            send_byte(data_buf[idx]);
        release_manual_mode();

        if((start_address & 0xFFFF) == 0)
            printf("\nBlock at 0x%.8x \nPages: %.2x", start_address, (start_address >> 8) & 0xFF);
        else
            printf("  %.2x", (start_address >> 8) & 0xFF);
        if((start_address & 0xFFF) == 0)
            fflush (stdout);
        start_address += BLOCK_SIZE;

        wait_busy();
    }
    printf("\n\n DONE! Bytes are written: %d\n", start_address - dst_address);

    // ==================================================================
    // Phase 5: Free resources and program end
    // ==================================================================
free_res:
    if (filein != NULL)
        fclose(filein);

    return ret_val;
} // main





