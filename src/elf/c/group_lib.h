#ifndef LIB_GROUP
#define LIB_GROUP

#define LEVEL_INFO "info"
#define LEVEL_DEBUG "debug"

/** 默认的日志级别 */
extern const char *default_level;

/**
 * 记录日志，使用默认的日志级别。
 * @param message 日志消息
 */
void log(const char *message);

#endif
