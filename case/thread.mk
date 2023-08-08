# 线程实现原理分析，对应源文件 thread.c

# 分析创建线程使用的系统调用
thread.strace.case: ARGS+=2
thread.strace.case: $(BUILD)/thread.bin.strace;
# 分析线程 id
thread.ps.case: ARGS+=2
thread.ps.case: proc.delay=1
thread.ps.case: lldb.batch.thread.bin $(BUILD)/thread.bin.ps;
# 分析线程的栈帧
thread.frame.case:
	make lldb.batch.thread.bin lldb.source=.frame
# 清除构建内容
thread.clean.case: clean/*thread*;
# 合并多个案例
thread.cases: thread.strace.case thread.ps.case thread.frame.case;
cases += thread.cases
