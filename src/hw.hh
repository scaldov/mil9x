#ifndef __hw_h__
#define __hw_h__

#ifdef __cplusplus
extern "C" {
#endif
#include <MDR32F9Qx_config.h>
#include <MDR32F9Qx_board.h>
#include <MDR32Fx.h>
#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_ssp.h>
#include <MDR32F9Qx_timer.h>
#include <MDR32F9Qx_rst_clk.h>
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_eeprom.h"

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#ifdef __cplusplus
}
#endif

#include <core_cm3.h>

#include <sys/types.h>
#include <limits.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
extern int __data_start__;
extern int __bss_end__;
extern int __HeapLimit;
extern int __StackLimit;
extern size_t xFreeBytesRemaining;
#ifdef __cplusplus
}
#endif

typedef uint32_t BaseAddr;

#include "bits.hh"
#include "rst.hh"
#include "dma.hh"
#include "adc.hh"
#include "timer.hh"
#include "ssp.hh"
#include "port.hh"

#endif
