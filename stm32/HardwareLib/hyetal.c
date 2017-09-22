/*
 * hyetal.c
 *
 *  Created on: 2017年9月21日
 *      Author: Administrator
 */
#include "hyetal.h"
#include "config.h"

waterInf_t waterInf;

void move_waterInf_timeWindow(waterInf_t *waterInf) {
	uint16_t i;
	/*
	 * 雨量时间窗口移动
	 */
	for (i = MINI_ELEMENT_COUNT * TOTLAL_HOURS - 1; i > 0; i--) {
		waterInf->rainFall[i] = waterInf->rainFall[i - 1];
	}
	waterInf->rainFall[0] = 0;

	/*
	 * 水位时间窗口移动
	 */
	for (i = MINI_ELEMENT_COUNT - 1; i > 0; i--) {
		waterInf->waterLavel[i] = waterInf->waterLavel[i - 1];
	}
	waterInf->waterLavel[0] = 0;
}

uint32_t get_rainFall(waterInf_t *waterInf, uint16_t timeScope) {
	uint16_t i;
	uint32_t sum = 0;
	if (timeScope <= MINI_ELEMENT_COUNT) {
		for (i = 0; i < timeScope; i++) {
			sum += waterInf->rainFall[i];
		}

	} else if (timeScope == TOTAL_RAINFALL_TIME_SCOPE) {
		sum = waterInf->totalRainFall;
	}

	else {
		sum = 0;
	}

	return sum;
}

void clear_waterInf(waterInf_t *waterInf) {
	waterInf->rainGaugePscCount = 0;
	waterInf->totalRainFall = 0;

	memset(waterInf->waterLavel, 0, MINI_ELEMENT_COUNT * TOTLAL_HOURS);
	memset(waterInf->waterLavel, 0, MINI_ELEMENT_COUNT);
}

/*
 * 雨量计翻转一次调用一次
 */
void trigger_rain_signal(waterInf_t *waterInf) {
	waterInf->rainGaugePscCount++;

	if (waterInf->rainGaugePscCount
			>= rtuParameter.rainGaugeParamater.rainGaugePsc) {
		waterInf->rainFall[0]++;
		waterInf->rainGaugePscCount = 0;
	}
}

