memory
======


## 函数原型

```txt
#include <stdlib.h>

void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
```

## malloc

### 返回类型转换

malloc()函数可用于返回指向数组的指针、指向结构体的指针等，
所以通常该函数的返回值会被强制转换为匹配的类型，但是从C99版本开始，
`void *`类型的指针不需要强制转换地赋给所有的指针变量。

### 长度计算

对于字符串的长度计算，需要用`strlen(buf) + 1`

> note: 此时不能用`sizeof`，因为sizeof会返回数组和指针的长度，而不是字符串的长度







