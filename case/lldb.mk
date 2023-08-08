# 本文件记录执行 LLDB 命令相关的内容

# 直接执行 LLDB，作为其他目标的依赖
.PHONY: lldb
lldb:
	lldb $(LFLAGS)
# 构建目标并执行 LLDB
lldb.%: $(BUILD)/%
	lldb $(LFLAGS) -- $< $(ARGS) $(BACK)
# LLDB 自动执行命令
lldb.source?=#同一个目标，多个使用场景，需要使用不同的 source
.PHONY: lldb.source/%
lldb.source.%: $(BUILD)/% $(BUILD)/%$(lldb.source).lldb.source
	lldb $(LFLAGS) -s $(word 2,$^) -- $< $(ARGS) $(BACK)
# 双井号注释不会输出到 lldb 控制台，单井号注释输出到控制台可以作为步骤提示
$(BUILD)/%$(lldb.source).lldb.source: lldb/%$(lldb.source).lldb.source $(BUILD)
	cat $< | grep -v "^##" > $@
# LLDB 批量执行命令，后台执行用于捕获线程信息
lldb.batch.%: LFLAGS+=-b
lldb.batch.%: BACK=&
lldb.batch.%: lldb.source.%;
# 睡眠 n 秒
$(BUILD)/%.sleep.lldb.source: $(BUILD)
	echo 'b main' > $@
	echo 'r' >> $@
	echo 'shell sleep $*' >> $@
	echo 'c' >> $@
