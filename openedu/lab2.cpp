#include "hal.h"


const int INITIAL = 500;
const int T = 150;
const int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};

int frame = 0, mul = 0;


inline void on(int index) {
	HAL_GPIO_WritePin(GPIOD, leds_num[index], GPIO_PIN_SET);
}

inline void off(int index) {
	HAL_GPIO_WritePin(GPIOD, leds_num[index], GPIO_PIN_RESET);
}

void animate() {
	if (frame < 6) {
		on(6 - frame % 6);
		on(7 - frame % 6);
		if (frame != 0) off(8 - frame % 6);
	} else {
		on(frame % 6);
		on(frame % 6 + 1);
		if (frame != 6) off(frame % 6 - 1);
	}
	if (frame == 6) off(2);
	else if (frame == 0) off(5);

	frame = (frame + 1) % 12;
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

void handle_sw() {
	int sw = get_sw_value();
	if (mul != sw) {
		mul = sw;
		WRITE_REG(TIM6_ARR, 500 + mul * T);
	}
}

int umain() {
	registerTIM6_IRQHandler(animate);
	__enable_irq();

	WRITE_REG(TIM6_ARR, 500);
	WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
	WRITE_REG(TIM6_PSC, 0);

	WRITE_REG(TIM6_CR1, TIM_CR1_CEN);

	while (1) handle_sw();

	return 0;
}
