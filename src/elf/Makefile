SUBDIR:=#在子目录下构建时使用，默认为空格，设置时需要以 / 起始，例如：SUBDIR=/dynamic
SRC:=cpp$(SUBDIR)#设置源文件目录
BUILD:=build$(SUBDIR)#
BUILD:=$(shell echo "$(BUILD)" | tr -d ' ')#删除 BUILD 中的空格，避免执行 clean 时，误删根目录
#编译选项
CFLAGS:=
CFLAGS+=-fno-builtin# 禁用内建函数内联优化。
CFLAGS+=-static  #创建动态共享库时不能指定此参数 https://stackoverflow.com/questions/44429253/building-a-shared-library-created-a-static-library-instead
#CFLAGS+=-g #开启调试。追加选项，会添加空格，-fno-builtin -g
#CFLAGS+= -verbose #输出详细信息

READELF:= readelf
#TODO Linux 使用 readelf，其他环境替换为空命令
OS="`uname -s`"
#READELF:= $(if $(filter Linux,$(OS)),readelf,$(BUILD)/nop)

.SECONDARY: #保留中间过程文件

all: empty;
init:
# @ 避免在控制台输出当前命令执行语句。注释顶头写，如果缩进到命令的位置，会作为命令执行
	@mkdir -p $(BUILD)
	@echo '#!/bin/bash' > $(BUILD)/nop && chmod +x $(BUILD)/nop #创建无操作脚本
#查看变量值。单引号方便看出变量中是否包含空格
vars:
	@echo "OS: '$(OS)'"
	@echo "SUBDIR: '$(SUBDIR)'"
	@echo "SRC: '$(SRC)'"
	@echo "BUILD: '$(BUILD)'"
	@echo "CFLAGS: '$(CFLAGS)'"
#清除所有
clean:
	@rm -rf $(BUILD)
#清除指定文件
$(BUILD)/%.clean:
	@rm -rf $(BUILD)/$*
# make build/empty.bin.clean
#计算 16 进制
HEX1:=0x0
HEX2:=0x0
hex%:
	@printf "%x\n" $$(($(HEX1) $* $(HEX2)))
# make hex+ HEX1=0x000db2fc HEX2=0x0000000000000157
# make hex+ HEX1=0x00000000004c72a0 HEX2=0x0000000000005980
# make hex- HEX1=0x00000000004c17b0 HEX2=0x00000000004C1000
# 查看 libc 静态链接库的内容
libc.a: /usr/lib/x86_64-linux-gnu/libc.a
	@ls -lh $<
	@ar -t $< | wc -l
	@objdump -t $< | grep '^printf.o' -A 10
#查看文件大小，输出文件名、10 进制字节数和 16 进制字节数，$$ 用于在 makefile 中转义 $
$(BUILD)/%.size: $(BUILD)/%
	@ls -lh $<
	@ls -l $< | awk '{printf "%s %i %x\n", $$9, $$5, $$5}'
# make build/empty.bin.size
#查看指定区域内容
START:=0x0
END:=0x40
$(BUILD)/%.bytes: $(BUILD)/%
	dd if=$< of=$(BUILD)/$*.$(START)-$(END).bytes bs=1 skip=`echo $$(($(START)))` count=`echo $$(($(END) - $(START)))`
# make build/empty.bin.bytes
# make build/empty.bin.bytes START=0x528 END=0x1000
#生成汇编文件
$(BUILD)/%.s: $(SRC)/%.cpp init
	gcc $(CFLAGS) -S $< -o $@
#生成对象文件
$(BUILD)/%.o: $(BUILD)/%.s
	@gcc $(CFLAGS) -c $< -o $@
#生成静态链接库文件
$(BUILD)/%.a: $(BUILD)/%.o
	@ar rcs $@ $< #选项 r 表示添加文件，选项 c 表示新建库文件，选项 s 表示创建符号表
#生成动态链接库文件
$(BUILD)/%.so: $(BUILD)/%.o
	@gcc $(CFLAGS) -fPIC -shared $< -o $@ #-shared 表示产生共享对象，-fPIC
#生成可执行文件
OBJS:=#额外依赖的对象或其他文件
$(BUILD)/%.bin: $(BUILD)/%.o $(OBJS)
	gcc $(CFLAGS)    $^ -o $@
# 生成 elf 解析文件
$(BUILD)/%.readelf: $(BUILD)/%
	$(READELF) -a $< > $@
$(BUILD)/%.strtab: $(BUILD)/%
	@$(READELF) -p .strtab $< > $@
$(BUILD)/%.shstrtab: $(BUILD)/%
	@$(READELF) -p .shstrtab $< > $@
$(BUILD)/%.data: $(BUILD)/%
	@$(READELF) -x .data $< > $@
$(BUILD)/%.got: $(BUILD)/%
	@$(READELF) -x .got $< > $@
# 生成反编译文件
$(BUILD)/%.objdump: $(BUILD)/%
	@objdump -d -f -z $< > $@
#readobj=[read]elf+[obj]dump，没有命令，末尾的分号必须
$(BUILD)/%.readobj: $(BUILD)/%.readelf $(BUILD)/%.strtab $(BUILD)/%.shstrtab $(BUILD)/%.data $(BUILD)/%.objdump $(BUILD)/%.size;
$(BUILD)/%.readobjs: $(BUILD)/%.o.readobj $(BUILD)/%.a.readobj ; #$(BUILD)/%.so.readobj;
$(BUILD)/%.readobjb: $(BUILD)/%.bin.readobj;
$(BUILD)/%.readobjsb: $(BUILD)/%.readobjb $(BUILD)/%.readobjs;
%: $(BUILD)/%.readobjsb;
#运行可执行程序
$(BUILD)/%.run: $(BUILD)/%.bin
	$< &
# make build/empty.run
#生成内存映射文件
$(BUILD)/%.maps: $(BUILD)/%.bin
	cat /proc/`ps -a | grep $*.bin | head -n 1 | awk '{printf $$1}'`/maps > $@
# make build/empty.maps
#结束可执行程序
$(BUILD)/%.stop: $(BUILD)/%.bin
	killall $<
#运行+生成+结束：r=run, m=maps, s=stop。
#运行的程序必须能持续一段时间，等到捕捉到 maps；否则可以使用 lldb 调试，使程序阻塞，再手动调用 maps
$(BUILD)/%.rms: $(BUILD)/%.readobjsb $(BUILD)/%.run $(BUILD)/%.maps $(BUILD)/%.stop;
# make build/sleep.rms
