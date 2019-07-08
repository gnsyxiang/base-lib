/**
 *
 * Release under GPLv2.
 * 
 * @file    symtab.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    22/07 2019 15:47
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        22/07 2019      create the file
 * 
 *     last modified: 22/07 2019 15:47
 */
#ifndef __BASE_LIB_UTILS_INC_SYMTAB_H_
#define __BASE_LIB_UTILS_INC_SYMTAB_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASE_LIB_UTILS_SRC_SYMTAB_GB
#define BASE_LIB_UTILS_SRC_SYMTAB_EX extern
#else
#define BASE_LIB_UTILS_SRC_SYMTAB_EX
#endif

int symtab_init(void **handle, int num);
void symtab_final(void *handle);

void symtab_insert(void *handle, char *key, void *value);
void *symtab_remove_peek(void *handle, char *key);
void *symtab_remove(void *handle, char *key);

typedef void (*symtab_user_cb_t)(char *key, void *value, void *args);
void symtab_map_user_display(void *handle, symtab_user_cb_t symtab_user_cb, void *args);

#ifdef __cplusplus
}
#endif

#endif /* __BASE_LIB_UTILS_INC_SYMTAB_H_ */

