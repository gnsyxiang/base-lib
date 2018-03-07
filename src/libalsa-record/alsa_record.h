/**
 * 
 * Release under GPLv2.
 * 
 * @file    alsa_record_interface.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    06/03 2018 20:43
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        06/03 2018      create the file
 * 
 *     last modified: 06/03 2018 20:43
 */
#ifndef __BASE_LIB_ALSA_RECORD_H_
#define __BASE_LIB_ALSA_RECORD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <alsa/asoundlib.h>

#ifndef ALSA_RECORD_INTERFACE_GB
#define ALSA_RECORD_INTERFACE_EX extern
#else
#define ALSA_RECORD_INTERFACE_EX
#endif

//#define DEV_NAME		"default"//"plughw:2,0"
//#define DEV_NAME		"plughw:2,0"
#define DEV_NAME		"hw:1,0"

#define CHANNELS		(2)
#define SAMPLE_RATE		(44100)
#define BIT_PER_SAMPLE	(16)

#define FRAMES_CNT		(1024)
#define FRAMES_SIZE		(BIT_PER_SAMPLE / 8 * CHANNELS)

ALSA_RECORD_INTERFACE_EX snd_pcm_t *alsa_record_get_handle(void);
ALSA_RECORD_INTERFACE_EX void alsa_record_put_handle(snd_pcm_t *capture_handle);
ALSA_RECORD_INTERFACE_EX int alsa_record_read_pcm(snd_pcm_t *capture_handle, void *buf, int len);

#ifdef __cplusplus
}
#endif

#endif /* end __BASE_LIB_ALSA_RECORD_H_ */

