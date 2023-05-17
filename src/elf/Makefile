# https://www.gnu.org/software/make/manual/html_node/Options-Summary.html
SUBDIR:=#在子目录下构建时使用，默认为空格，设置时需要以 / 起始，例如：SUBDIR=/dynamic
BUILD:=build$(SUBDIR)#设置构建目录
#删除 BUILD 中的空格，避免执行 clean 时，误删根目录。如果传入 SUBDIR=' / sub'，则删除命令为 rm -rf build / sub，结果很惨烈，好在是虚拟机
BUILD:=$(shell echo "$(BUILD)" | tr -d ' ')

#编译选项
CFLAGS:=
V:=#-v
CFLAGS+=$(V)#输出详细的编译信息

# make 命令选项
MAKEFLAGS:=
#MAKEFLAGS+=-s#静默模式，
#MAKEFLAGS+=--debug
MAKEFLAGS+=--no-print-directory#屏蔽输出 make[1]: Entering directory '/media/sf_learn-c/elf'

################# 基础配置 #################
.SECONDARY:#保留中间过程文件
.SUFFIXES:#取消默认文件后缀识别。例如：make empty -> make empty.cpp
.MAKEFLAGS:#取消 make 命令默认的选项和标志
#.SILENT:#静默模式，不会输出执行的命令
#所有未明确匹配的目标，都转到 build 下。例如：make empty.bin -> make build/empty.bin
%: $(BUILD)/%;

#创建构建目录。make build
$(BUILD):
	mkdir -p $@
#删除构建目录。make clean
clean:
	rm -rf $(BUILD)
#删除指定目录或文件。make singleton.clean
$(BUILD)/%.clean:
	rm -rf $(BUILD)/$*

#主线：构建可执行文件，分析文件内容。
include build-gnu.mk    #构建时使用 gnu 提供的类库，包括：crt1.o、libc.a 等
include build-mini.mk   #构建时使用 个人 提供的类库，包括：crtmini.c、libmini.c。gnu 提供的类库内容庞大不便于分析，个人类库很小。
include build-case.mk   #构建出分析时需要的针对性案例，基于 个人类库 构建以便分析。
#支线：
include gnu.mk   		#看看 gnu 类库的内容
include command.mk		#测试可能的命令组合方式
include other.mk  		#其他
