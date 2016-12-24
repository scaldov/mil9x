#ifndef __LK_LED_HH__
#define __LK_LED_HH__

#include "hw.hh"

#include "pin.hh"

#include <stdio.h>
#include <vector>

namespace lk {

class led;

extern const uint32_t led_ptrn_heartbeat;
extern const uint32_t led_ptrn_blink;
extern const uint32_t led_ptrn_alarm;

class led : public pin{
	int index, cnt, state;
    static std::vector<led*> led_list;
public:
	enum PATTERN {
		PTRN_HEARTBEAT = 0x23050505,
		PTRN_BLINK = 0x1919,
		PTRN_ALARM = 0x0703
	};
	uint32_t pattern;
	int scale;
	led(gpio *port, int pin_num)
	:pin::pin(port, pin_num, 1)
	{
		reset();
		this->pattern = PTRN_HEARTBEAT;
		scale = 1;
        printf_("led\n");
		led_list.push_back(this);
//		led(port, pin_num, PTRN_HEARTBEAT, 0);
	}
	led(gpio *port, int pin_num, uint32_t pattern, int state)
	:pin::pin(port, pin_num, 1)
	{
		led_list.push_back(this);
		reset();
		this->pattern = pattern;
		scale = 1;
	}
	~led() {
        for(auto it = led_list.begin(); it != led_list.end(); ++it) {
            if(*it == this){
                led_list.erase(it);
                break;
            }
        }
	}
	void reset() {
		index = 0;
		state = 0;
		cnt = 0;
//		pin::reset();
	}
	void clk_this() {
		if(cnt != 0) {
			cnt --;
			return;
		} else {
			int v = (pattern >> index) & 0xFF;
			if(v == 0 || index == 32) {
				index = 0;
				v = (pattern >> index) & 0xFF;
			}
			cnt = v * scale;
			if(cnt != 0){
				if(index & 0b1000) {
                    pin::reset();
				} else {
					pin::set();
				}
			}
			index += 8;
		}
	}
	static void clk() {
		for(auto it : led_list) {
			it->clk_this();
		}
	}
};
}

#endif
