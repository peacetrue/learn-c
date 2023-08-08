# 运行程序
debug.vars+=ARGS #程序参数
debug.vars+=BACK #后台执行
run/%: $(BUILD)/%
	$< $(ARGS) $(BACK)

# 追踪系统调用
debug.vars+=STRACE_FLAGS #strace 选项
# STRACE_FLAGS=-ff
$(BUILD)/%.strace: $(BUILD)/%
	strace $(STRACE_FLAGS) -o $@ $< $(ARGS)
