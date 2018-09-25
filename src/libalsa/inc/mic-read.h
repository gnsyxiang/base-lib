/**
 *
 * Release under GPLv2.
 * 
 * @file    mic-read.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    02/05 2018 17:47
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        02/05 2018      create the file
 * 
 *     last modified: 02/05 2018 17:47
 */
#ifndef __MIC_READ_H_
#define __MIC_READ_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <alsa/asoundlib.h>

#ifndef MIC_READ_GB
#define MIC_READ_EX		extern
#else
#define MIC_READ_EX
#endif

#define BIT_PER_SAMPLE_DEF	1

#define DEV_NAME			"hw:1,0"

#define CHANNELS			(8)
#define SAMPLE_RATE			(16000)

#define PERIOD_TIME			(64 * 1000)
#define FRAME_CNT			(1024)

#define MIC_READ_QUEUE_CNT	(2)
#define MIC_READ_QUEUE_LEN	(FRAME_CNT * CHANNELS * sizeof(int) * MIC_READ_QUEUE_CNT)

#if (BIT_PER_SAMPLE_DEF == 0)
	#define BIT_PER_SAMPLE_STR	"S16_LE"
	typedef struct mic_data {
		short amic[CHANNELS];
	}mic_data_t;
#elif (BIT_PER_SAMPLE_DEF == 1)
	#define BIT_PER_SAMPLE_STR	"S24_LE"
	typedef struct mic_data {
		int amic[CHANNELS];
	}mic_data_t;
#endif

MIC_READ_EX void mic_read_init(void);
MIC_READ_EX void mic_read_clean(void);

MIC_READ_EX void mic_read_pause(void);
MIC_READ_EX void mic_read_resume(void);

MIC_READ_EX void mic_read_get_data(void *buf, int len);

#ifdef __cplusplus
}
#endif

#endif

