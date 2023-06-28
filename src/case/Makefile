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
CFLAGS:=-g
$(BUILD)/%.i: c/%.c $(BUILD)
	gcc $(CFLAGS) -E -o $@ $<
$(BUILD)/%.i: cpp/%.cpp $(BUILD)
	gcc $(CFLAGS) -E -o $@ $<
$(BUILD)/%.s: c/%.c $(BUILD)
	gcc $(CFLAGS) -S -o $@ $<
$(BUILD)/%.s: cpp/%.cpp $(BUILD)
	gcc $(CFLAGS) -S -o $@ $<
$(BUILD)/%.bin: c/%.c $(BUILD)
	gcc $(CFLAGS) -o $@ $<
$(BUILD)/%.bin: cpp/%.cpp $(BUILD)
	gcc $(CFLAGS) -o $@ $<

EXEC_EXTENSION=.bin
include ../elf/run.mk
$(BUILD)/%.trace.mini: $(BUILD)/%.trace
	cat $< | sed -n '/write(1, "way:/,/write(1, "buffer location:/p' > $@
#	cat $< | grep -A 3 'write(1, "size: ' > $@

# 分析应用程序堆内存
heap_subdir:=/heap
heap_time:=2
heap_size:=10
heap_seconds:=1
heap.case: $(BUILD)/heap.trace.mini $(BUILD)/heap.top $(BUILD)/heap.pmap $(BUILD)/heap.maps.head $(BUILD)/heap.so.ls stop/heap;
heap.cases:
	@for way in $(shell seq 0 3); do \
	    make clean heap.case SUBDIR=$(heap_subdir)-$$way-$(heap_time)-$(heap_size)-$(heap_seconds) ARGS="$$way $(heap_time) $(heap_size) $(heap_seconds)";\
    done
#	make clean heap.case SUBDIR=$(heap_subdir_prefix)-0 ARGS='0 0 0 $(heap_seconds)'
#	make clean heap.case SUBDIR=$(heap_subdir_prefix)-brk-2-10 ARGS='1 2 10 $(heap_seconds)'
#	make clean heap.case SUBDIR=$(heap_subdir_prefix)-mmap-2-10 ARGS='2 2 10 $(heap_seconds)'
#	make clean heap.case SUBDIR=$(heap_subdir_prefix)-malloc-2-10 ARGS='3 2 10 $(heap_seconds)'

# 执行 graffle 堆内存绘图
graffle_dir:=$(workingDir)/learn-graffle/automation
heap.graffle.cases:
	@for way in $(shell seq 0 3); do \
	    make -C $(graffle_dir) demo-memory.call.case script_argument=$(shell pwd)/$(BUILD)$(heap_subdir)-$$way-$(heap_time)-$(heap_size)-$(heap_seconds)/heap.maps.txt;\
    done
#	make -C $(graffle_dir) demo-memory.call.case script_argument=$(shell pwd)/$(BUILD)/heap-0/heap.maps.txt
#	make -C $(graffle_dir) demo-memory.call.case script_argument=$(shell pwd)/$(BUILD)/heap-brk-2-10/heap.maps.txt
#	make -C $(graffle_dir) demo-memory.call.case script_argument=$(shell pwd)/$(BUILD)/heap-mmap-2-10/heap.maps.txt
#	make -C $(graffle_dir) demo-memory.call.case script_argument=$(shell pwd)/$(BUILD)/heap-malloc-2-10/heap.maps.txt

#分析预处理过程。
preprocess.case: $(BUILD)/preprocess.i;

#分析类型转换实现原理。内存中小端序，寄存器中低位对应内存中低位，eax 截取 rax 的低位部分，最终得到低位部分值
type_cast.case: $(BUILD)/type_cast.source $(BUILD)/type_cast.bin $(BUILD)/type_cast.s
	lldb -b -s $< -- $(word 2, $^)
$(BUILD)/type_cast.source:
	echo "breakpoint set --all-files -p 'printf\\(\"long'" > $@
	echo "run" >> $@
	echo "x/8b &var_long" >> $@
	echo "x/2w &var_long" >> $@
	echo "x/1g &var_long" >> $@
	echo "reg read rax" >> $@
	echo "reg read eax" >> $@
	echo "c" >> $@
