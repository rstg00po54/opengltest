//调试打印开关
#define __DEBUG
 
#ifdef __DEBUG
//只取文件名
#define MYFILE(x) strrchr(x,'/')?strrchr(x,'/')+1:x
// windows:
#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x
// linux:
// #define filename(x) strrchr(x,'/')?strrchr(x,'/')+1:x
#define pr_debug(format, ...) printf("[%10s:%d][%s]: \033[32m" format "\033[32;0m\n", MYFILE(__FILE__), __LINE__, __func__, ##__VA_ARGS__)
#define pr_info(format, ...) printf("[" __FILE__ "][Line: %d][%s]: \033[33m" format "\033[32;0m\n", __LINE__, __func__, ##__VA_ARGS__)
#define pr_err(format, ...) printf("[" __FILE__ "][Line: %d][%s]: \033[31m" format "\033[32;0m\n", __LINE__, __func__, ##__VA_ARGS__)
#else
#define pr_debug(format, ...)
#define warn_info(format, ...)
#define error_info(format, ...)
#endif
