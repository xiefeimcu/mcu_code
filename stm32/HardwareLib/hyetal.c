/*
 * hyetal.c
 *
 *  Created on: 2017年9月21日
 *      Author: Administrator
 */
#include "hyetal.h"

rainGauge_t rainGauge;

uint32_t getRainFall(rainGauge_t *rainGauge){

	return ((rainGauge->rainFallCount * RAIN_GAUGE_RATIO) *10);
}

void clearRainFall(rainGauge_t *rainGauge){
	rainGauge->rainFallCount = 0;
	rainGauge->isRaining = 0;
}

/*
 * 雨量计翻转一次调用一次
 */
void call_from_rain_signal(rainGauge_t *rainGauge){
	rainGauge->isRaining = 1;
	rainGauge->rainFallCount++;
}


