include ../elf/common.mk

MACROS:=
MACROS+= -DVOLATILE=volatile
MACROS+= -DFENCE_COMPILER
#MACROS+= -DFENCE_TYPE=compiler
#CFLAGS+= -masm=intel

all: init elf
%: $(CPP)/%.cpp init
	gcc $(CFLAGS) -S -O0 $< -o $(BUILD)/$@-O0.s
	gcc $(CFLAGS) -S -O1 $< -o $(BUILD)/$@-O1.s
	gcc $(CFLAGS) -S -O1 $(MACROS) $< -o $(BUILD)/$@-O1-volatile.s
	gcc $(CFLAGS) -O0 $< -o $(BUILD)/$@-O0.bin
	gcc $(CFLAGS) -O1 $< -o $(BUILD)/$@-O1.bin
	gcc $(CFLAGS) -O1 $(MACROS) $< -o $(BUILD)/$@-O1-volatile.bin
	gcc $(CFLAGS) -O0 -c $< -o $(BUILD)/$@-O0.o

#临时关闭地址随机化
close_rvs:
	sysctl -w kernel.randomize_va_space=0
	sysctl -a | grep kernel.randomize_va_space
$(BUILD)/%.bin: c/%.c $(BUILD)
	gcc $(CFLAGS) -o $@ $<
$(BUILD)/%.bin: cpp/%.cpp $(BUILD)
	gcc $(CFLAGS) -o $@ $<
exec/%: $(BUILD)/%.bin
	$< $(ARGS) > $(BUILD)/$*.log &
$(BUILD)/%.trace: $(BUILD)/%.bin
	strace -o $@ $< $(ARGS)
$(BUILD)/%.trace.mini: $(BUILD)/%.trace
	cat $< | sed -n '/write(1, "way:/,/write(1, "buffer location:/p' > $@
#	cat $< | grep -A 3 'write(1, "size: ' > $@
$(BUILD)/%.pid: exec/%
	ps -a | grep "$*" | grep -v "grep" | head -n 1 | awk '{print $$1}' > $@
# top 分析内存  -e M
$(BUILD)/%.top: $(BUILD)/%.pid
	top -b -e m -n 1 -p `cat $<` > $@
$(BUILD)/%.pmap: $(BUILD)/%.pid
	pmap `cat $<` > $@
# 此处理想的后缀名为 maps，为了兼容 graffle 自动化时选择文件改成了 maps.txt
$(BUILD)/%.maps.txt: $(BUILD)/%.pid
	cat /proc/`cat $<`/maps > $@
# 分析应用程序堆内存
heap.case: $(BUILD)/heap.trace.mini $(BUILD)/heap.top $(BUILD)/heap.pmap $(BUILD)/heap.maps.txt;
heap_subdir_prefix:=/heap
heap_seconds:=1
heap_sleep_seconds:=$(shell expr $(heap_seconds) \* 2)
heap.cases:
	make clean heap.case SUBDIR=$(heap_subdir_prefix)-0 ARGS='0 0 0 $(heap_seconds)'
	sleep $(heap_sleep_seconds)
	make clean heap.case SUBDIR=$(heap_subdir_prefix)-brk-2-10 ARGS='1 2 10 $(heap_seconds)'
	sleep $(heap_sleep_seconds)
	make clean heap.case SUBDIR=$(heap_subdir_prefix)-mmap-2-10 ARGS='2 2 10 $(heap_seconds)'
	sleep $(heap_sleep_seconds)
	make clean heap.case SUBDIR=$(heap_subdir_prefix)-malloc-2-10 ARGS='3 2 10 $(heap_seconds)'


# ./build/reorder-O0.bin
# ./build/reorder-O1-volatile.bin
# ./build/reorder-O1.bin
