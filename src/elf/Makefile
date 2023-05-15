# https://www.gnu.org/software/make/manual/html_node/Options-Summary.html
SUBDIR:=#在子目录下构建时使用，默认为空格，设置时需要以 / 起始，例如：SUBDIR=/dynamic
BUILD:=build$(SUBDIR)#设置构建目录
BUILD:=$(shell echo "$(BUILD)" | tr -d ' ')#删除 BUILD 中的空格，避免执行 clean 时，误删根目录
#编译选项
CFLAGS:=
V:=#-v
CFLAGS+=$(V)#输出详细的编译信息
# make 命令选项
MFLAGS:=
#MFLAGS+=-s#静默模式，
MFLAGS+=--no-print-directory#屏蔽输出 make[1]: Entering directory '/media/sf_learn-c/elf'

################# 一般性内容 #################
.SECONDARY:#保留中间过程文件
.SUFFIXES:#取消默认文件后缀识别。例如：make empty -> make empty.cpp
.MAKEFLAGS:#取消 make 命令默认的选项和标志
#.SILENT:#静默模式，不会输出执行的命令
#所有未明确匹配的目标，都转到 build 下。例如：make empty.bin -> make build/empty.bin
%: $(BUILD)/%;

#查看变量值。单引号方便看出变量中是否包含空格。
vars:
	@echo "BUILD: '$(BUILD)'"
	@echo "SUBDIR: '$(SUBDIR)'"
	@echo "CFLAGS: '$(CFLAGS)'"
	@echo "STATIC: '$(STATIC_OPT)'"
	@echo "CRT_DIR: '$(CRT_DIR)'"
	@echo "STRIPPED: '$(STRIPPED)'"
	@echo "C_LIB: '$(C_LIB)'"
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
#-fno-stack-protector#防止生成 __stack_chk_fail 调用
PIC_OPT:=$(if $(filter TRUE,$(PIC)),-fPIC,)
$(BUILD)/%.s: c/%.c $(BUILD)
	cc $(CFLAGS) -fno-builtin -fno-stack-protector $(PIC_OPT) -S -o $@ $<
#	gcc $(CFLAGS) -fno-builtin -S -o $@ $<
$(BUILD)/%.s: cpp/%.cpp $(BUILD)
	cc $(CFLAGS) -fno-builtin -fno-stack-protector $(PIC_OPT) -S -o $@ $<
#	gcc $(CFLAGS) -fno-builtin -S -o $@ $<
STRIPPED:=.stripped#是否启用 stripped 模式，剥离汇编、二进制文件中无关内容。默认启用
#删除汇编中无关内容
$(BUILD)/%.s.stripped: $(BUILD)/%.s
	sed '/\.file/d;/\.LF/d;/\.cfi/d;/endbr64/d;/\.ident/d;/.note.GNU-stack/d;/.note.gnu.property/,/4:/d;' $< > $@
#从可执行文件中删除某些结构
$(BUILD)/%.stripped: $(BUILD)/%
	@objcopy \
			--remove-section .comment \
			--remove-section .hash \
			--remove-section .gnu.hash \
			--remove-section .gnu.version \
			--remove-section .eh_frame_hdr \
			--remove-section .eh_frame \
			--remove-section .note.ABI-tag \
			--remove-section .note.gnu.build-id \
			--remove-section .note.gnu.property \
			 $< $@
#			--remove-section .gnu.version_r \#包含符号版本的相关信息，用于动态链接器加载共享库时进行符号解析。

#生成对象文件，支持从源目录或者构建目录读取汇编文件
$(BUILD)/%.o: s/%.s$(STRIPPED)
	as $(CFLAGS) -c -o $@ $<
#	gcc $(CFLAGS) -c -o $@ $<
$(BUILD)/%.o: $(BUILD)/%.s$(STRIPPED)
	as $(CFLAGS) -c -o $@ $<
#	gcc $(CFLAGS) -c -o $@ $<
#生成静态链接库文件。选项 r 表示添加文件，选项 c 表示新建库文件，选项 s 表示创建符号表
$(BUILD)/%.a: $(BUILD)/%.o$(STRIPPED)
	@ar rcs $@ $<
#生成动态链接库文件，-shared 表示产生共享对象
$(BUILD)/%.so: $(BUILD)/%.o$(STRIPPED)
	gcc $(CFLAGS) -shared $(PIC_OPT) -o $@ $<
#生成可执行文件
#静态编译。创建动态共享库时不能指定此参数 https://stackoverflow.com/questions/44429253/building-a-shared-library-created-a-static-library-instead
STATIC_OPT:=$(if $(filter TRUE,STATIC),-static,)
#$(BUILD)/crt0.o /usr/lib/x86_64-linux-gnu/crt1.o /usr/lib/x86_64-linux-gnu/libc.a
$(BUILD)/%.bin: $(BUILD)/%.o$(STRIPPED) $(OBJS)
	gcc $(CFLAGS) $(STATIC_OPT) -o $@ $^
#	ld $(CFLAGS) $(STATIC_OPT) -o $@ $^
# 生成 elf 解析文件
$(BUILD)/%.readelf: $(BUILD)/%
#   true 防止 readelf 返回非 0 状态导致 make 警告
	readelf -a -W $< > $@ || true
	@readelf -p .strtab $< >> $@
	@readelf -p .shstrtab $< >> $@
$(BUILD)/%.data: $(BUILD)/%
	readelf -x .rodata $< >> $@
#	readelf -x .data $< >> $@
#	readelf -x .data.rel.local $< >> $@
#	readelf -x .rela.data.rel.local $< >> $@
#	readelf -p .interp $< >> $@
# 生成反编译文件
$(BUILD)/%.objdump: $(BUILD)/%
	objdump -d -f -z $< > $@
#	objdump -d -f -z $< | grep -E '>:|call ' > $@.fun
#readobj=[read]elf+[obj]dump，没有命令，末尾的分号必须
$(BUILD)/%.readobj: $(BUILD)/%.readelf $(BUILD)/%.objdump;
$(BUILD)/%.readobjo: $(BUILD)/%.o$(STRIPPED).readobj;
$(BUILD)/%.readobjso: $(BUILD)/%.readobjo $(BUILD)/%.so$(STRIPPED).readobj ;
$(BUILD)/%.readobjbin: $(BUILD)/%.readobjo $(BUILD)/%.bin$(STRIPPED).readobj $(if $(filter TRUE,$(STATIC)),,$(BUILD)/%.readobjso);

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
	xxd -p $(BUILD)/$*.$(START)-$(END).bytes

# make build/empty.bin.bytes
# make build/empty.bin.bytes START=0x528 END=0x1000

################# 运行相关 #################
#运行可执行程序
BACK:=#'&'
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
# make sleep.rms SUBDIR=/sleep-static STATIC=TRUE BACK='&'

################# PIC 相关 #################
$(BUILD)/%.pic:
	 make $*.readobjbin SUBDIR=/$*-pic-no
	 make $*.readobjbin SUBDIR=/$*-pic-ye PIC=TRUE
# make empty.pic
# make singleton.pic

################# ELF 文件构建过程 #################
ELF:=$(BUILD)/elf
$(BUILD)/process:
	mkdir -p $(ELF)
	cc -S -o $(ELF)/crtmini.s c/crtmini.c
	cc -S -o $(ELF)/empty.s c/empty.c
	as -c -o $(ELF)/crtmini.o $(ELF)/crtmini.s
	as -c -o $(ELF)/empty.o $(ELF)/empty.s
	ld    -o $(ELF)/crtmini_empty.o -r $(ELF)/crtmini.o $(ELF)/empty.o
	ar rcs $(ELF)/crtmini.a $(ELF)/crtmini.o
	ar rcs $(ELF)/empty.a $(ELF)/empty.o
	ar rcs $(ELF)/crtmini.o_empty.o.a $(ELF)/crtmini.o $(ELF)/empty.o
	ar rcs $(ELF)/crtmini_empty.a $(ELF)/crtmini_empty.o
#
	ld -shared -o $(ELF)/crtmini.so $(ELF)/crtmini.o
	ld -shared -o $(ELF)/empty.so $(ELF)/empty.o
	ld -shared -o $(ELF)/crtmini.o_empty.so.so $(ELF)/empty.o $(ELF)/crtmini.so
#
	ld    -o $(ELF)/crtmini.bin $(ELF)/crtmini.o
	ld    -o $(ELF)/empty.bin $(ELF)/empty.o	 || true
	ld    -o $(ELF)/crtmini_empty.bin $(ELF)/crtmini_empty.o
	ld    -o $(ELF)/crtmini.o_empty.o.bin $(ELF)/crtmini.o $(ELF)/empty.o
	ld    -o $(ELF)/crtmini.o_empty.so.bin $(ELF)/crtmini.o $(shell pwd)/$(ELF)/empty.so
#
	ld    -o $(ELF)/crtmini.so.bin $(ELF)/crtmini.so || true
	ld    -o $(ELF)/empty.so.bin $(ELF)/empty.so	 || true
	ld    -o $(ELF)/crtmini.so_empty.so.bin $(shell pwd)/$(ELF)/crtmini.so $(shell pwd)/$(ELF)/empty.so	 || true
# /lib/ld64.so.1
# ln /lib64/ld-linux-x86-64.so.2 /lib/ld64.so.1
# ld  -dynamic-linker /lib64/ld-linux-x86-64.so.2

################# 自定义类库相关 #################
# CUS_LIB: 使用自定义类库，LIB_NONE LIB_CRT LIB_C

$(BUILD)/all-in-one.bin: $(BUILD)/all-in-one.o
	ld $(V) -static -e nomain -o $@ $<

# LIB_TYPE=o|so，类库类型
ifndef LIB_TYPE
LIB_TYPE:=$(if $(filter TRUE,$(STATIC)),o,so)
endif

ifdef CUS_LIB

ifneq ($(STATIC),TRUE)
DYNAMIC_OPT:=-pie -dynamic-linker /lib64/ld-linux-x86-64.so.2
endif

ENTRY:=$(if $(filter CRT,$(CUS_LIB)),,-e main)
CRT_LIB:=$(if $(filter CRT,$(CUS_LIB)),$(BUILD)/crtmini,)
C_LIB:=$(if $(filter C,$(CUS_LIB)),$(BUILD)/libmini,)
CRT_READOBJ:=$(if $(CRT_LIB),$(CRT_LIB).readobjo,)
C_READOBJ:=$(if $(C_LIB),$(C_LIB).readobj$(LIB_TYPE),)
#共享类库需要使用全路径，否则解析器无法识别，CRT_LIB 目前只能使用 o 文件，将来可改成 so 文件
CRT_LIB_PATH:=$(if $(CRT_LIB),$(shell pwd)/$(CRT_LIB).o,)
C_LIB_PATH:=$(if $(C_LIB),$(shell pwd)/$(C_LIB).$(LIB_TYPE),)
# gcc  -shared -fPIC -o build/empty-crtmini-dynamic/empty.so build/empty-crtmini-dynamic/empty.o.stripped build/empty-crtmini-dynamic/crtmini.o.stripped
#$(BUILD)/%.bin: $(BUILD)/%.o build/crtmini.o 此写法因为依赖项多出一个 crtmini.o，导致优先级较低，无法覆盖先前的定义
#$(C_LIB_PATH)
$(BUILD)/%.so: $(BUILD)/%.o$(STRIPPED)
	ld $(CFLAGS) -shared $(PIC_OPT) -o $@ $(CRT_LIB_PATH) $<
$(BUILD)/%.bin: $(BUILD)/%.o$(STRIPPED)
#   添加一个 make 命令，手动构建依赖项。$(BUILD) 作为占位命令，当 CRT_READOBJ 和 C_READOBJ 都为空时有效
	make $(CRT_READOBJ) $(C_READOBJ) $(BUILD)
	ld $(V) $(STATIC_OPT) $(DYNAMIC_OPT) $(ENTRY) -o $@ $(CRT_LIB_PATH) $(C_LIB_PATH) $<
endif


################# 类库案例 #################
#统计文件行数
$(BUILD)/%.wc: $(BUILD)/%
	@wc -l $<

BUILD_TYPES:=default main crtmini
BUILD_WAYS:=static dynamic
EMPTY_TYPE_WAYS:=$(foreach type,$(BUILD_TYPES),$(foreach way,$(BUILD_WAYS),empty-$(type)-$(way)))
EMPTY_TYPE_WAYS_CLEAN:=$(addsuffix .clean, $(EMPTY_TYPE_WAYS))
EMPTY_TYPE_WAYS_RUN:=$(addsuffix /empty.run, $(EMPTY_TYPE_WAYS))

%.case:
	make $(MFLAGS) $*.readobjbin SUBDIR=/$*-default-static STATIC=TRUE
	make $(MFLAGS) $*.readobjbin SUBDIR=/$*-default-dynamic PIC=TRUE
	make $(MFLAGS) $*.readobjbin SUBDIR=/$*-main-static  CUS_LIB='$(if $(filter $*,empty),NONE,C)' STATIC=TRUE
	make $(MFLAGS) $*.readobjbin SUBDIR=/$*-main-dynamic CUS_LIB='$(if $(filter $*,empty),NONE,C)' PIC=TRUE
	make $(MFLAGS) $*.readobjbin SUBDIR=/$*-crtmini-static CUS_LIB='CRT $(if $(filter $*,empty),,C)' STATIC=TRUE
	make $(MFLAGS) $*.readobjbin SUBDIR=/$*-crtmini-dynamic CUS_LIB='CRT $(if $(filter $*,empty),,C)' PIC=TRUE
#	make $(MFLAGS) $*$(STRIPPED).bin.bytes SUBDIR=/$*-main-static START=0xb0 END=0x1000
# make empty.case
# make singleton.case


%.case.run:
	@make $(MFLAGS) $*.run SUBDIR=/$*-default-static
	@make $(MFLAGS) $*.run SUBDIR=/$*-default-dynamic
	@make $(MFLAGS) $*.run SUBDIR=/$*-crtmini-static
	@make $(MFLAGS) $*.run SUBDIR=/$*-crtmini-dynamic
	@make $(MFLAGS) $*.run SUBDIR=/$*-main-static
	@make $(MFLAGS) $*.run SUBDIR=/$*-main-dynamic

%.case.clean:
	make $(MFLAGS) $(addprefix $(BUILD)/,$(patsubst empty%, $*%,$(EMPTY_TYPE_WAYS_CLEAN)))
empty.wc: $(addprefix $(BUILD)/, $(addsuffix /empty$(STRIPPED).bin.readelf.wc, $(EMPTY_TYPE_WAYS)));
# ld -r file1.o file2.o -o merged.o

################# log 相关 #################
log_main: $(BUILD)/log_main_info.readobjbin $(BUILD)/log_main_debug.readobjbin;
ifeq ($(STATIC_OPT),-static)
#静态链接
log: $(BUILD)/log_lib.o.readobj log_main;
$(BUILD)/log_%.bin: $(BUILD)/log_%.o $(BUILD)/log_lib.o
	gcc -o $@ $^
else
#动态链接
log: $(BUILD)/log_lib.readobjso log_main;
$(BUILD)/log_%.bin: $(BUILD)/log_%.o $(BUILD)/log_lib.so
	gcc $(CFLAGS) -o $@ $^
endif
# make log SUBDIR=/log/static STATIC=
# make log SUBDIR=/log/dynamic PIC=-fPIC
# make log_main_debug.lldb SUBDIR=/log/dynamic
# make log_main_debug.bin.maps SUBDIR=/log/dynamic

# make log_lib.o.readobj SUBDIR=/log/static/ye-plt STATIC=
# make log_lib.o.readobj SUBDIR=/log/static/no-plt STATIC=TRUE PLT=-fno-plt

# make log_lib.o.readobj SUBDIR=/pic-y PIC=-fPIC
# make log_lib.o.readobj SUBDIR=/pic-n
# make log_lib.o.readobj SUBDIR=/log/static STATIC=
# make log_lib.o.readobj SUBDIR=/log/dynamic
