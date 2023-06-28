################# 运行相关 #################
BACK=&#后台执行
#EXEC_EXTENSION=#执行程序扩展名
run/%: $(BUILD)/%$(EXEC_EXTENSION)
	$< $(ARGS) > $(BUILD)/$*.log $(BACK)
$(BUILD)/%.trace: $(BUILD)/%$(EXEC_EXTENSION)
	strace -o $@ $< $(ARGS)
$(BUILD)/%.lldbinit: $(BUILD)/%$(EXEC_EXTENSION)
	echo "target create $<" > $@
	echo "breakpoint set --name main" >> $@
	echo "r $(ARGS)" >> $@
	echo "platform shell sleep 5" >> $@
	echo "c" >> $@
	echo "q" >> $@
# 在 .lldbinit 中配置初始化命令
lldb/%: $(BUILD)/%.lldbinit
	lldb -s $< #> /dev/null 2>&1
#	lldb -s $< > /dev/null 2>&1 & # lldb 无法在后台运行，会直接退出
# ps -f 选项展示 CMD 全部内容，默认会截断 CMD 名称
$(BUILD)/%.pid: $(if $(filter &,$(BACK)),run/%,)
	ps -a | grep "$*" | grep -v "grep" | head -n 1 | awk '{print $$1}' > $@
#	ps -af | grep -E "elf.*$*" | head -n 1 | awk '{printf $$2}'
# top 分析内存  -e M
$(BUILD)/%.top: $(BUILD)/%.pid
	top -b -e m -n 1 -p `cat $<` > $@
#$(BUILD)/%.top.humanize: $(BUILD)/%.top
#	cat $< | awk '/java/ {$$5 = $$5 + 10} { print }' > $@
#	cat $< | awk '/java/ {cmd="numfmt --from-unit=K --to=iec-i "; for (i = 5; i <= 7; i++) {cmd_i=cmd $$i; cmd_i | getline $$i; close(cmd_i); } } { if(/PID|java/) print }' | column -t > $@
$(BUILD)/%.pmap: $(BUILD)/%.pid
	pmap `cat $<` > $@
#	vmmap `cat $<` > $@
$(BUILD)/%.maps: $(BUILD)/%.pid
	cat /proc/`cat $<`/maps > $@
# 添加序号表头
$(BUILD)/%.maps.head: $(BUILD)/%.maps
	sed "1i $(shell seq -s ' ' 1 `cat $< | grep heap | head -n 1 | awk '{print NF}'`)" $< | column -t > $@
# 列出 so 文件。例如：/usr/lib/x86_64-linux-gnu/libc.so.6             2.2M
$(BUILD)/%.so.ls: $(BUILD)/%.maps
	cat $< | grep -E '/usr|/root' | awk '{printf "%s\n", $$NF}' | uniq | xargs -I {} ls -lah "{}" | awk '{printf "%s %s\n",$$NF,$$5}' | column -t > $@
# 列出 so 文件。例如：libc.so.6	2.2M
$(BUILD)/%.size: $(BUILD)/%.so.ls
	cat $< | awk -F / '{printf "%s\n", $$NF}' | column -t > $@
stop/%: $(BUILD)/%.pid
	kill -9 `cat $<` | true
maps/%: $(BUILD)/%.maps.head $(BUILD)/%.pmap $(BUILD)/%.top;
#maps/%: $(BUILD)/%.maps stop/%; #lldb 运行中无法 stop

#运行+生成+结束：r=run, m=maps, s=stop。
#运行的程序必须能持续一段时间，等到捕捉到 maps；否则可以使用 lldb 调试，使程序阻塞，再手动调用 maps
#$(BUILD)/%.rms: $(BUILD)/%.readobjbin $(BUILD)/%.run $(BUILD)/%$(EXEC_EXTENSION).maps $(BUILD)/%.stop;
# make sleep.rms SUBDIR=/sleep-default-static STATIC=TRUE BACK='&'
