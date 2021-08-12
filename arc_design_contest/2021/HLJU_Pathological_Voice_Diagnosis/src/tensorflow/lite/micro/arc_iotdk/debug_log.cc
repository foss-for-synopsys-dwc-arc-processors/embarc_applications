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

#include "tensorflow/lite/micro/debug_log.h"

#include <cstdint>
#include <cstdio>
#include <cstring>

// Print to debug console by default. One can define next to extend destinations
// set: IOTDK_LOG_TO_MEMORY
//   : fill .debug_log memory region (data section) with passed chars.
// IOTDK_LOG_TO_HOST
//   : Use MetaWare HostLink to print output log. Requires Synopsys MetaWare
//   debugger
// IOTDK_LOG_TO_UART
//   : use default debug UART (out to FTDI channel 0). The same USB Port is used
//   for JTAG.
#define IOTDK_LOG_TO_UART

// Memory size for symbols dump in IOTDK_LOG_TO_MEMORY destination
#define IOTDK_LOG_TO_MEMORY_SIZE (2 * 1024)

// IOTDK Debug UART related defines (registers and bits)
#define IOTDK_DBG_UART_BASE (0x80014000U)
#define UART_THR (0x00U)       /* DFSS UART Transmit Holding Register offset*/
#define UART_LSR (0x14U)       /* DFSS UART Line Status Register offset */
#define UART_LSR_TEMT (1 << 6) /* LSR:Transmitter Empty Bit  */

static inline void aux_reg_write(uint32_t val, uint32_t adr) {
  // Write a `val` to auxiliary register with adress `adr`
  _sr(val, adr);
}

static inline uint32_t aux_reg_read(uint32_t adr) { 
    // Read a value from auxiliary register with adress `adr` and return it
    return (uint32_t)_lr(adr); 
}


// For simplicity we assume U-boot has already initialized debug console
// during application loading (or on reset). Hence, we use only status and
// data registers to organize blocking loop for printing symbols. No input
// and no IRQ handling. See embarc_osp repository for full DFSS UART driver.
// (https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp)
void DbgUartSendStr(const char* s) {
  const char* src = s;
  while (*src) {
    // Check uart line status to transmit symbol
    uint32_t LSR_reg = aux_reg_read(IOTDK_DBG_UART_BASE + UART_LSR);
    bool uart_is_ready = LSR_reg & UART_LSR_TEMT;

    // Push a symbol to transmit aux register
    if (uart_is_ready) 
        aux_reg_write(*src++, IOTDK_DBG_UART_BASE + UART_THR);
  }
}

// Simple dump of symbols to a pre-allocated memory region.
// When total log exceeds memory region size, cursor is moved to its begining.
// The memory region can be viewed afterward with debugger.
// It can be viewed/read with debugger afterward.
void LogToMem(const char* s) {
  static int cursor = 0;
#pragma Bss(".debug_log")
  volatile static char debug_log_mem[IOTDK_LOG_TO_MEMORY_SIZE];
#pragma Bss()

  const char* src = s;
  while (*src) {
    debug_log_mem[cursor] = *src++;
    cursor = (cursor < IOTDK_LOG_TO_MEMORY_SIZE) ? cursor + 1 : 0;
  }
  debug_log_mem[cursor] = '^';
}

extern "C" void DebugLog(const char* s) {
#ifndef TF_LITE_STRIP_ERROR_STRINGS

#if defined IOTDK_LOG_TO_UART
  DbgUartSendStr(s);
#endif

#if defined IOTDK_LOG_TO_MEMORY
#warning \
    "IOTDK_LOG_TO_MEMORY is defined. View .debug_log memory region for stdout"
  LogToMem(s);
#endif

#if defined IOTDK_LOG_TO_HOST
#warning "IOTDK_LOG_TO_HOST is defined. Ensure hostlib is linked."
  fprintf(stderr, "%s", s);
#endif

#endif  // TF_LITE_STRIP_ERROR_STRINGS
}
