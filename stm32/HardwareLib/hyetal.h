/*
 * hyetal.h
 *
 *  Created on: 2017Äê9ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef HYETAL_H_
#define HYETAL_H_

#include "stm32f1xx.h"

#define RAIN_GAUGE_RATIO 0.5f
#define MINI_ELEMENT_COUNT 12

typedef struct {
	uint8_t  isRaining;
	uint16_t rainFallCount;
}rainGauge_t;

typedef struct{
	uint8_t  elementOfMini[MINI_ELEMENT_COUNT];
}hourElement_t;

typedef struct{
	hourElement_t rainFall;
	hourElement_t waterLavel;
}waterInfHourElement_t;

extern rainGauge_t rainGauge;

uint32_t getRainFall(rainGauge_t *rainGauge);
void call_from_rain_signal(rainGauge_t *rainGauge);
void clearRainFall(rainGauge_t *rainGauge);

#endif /* HYETAL_H_ */
