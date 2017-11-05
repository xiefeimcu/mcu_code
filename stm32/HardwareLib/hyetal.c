/*
 * hyetal.c
 *
 *  Created on: 2017年9月21日
 *      Author: Administrator
 */
#include "hyetal.h"
#include "config.h"
#include "rtc.h"
#include "string.h"

waterInf_t waterInf;

#define RAINFALL_SAVE_FLAG 0xA5A5

/*
 * 保存雨量累计值到后备区域
 */

void save_rainFall_backReg(uint32_t count){
	typedef union {
		uint32_t intNum;
		uint16_t numHbit;
		uint16_t numLbit;
	} num_t;
	num_t num;

	num.numHbit = HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR40);
	num.numLbit = HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR41);

	num.intNum += count;

	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR40,num.numHbit);
	HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR41,num.numLbit);
}

/*
 * 每5分钟调用一次
 * */
void calculate_5minute_rainFall(waterInf_t *waterInf) {
	uint16_t i;
	/*
	 * 雨量时间窗口移动
	 */
	for (i = RAINFALL_DATA_NUM_OF_HOUR * TOTLAL_HOURS - 1; i > 0; i--) {
		waterInf->rainFall[i] = waterInf->rainFall[i - 1];
	}
	waterInf->rainFall[0] = 0;

	/*
	 * 水位时间窗口移动
	 */
	for (i = RAINFALL_DATA_NUM_OF_HOUR - 1; i > 0; i--) {
		waterInf->waterLavel[i] = waterInf->waterLavel[i - 1];
	}
	waterInf->waterLavel[0] = 0;
}

float convt_rainFall_to_float(uint32_t count){
	return (RAIN_GAUGE_RATIO * (float) (count) * (float)rtuParameter.rainGaugeParamater.rainGaugePsc);
}
/*
 * 参数 conut: 需要计算五分钟雨量的个数
 */
float get_rainFall(waterInf_t *waterInf, uint16_t count) {
	uint16_t i;
	uint32_t sum = 0;

	if(count >= RAINFALL_DATA_NUM_OF_HOUR * TOTLAL_HOURS ){
		return -1;
	}

	for (i = 0; i < count; i++) {
		sum += waterInf->rainFall[i];
	}

	return convt_rainFall_to_float(sum);
}

/*
 * 根据雨量计的分辨率计算实际降水量
 */
float get_5minute_rainFall(waterInf_t *waterInf){
	return get_rainFall(waterInf,1);
}

float get_hour_rainFall(waterInf_t *waterInf){
	return get_rainFall(waterInf,RAINFALL_DATA_NUM_OF_HOUR);
}

float get_day_rainFall(waterInf_t *waterInf){
	return get_rainFall(waterInf,RAINFALL_DATA_NUM_OF_HOUR * TOTLAL_HOURS);
}

void clear_waterInf(waterInf_t *waterInf) {
	waterInf->rainGaugePscCount = 0;
	waterInf->totalRainFall = 0;

	memset(waterInf->waterLavel, 0, RAINFALL_DATA_NUM_OF_HOUR * TOTLAL_HOURS);
	memset(waterInf->waterLavel, 0, RAINFALL_DATA_NUM_OF_HOUR);
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

