# 分析应用程序堆内存
$(BUILD)/heap.bin.strace.mini: $(BUILD)/%.strace
	cat $< | sed -n '/write(1, "way:/,/write(1, "buffer location:/p' > $@
#	cat $< | grep -A 3 'write(1, "size: ' > $@

heap_time:=2
heap_size:=10
heap_seconds:=5
# 追踪系统调用不需要睡眠
heap.strace.case: $(BUILD)/heap.bin.strace.mini;
# 需要睡眠且需要后台执行
run/heap.bin: BACK=&
heap.case: run/heap.bin $(BUILD)/heap.bin.top $(BUILD)/heap.bin.pmap $(BUILD)/heap.bin.maps.head $(BUILD)/heap.bin.so.ls stop/heap.bin;
heap.cases:
	@for way in $(shell seq 0 3); do \
	    make clean heap.strace.case BUILD=$(BUILD)/heap-$$way-$(heap_time)-$(heap_size)-$(heap_seconds) ARGS="$$way $(heap_time) $(heap_size) 0";\
	    make heap.case BUILD=$(BUILD)/heap-$$way-$(heap_time)-$(heap_size)-$(heap_seconds) ARGS="$$way $(heap_time) $(heap_size) $(heap_seconds)";\
    done
cases += heap.cases
