#include "hal.h"


const int GREEN = 0b100, YELLOW = 0b10, RED = 0b1;
const int N = 0x1;
const int delay = 500, freq = 4;
volatile bool block = false;


inline GPIO_PinState get_state(int is_one) {
	return is_one? GPIO_PIN_SET: GPIO_PIN_RESET;
}


void set_color(int color) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, get_state(color >> 2 & 1));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, get_state(color >> 1 & 1));
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, get_state(color >> 0 & 1));
}


int get_sw_value() {
	unsigned int sw_num[] = {GPIO_PIN_12, GPIO_PIN_10, GPIO_PIN_8, GPIO_PIN_4};
	int num = 0, pow = 1;
	for (int i = 0; i < 4; i++) {
		num += pow * HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
		pow <<= 1;
	}
	return num;
}

void set_pins(int state) {
	unsigned int leds_num[] = {GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3};
	for (int i = 0; i < 4; i++) {
		HAL_GPIO_WritePin(GPIOD, leds_num[i], get_state(state & 1));
		state >>= 1;
	}
}

inline bool is_button_clicked() {
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_RESET;
}

void wait_for_btn() {
	do {
		for (int i = 0; i < freq; i++) {
			if (is_button_clicked()) {
				set_color((block ^= true)? RED: GREEN);
				HAL_Delay(delay);
				break;
			} HAL_Delay(delay / freq);
		}
	} while (block);
}


void show_animation() {
	unsigned int leds_num[] = {
		GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6,
		GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12
	};

	while (1) {
		for (int i = 0; i < 8; i++) {
			if (get_sw_value() != N) return;
			for (int j = 0; j < 8; j++) {
				if (j == i || j == i + 1 || j == 7 - i) {
					HAL_GPIO_WritePin(GPIOD, leds_num[j], GPIO_PIN_SET);
					continue;
				}
				HAL_GPIO_WritePin(GPIOD, leds_num[j], GPIO_PIN_RESET);
			}
			wait_for_btn();
		}
	}
}

int umain() {
	while (1) {
		int state = get_sw_value();
		if (state == N) {
			show_animation();
		} else {
			set_color(YELLOW);
			set_pins(state);
		}
	}

	return 0;
}
