/*
 * hyetal.h
 *
 *  Created on: 2017年9月21日
 *      Author: Administrator
 */

#ifndef HYETAL_H_
#define HYETAL_H_

#include "stm32f1xx.h"

#define RAIN_GAUGE_RATIO   0.5f
#define TOTLAL_HOURS       24
#define MINI_ELEMENT_COUNT 12
#define TOTAL_RAINFALL_TIME_SCOPE 0xFFFF

typedef struct {
	uint8_t isRaining;
	uint16_t rainFallCount;
} rainGauge_t;

typedef struct {
	uint16_t rainFall[MINI_ELEMENT_COUNT * TOTLAL_HOURS]; //用于保存最近24h的雨量信息
	uint16_t waterLavel[MINI_ELEMENT_COUNT]; //用于保存最近1h的水位
	uint32_t totalRainFall; //保存累计雨量
	uint8_t rainGaugePscCount;
} waterInf_t;

extern waterInf_t waterInf;

void trigger_rain_signal(waterInf_t *waterInf);
uint32_t get_rainFall(waterInf_t *waterInf, uint16_t timeScope);
void move_waterInf_timeWindow(waterInf_t *waterInf);
void clear_waterInf(waterInf_t *waterInf);

#endif /* HYETAL_H_ */
