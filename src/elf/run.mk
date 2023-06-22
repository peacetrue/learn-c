################# 运行相关 #################
#运行可执行程序
BACK:=#'&'
$(BUILD)/%.run: $(BUILD)/%.bin$(STRIPPED)
	$< $(BACK)
#调试可执行程序。make build/empty.lldb
$(BUILD)/%.lldb: $(BUILD)/%.bin$(STRIPPED)
	lldb $<
#生成内存映射文件。ps -f 选项展示 CMD 全部内容，默认会截断 CMD 名称。make build/empty.bin.maps
#生成内存映射文件。ps -f 选项展示 CMD 全部内容，默认会截断 CMD 名称。make build/singleton-none-static/singleton.bin.stripped.maps  SUBDIR=/singleton-none-static
$(BUILD)/%.maps:
#	cat /proc/`ps -a | grep "$*" | head -n 1 | awk '{printf $$1}'`/maps > $@
	cat /proc/`ps -af | grep -E "elf.*$*" | head -n 1 | awk '{printf $$2}'`/maps > $@
#结束可执行程序
$(BUILD)/%.stop: $(BUILD)/%.bin$(STRIPPED)
	killall $<
#运行+生成+结束：r=run, m=maps, s=stop。
#运行的程序必须能持续一段时间，等到捕捉到 maps；否则可以使用 lldb 调试，使程序阻塞，再手动调用 maps
$(BUILD)/%.rms: $(BUILD)/%.readobjbin $(BUILD)/%.run $(BUILD)/%.bin$(STRIPPED).maps $(BUILD)/%.stop;
# make sleep.rms SUBDIR=/sleep-default-static STATIC=TRUE BACK='&'
