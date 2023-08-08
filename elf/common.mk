# https://www.gnu.org/software/make/manual/make.html
SRC:=src
SUBDIR:=#在子目录下构建时使用，默认为空格，设置时需要以 / 起始，例如：SUBDIR=/dynamic
BUILD:=build$(SUBDIR)#设置构建目录
#删除 BUILD 中的空格，避免执行 clean 时，误删根目录。如果传入 SUBDIR=' / sub'，则删除命令为 rm -rf build / sub，结果很惨烈，好在是虚拟机
BUILD:=$(shell echo "$(BUILD)" | tr -d ' ')

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
# 此目标无依赖，优先级很高，会取代其他有依赖的任务，不能使用
#$(BUILD)/%:
#	mkdir -p $@
mkdir/%:
	mkdir -p $(BUILD)/$*
#删除构建目录。make clean
clean:
	rm -rf $(BUILD)
#删除指定目录或文件。make clean/singleton
clean/%:
	rm -rf $(BUILD)/$*
