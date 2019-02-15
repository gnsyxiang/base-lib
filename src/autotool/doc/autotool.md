autotool.md
===========




## configure.ac

### AC_DEFINE: 定义C语言中的宏

* 原型

  ```
  AC_DEFINE(VARIABLE, VALUE, DESCRIPTION)

  在configure.ac中定义，输出到config.h中: 
  
  /* DESCRIPTION */
  #define VARIABLE VALUE
  ```

* The macro AC_DEFINE can be used anywhere in the script, to define a C preprocessor macro.
* It accepts an optional argument indicating the value of the macro.
* If you want to do shell expansion for the value, use AC_DEFINE_UNQUOTED instead.

Some Examples:

  ```
  AC_DEFINE(HELLO) 

  AC_DEFINE(VERSION, "5.6") 
  
  myvar="foo" othervar="bar"
  AC_DEFINE_UNQUOTED(TEST, "${myvar}${othervar}")
  ```

### AM_SILENT_RULES: 屏蔽一些输出信息

生成 Makefile 的过程通常产生非常长的信息，我们一般不需要看这么多， 而且大量无用的信息刷屏可能会让你忽视夹在其中的警告和错误信息。

AM_SILENT_RULES([yes]): 将屏蔽这些消息，只输出一些总结信息。

> note: 一个真正的程序员可能会使用 AM_SILENT_RULES([no])，然后运行 ./configure --enable-silent-rules 来保证正确性。
> 如果你开启了静默模式，但还是希望能检查所有输出信息，可以使用 make V=1 命令（V 表示 Verbose）。

