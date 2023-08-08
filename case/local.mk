# 本地使用
graffle_dir:=$(workingDir)/learn-graffle

# thread.mk
## 使用 graffle 绘制
thread.graffle.case:
	make -C $(workingDir)/learn-graffle omnijs-run-file script_content=class-caller.js.encode script_argument='thread.bin.maps.json.encode' external_src='$(shell pwd)/$(BUILD)'
	make -C $(workingDir)/learn-graffle omnijs-run-file script_content=class-caller.js.encode script_argument='thread.bin[1].frames.json.encode' external_src='$(shell pwd)/$(BUILD)'
	make -C $(workingDir)/learn-graffle omnijs-run-file script_content=class-caller.js.encode script_argument='thread.bin[2].frames.json.encode' external_src='$(shell pwd)/$(BUILD)'


# thread.mk
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


MACROS:=
MACROS+= -DVOLATILE=volatile
MACROS+= -DFENCE_COMPILER
#MACROS+= -DFENCE_TYPE=compiler
#CFLAGS+= -masm=intel

#all: init elf
#%: $(CPP)/%.cpp init
#	gcc $(CFLAGS) -S -O0 $< -o $(BUILD)/$@-O0.s
#	gcc $(CFLAGS) -S -O1 $< -o $(BUILD)/$@-O1.s
#	gcc $(CFLAGS) -S -O1 $(MACROS) $< -o $(BUILD)/$@-O1-volatile.s
#	gcc $(CFLAGS) -O0 $< -o $(BUILD)/$@-O0.bin
#	gcc $(CFLAGS) -O1 $< -o $(BUILD)/$@-O1.bin
#	gcc $(CFLAGS) -O1 $(MACROS) $< -o $(BUILD)/$@-O1-volatile.bin
#	gcc $(CFLAGS) -O0 -c $< -o $(BUILD)/$@-O0.o
