.SECONDARY:#保留中间过程文件
CFLAGS:=-g

include os.mk
include build.common.mk
include build.elf.mk
include run.mk
include lldb.mk
include process.mk
include heap.mk
include thread.mk
include make.debug.mk

#分析预处理过程
preprocess.case: $(BUILD)/preprocess.i;
cases += preprocess.case
#分析类型转换实现原理。内存中小端序，寄存器中低位对应内存中低位，eax 截取 rax 的低位部分，最终得到低位部分值
type_cast.case: lldb.batch.type_cast.bin $(BUILD)/type_cast.s;
cases += type_cast.case
# 执行所有案例
all: $(cases);
