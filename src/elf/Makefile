SRC:=cpp#设置源文件目录
SUBDIR:=#在子目录下构建时使用，默认为空格，设置时需要以 / 起始，例如：SUBDIR=/dynamic
BUILD:=build$(SUBDIR)#设置构建目录
BUILD:=$(shell echo "$(BUILD)" | tr -d ' ')#删除 BUILD 中的空格，避免执行 clean 时，误删根目录
#编译选项
CFLAGS:=
V:=#-v
CFLAGS+=$(V)#输出详细的编译信息
PIC:=#-fPIC
CFLAGS+=$(PIC)#位置无关代码


################# 一般性内容 #################
.SECONDARY:#保留中间过程文件
.SUFFIXES:#取消默认文件后缀识别。make empty -> make empty.cpp
.MAKEFLAGS:#取消 make 命令默认的选项和标志
#所有明确匹配的目标，都转到 build 下。make empty.bin -> make build/empty.bin
%: $(BUILD)/%;

#查看变量值。单引号方便看出变量中是否包含空格
vars:
	@echo "OS: '$(OS)'"
	@echo "SUBDIR: '$(SUBDIR)'"
	@echo "SRC: '$(SRC)'"
	@echo "BUILD: '$(BUILD)'"
	@echo "CFLAGS: '$(CFLAGS)'"
	@echo "STATIC: '$(STATIC)'"
	@echo "CRT_DIR: '$(CRT_DIR)'"
#创建构建目录
$(BUILD):
	@mkdir -p $@;
#删除构建目录。make clean
clean:
	@rm -rf $(BUILD)
#删除指定目录或文件。make all-in-one.clean
$(BUILD)/%.clean:
	rm -rf $(BUILD)/$*

################# crt（C runtime）相关 #################
CRT_DIR:=$(BUILD)/crt
$(CRT_DIR):
	mkdir -p $@
	cp /usr/lib/x86_64-linux-gnu/crt* $@
	cp /usr/lib/gcc/x86_64-linux-gnu/11/crt* $@
#	@ls -la /usr/lib/gcc/x86_64-linux-gnu/11 | grep crt
#	@ls -la /usr/lib/x86_64-linux-gnu | grep crt
$(BUILD)/crts: $(CRT_DIR)			\
	 $(CRT_DIR)/crt1.o.readobj		\
	 $(CRT_DIR)/crti.o.readobj		\
	 $(CRT_DIR)/crtn.o.readobj		\
	 $(CRT_DIR)/crtbegin.o.readobj	\
	 $(CRT_DIR)/crtbeginS.o.readobj	\
	 $(CRT_DIR)/crtbeginT.o.readobj	\
	 $(CRT_DIR)/crtend.o.readobj	\
	 $(CRT_DIR)/crtendS.o.readobj	\
	;
# crt1.o crti.o crtn.o crtbegin.o crtbeginS.o crtbeginT.o crtend.o crtendS.o
# 查看 libc 静态链接库的内容
libc.a: /usr/lib/x86_64-linux-gnu/libc.a
	ls -lh $<
	ar -t $< | wc -l
	objdump -t $< | grep '^printf.o' -A 10
# ar -t /usr/lib/x86_64-linux-gnu/libc.a | grep printf.o

################# 构建相关 #################
#生成汇编文件，支持 c、cpp 源文件
#-fno-builtin#禁用内建函数内联优化，例如：将无参的 printf 替换为 puts
$(BUILD)/%.s: c/%.c $(BUILD)
	cc $(CFLAGS) -fno-builtin -S -o $@ $< #	gcc $(CFLAGS) -fno-builtin -S -o $@ $<
$(BUILD)/%.s: cpp/%.cpp $(BUILD)
	cc $(CFLAGS) -fno-builtin -S -o $@ $< #	gcc $(CFLAGS) -fno-builtin -S -o $@ $<
#生成对象文件，支持从源目录或者构建目录读取汇编文件
$(BUILD)/%.o: s/%.s
	as $(CFLAGS) -c -o $@ $< #	gcc $(CFLAGS) -c -o $@ $<
$(BUILD)/%.o: $(BUILD)/%.s
#删除汇编中无关内容
	sed -i.bak '/.file/d;/.cfi/d;/.ident/d;/.note.GNU-stack/d;/.note.gnu.property/,/4:/d;' $<
	as $(CFLAGS) -c -o $@ $< #	gcc $(CFLAGS) -c -o $@ $<
#生成静态链接库文件。选项 r 表示添加文件，选项 c 表示新建库文件，选项 s 表示创建符号表
$(BUILD)/%.a: $(BUILD)/%.o
	@ar rcs $@ $<
#生成动态链接库文件，-shared 表示产生共享对象
$(BUILD)/%.so: $(BUILD)/%.o
	gcc $(CFLAGS) -shared -o $@ $<
#生成可执行文件
STATIC:=#-static #静态编译。创建动态共享库时不能指定此参数 https://stackoverflow.com/questions/44429253/building-a-shared-library-created-a-static-library-instead
$(BUILD)/%.bin: $(BUILD)/%.o $(OBJS) #$(BUILD)/crt0.o /usr/lib/x86_64-linux-gnu/crt1.o /usr/lib/x86_64-linux-gnu/libc.a
	gcc $(CFLAGS) $(STATIC) -o $@ $^ #	ld $(CFLAGS) $(STATIC) -o $@ $^
# 生成 elf 解析文件
$(BUILD)/%.readelf: $(BUILD)/%
	readelf -a $< > $@
$(BUILD)/%.data: $(BUILD)/%
	readelf -x .data $< >> $@
	readelf -x .data.rel.local $< >> $@
	readelf -x .rela.data.rel.local $< >> $@
	readelf -s .interp $< >> $@
# 生成反编译文件
$(BUILD)/%.objdump: $(BUILD)/%
	objdump -d -f -z $< > $@
	objdump -d -f -z $< | grep -E '>:|call ' > $@.fun
#readobj=[read]elf+[obj]dump，没有命令，末尾的分号必须
$(BUILD)/%.readobj: $(BUILD)/%.readelf $(BUILD)/%.objdump;
$(BUILD)/%.readobjo:  $(BUILD)/%.o.readobj;
$(BUILD)/%.readobjso: $(BUILD)/%.so.readobj $(BUILD)/%.readobjo;
$(BUILD)/%.readobjbin: $(BUILD)/%.bin.readobj $(BUILD)/%.readobjo;

################# 查看相关 #################
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

################# 运行相关 #################
#运行可执行程序。make build/empty.run
BACK:=
$(BUILD)/%.run: $(BUILD)/%.bin
	$< $(BACK)
#调试可执行程序。make build/empty.lldb
$(BUILD)/%.lldb: $(BUILD)/%.bin
	lldb $<
#生成内存映射文件。ps -f 选项展示 CMD 全部内容，默认会截断 CMD 名称。make build/empty.bin.maps
$(BUILD)/%.maps:
#	cat /proc/`ps -a | grep "$*" | head -n 1 | awk '{printf $$1}'`/maps > $@
	cat /proc/`ps -af | grep -E "elf.*$*" | head -n 1 | awk '{printf $$2}'`/maps > $@
#结束可执行程序
$(BUILD)/%.stop: $(BUILD)/%.bin
	killall $<
#运行+生成+结束：r=run, m=maps, s=stop。
#运行的程序必须能持续一段时间，等到捕捉到 maps；否则可以使用 lldb 调试，使程序阻塞，再手动调用 maps
$(BUILD)/%.rms: $(BUILD)/%.readobjbin $(BUILD)/%.run $(BUILD)/%.bin.maps $(BUILD)/%.stop;
# make sleep.rms SUBDIR=/sleep BACK=&

################# 自定义 crt 相关 #################
$(BUILD)/all-in-one.bin: $(BUILD)/all-in-one.o
	ld $(V) -static -e nomain -o $@ $<
ifeq ($(CRT),NONE)
$(BUILD)/%.bin: $(BUILD)/%.o
	ld $(V) -static -e main -o $@ $^
else ifeq ($(CRT),MINI)
$(BUILD)/%.bin: $(BUILD)/%.o $(BUILD)/crtmini.o
	ld $(V) -static -o $@ $^
$(BUILD)/%.readobjo: $(BUILD)/%.o.readobj $(BUILD)/crtmini.o.readobj;
endif

#minibash 小实验：
# make minibash.clean minibash.bin
# build/minibash.bin
# cat /etc/passwd | grep root
#$ sed -i 's|/bin/bash|/media/sf_learn-c/elf/build/minibash.bin|' /etc/passwd
#$ sed -i 's|/media/sf_learn-c/elf/build/minibash.bin|/bin/bash|' /etc/passwd
#$ ssh root@test-node01

# crtmini 小实验：
# make empty-crtnone.clean
# make empty.run SUBDIR=/empty-crtnone CRT=NONE
# make empty-crtmini.clean
# make empty.run SUBDIR=/empty-crtmini CRT=MINI
# make all-in-one.clean
# make all-in-one.run SUBDIR=/all-in-one

# 生命周期小实验：
# make init_fini.o.clean init_fini.o.readobj
# make lifecycle.clean
# make lifecycle.run OBJS=build/init_fini.o SUBDIR=/lifecycle

################# group 相关 #################
group_main: $(BUILD)/group_main_info.readobjbin $(BUILD)/group_main_debug.readobjbin;
ifeq ($(STATIC),-static)
#静态链接
group: $(BUILD)/group_lib.o.readobj group_main;
$(BUILD)/group_%.bin: $(BUILD)/group_%.o $(BUILD)/group_lib.o
	gcc $(CFLAGS) -o $@ $^
else
#动态链接
group: $(BUILD)/group_lib.readobjso group_main;
$(BUILD)/group_%.bin: $(BUILD)/group_%.o $(BUILD)/group_lib.so
	gcc $(CFLAGS) -o $@ $^
endif
# make group SUBDIR=/group/static STATIC=-static
# make group SUBDIR=/group/dynamic PIC=-fPIC
# make group_main_debug.lldb SUBDIR=/group/dynamic
# make group_main_debug.bin.maps SUBDIR=/group/dynamic

# make group_lib.o.readobj SUBDIR=/group/static/ye-plt STATIC=-static
# make group_lib.o.readobj SUBDIR=/group/static/no-plt STATIC=-static PLT=-fno-plt

# make group_lib.o.readobj SUBDIR=/pic-y PIC=-fPIC
# make group_lib.o.readobj SUBDIR=/pic-n
# make group_lib.o.readobj SUBDIR=/group/static STATIC=-static
# make group_lib.o.readobj SUBDIR=/group/dynamic

# make empty.readobjbin SUBDIR=/empty/static STATIC=-static V=-v
# make empty.readobjbin SUBDIR=/empty/dynamic
# make empty.lldb SUBDIR=/empty/static
# make empty.bin.maps SUBDIR=/empty/static
# make empty.lldb SUBDIR=/empty/dynamic
# make empty.bin.maps SUBDIR=/empty/dynamic
