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

#ifndef MIC_READ_GB
#define MIC_READ_EX		extern
#else
#define MIC_READ_EX
#endif

#define DEV_NAME			"hw:1,0"

#define CHANNELS			(8)
#define SAMPLE_RATE			(16000)
#define BIT_PER_SAMPLE_STR	"S16_LE"

#define PERIOD_TIME			(64 * 1000)
#define FRAME_CNT			(1024)

#define MIC_READ_QUEUE_CNT	(2)
#define MIC_READ_QUEUE_LEN	(FRAME_CNT * CHANNELS * sizeof(int) * MIC_READ_QUEUE_CNT)

typedef struct mic_data {
	short amic[CHANNELS];
}mic_data_t;

MIC_READ_EX void mic_read_init(void);
MIC_READ_EX void mic_read_clean(void);

MIC_READ_EX void mic_read_get_data(mic_data_t *buf, int len);

#ifdef __cplusplus
}
#endif

#endif

