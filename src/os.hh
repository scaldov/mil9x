#ifndef __LK_OS_HH__
#define __LK_OS_HH__

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "semphr.h"

#include "hw.hh"

#include <stdlib.h>
#include <stdio.h>

extern int gdb_printf(char *s, ...);

//#define printf_ gdb_printf
#define printf_ printf

extern volatile uint32_t g_t[8];

#define SCHED_FREQ configTICK_RATE_HZ

constexpr uint32_t operator "" _bit(unsigned long long t) {
    return 1<<t;
}

constexpr uint32_t operator "" _ns(unsigned long long t) {
    return t;
}

constexpr uint32_t operator "" _us(unsigned long long t) {
	return t * 1000;
}

constexpr uint32_t operator "" _ms(unsigned long long t) {
	return t * 1000000;
}

constexpr uint32_t operator "" _MHz(unsigned long long t) {
	return t * 1000000;
}

constexpr uint32_t operator "" _kHz(unsigned long long t) {
	return t * 1000;
}

constexpr uint32_t operator "" _Hz(unsigned long long t) {
	return t;
}

namespace lk {

class os {
public:
	os() {
	}
	static void start() {
		vTaskStartScheduler();
	}
	static size_t get_free_mem() {
		size_t mem = 0, size, delta = 1 << 16;
		void *p;
		size = delta;
		do {
			p = malloc(size);
			//			printf("%d %d\n", p ? 1 : 0, size);
			if(p) {
				free(p);
				mem |= delta;
				delta = delta >> 1;
				size += delta;
			} else {
				delta = delta >> 1;
				size -= delta;
			}
		} while(delta);
		return mem;
	}
	static constexpr uint32_t usec(uint32_t usec){
		return (usec < 100000) ? SCHED_FREQ * usec / 1000000 :
				SCHED_FREQ * (usec / 1000) / 1000;
	}
	static void wait(int ticks) {
		vTaskDelay(ticks);
	}
};

class systick {
public:
	static uint32_t systick_ctrl;
	// guaranteed up to 100ms @ 128MHz on integers
	static constexpr uint32_t nsec(uint32_t ns, const uint32_t coreclk){
//		return (uint32_t)((double)coreclk * (double)ns / 1e9);
		return (ns < 100000) ? (coreclk / 1000000) * ns / 1000 :
				(coreclk / 1000000) * (ns / 1000);
	}
	static int wait_mark(uint32_t tick) {
		__disable_irq();
		uint32_t v, start, end;
		start = SysTick->VAL;
		if(start < (tick + 30)) {
			__enable_irq();
			return -1;
		}
		end = start - tick + 12;
		v = start;
		while(v > end){
			v = SysTick->VAL;
		}
		__enable_irq();
		return -0;
	}
	static void wait_until(uint32_t systick_val) {
		uint32_t v;
		v = SysTick->VAL;
		while(v > systick_val){
			v = SysTick->VAL;
		}
	}
	static void wait(uint32_t tick)
	__attribute__ ((noinline))
	__attribute__ ((__section__(".data")))
	{
		SysTick->LOAD = tick;
		SysTick->VAL = 0;
		SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
		while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	}

	static inline uint32_t get() {
		return SysTick->VAL;
	}
	static void begin() {
		systick_ctrl = SysTick->CTRL;
		SysTick->LOAD = 0xFFFFFF;
		SysTick->VAL = 0;
		SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	}
	static inline void end() {
		SysTick->CTRL = systick_ctrl;
	}
};


class thread {
public:
    TaskHandle_t id;
    void *param;
    thread (void (*function)(void*), const char *name, unsigned short stack, void *param, int priority){
		this->param = param;
		xTaskCreate( function,
				name ? name : "thr",
						stack ? (unsigned short)stack : configMINIMAL_STACK_SIZE,
								param, priority, &id);
	}
	void suspend() {
		vTaskSuspend(id);
	}
	void resume() {
		vTaskResume(id);
	}
	static void suspend_all() {
		vTaskSuspendAll();
	}
	static void resume_all() {
		xTaskResumeAll();
	}
};

}

#endif
