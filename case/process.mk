# 进程分析
# 获取进程 id。ps -f 选项展示 CMD 全部内容，默认会截断 CMD 名称
proc.pid=ps -a | grep "$(1)" | grep -v "grep" | head -n 1 | awk '{print $$1}' > $(2)
proc.delay=0#程序执行后，等待一定时间，直到操作系统完全启动了该程序，此时再观察其虚拟内存
$(BUILD)/%.ps: $(BUILD)
	sleep $(proc.delay)
	ps -T | grep "$*" | grep -v "grep" > $@
$(BUILD)/%.pid: $(BUILD)
	sleep $(proc.delay)
	$(call proc.pid,$*,$@)
# top 分析内存  -e M
$(BUILD)/%.top: $(BUILD)/%.pid
	top -b -e m -n 1 -p `cat $<` > $@
#$(BUILD)/%.top.humanize: $(BUILD)/%.top
#	cat $< | awk '/java/ {$$5 = $$5 + 10} { print }' > $@
#	cat $< | awk '/java/ {cmd="numfmt --from-unit=K --to=iec-i "; for (i = 5; i <= 7; i++) {cmd_i=cmd $$i; cmd_i | getline $$i; close(cmd_i); } } { if(/PID|java/) print }' | column -t > $@
$(BUILD)/%.pmap: $(BUILD)/%.pid
	$(if $(is_linux),pmap,vmmap) `cat $<` > $@
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
