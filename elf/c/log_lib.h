#ifndef LIB_GROUP
#define LIB_GROUP

#define LEVEL_INFO "info"
#define LEVEL_DEBUG "debug"

/** 默认的日志级别 */
extern const char *default_level;

/**
 * 记录日志，使用默认的日志级别。命名为 log_mini 避免 builtin 重名提示。
 * @param message 日志消息
 */
void log_mini(const char *message);

#endif
