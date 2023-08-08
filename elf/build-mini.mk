################# 自定义类库相关 #################
# CUS_LIB: 使用自定义类库，包括：NONE CRT C。
# NONE 不使用任何类库，例如：singleton.c
# CRT  使用 CRT 类库
# C    使用 C   类库
ENTRY_OPT:=$(if $(ENTRY),-e $(ENTRY))# ENTRY: 入口
DYNAMIC_OPT:=$(if $(STATIC),,-dynamic-linker /lib64/ld-linux-x86-64.so.2 -pie)
LIB_SUFFIX:=$(if $(STATIC),o,so)# LIB_SUFFIX=o|so，类库类型
CRT_LIB:=$(if $(filter CRT,$(CUS_LIB)),$(BUILD)/crtmini,)
C_LIB:=$(if $(filter C,$(CUS_LIB)),$(BUILD)/libmini,)
CRT_READOBJ:=$(if $(CRT_LIB),$(CRT_LIB).readobjo,)
C_READOBJ:=$(if $(C_LIB),$(C_LIB).readobj$(LIB_SUFFIX),)
#共享类库需要使用全路径，否则解析器无法识别，CRT_LIB 目前只能使用 o 文件，将来可改成 so 文件
CRT_LIB_PATH:=$(if $(CRT_LIB),$(shell pwd)/$(CRT_LIB).o$(STRIPPED),)
C_LIB_PATH:=$(if $(C_LIB),$(shell pwd)/$(C_LIB).$(LIB_SUFFIX)$(STRIPPED),)

ifneq ($(or $(ENTRY_OPT),$(CUS_LIB)),)
$(BUILD)/%.so: $(BUILD)/%.o$(STRIPPED)
	ld -z norelro $(CFLAGS) -shared $(PIC_OPT) -o $@ $(if $(filter $*,libmini),,$(C_LIB_PATH)) $<
#$(BUILD)/%.bin: $(BUILD)/%.o build/crtmini.o 此写法因为依赖项多出一个 crtmini.o，导致优先级较低，无法覆盖先前的定义
$(BUILD)/%.bin: $(BUILD)/%.o$(STRIPPED) $(DEPENDENCE)
#   添加一个 make 命令，手动构建依赖项。$(BUILD) 作为占位命令，当 CRT_READOBJ 和 C_READOBJ 都为空时有效
	make $(MAKEFLAGS) $(CRT_READOBJ) $(C_READOBJ) $(BUILD)
	ld -z norelro $(V) $(STATIC_OPT) $(DYNAMIC_OPT) $(ENTRY_OPT) -o $@ $(CRT_LIB_PATH) $(C_LIB_PATH) $^
endif
