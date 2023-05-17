################# 构建相关 #################
PIC_OPT:=$(if $(PIC),-fPIC,)#位置无关代码
STATIC_OPT:=$(if $(STATIC),-static,)#静态链接
STRIPPED:=.stripped#是否启用 stripped 模式，剥离汇编、二进制文件中无关内容。默认启用
DEPENDENCE:=#生成 bin 时的依赖文件
#生成汇编文件，支持 c、cpp 源文件
#-fno-builtin#禁用内建函数内联优化，例如：将无参的 printf 替换为 puts
#-fno-stack-protector#防止生成 __stack_chk_fail 调用
$(BUILD)/%.s: c/%.c $(BUILD)
	cc $(CFLAGS) -fno-builtin -fno-stack-protector $(PIC_OPT) -S -o $@ $<
$(BUILD)/%.s: cpp/%.cpp $(BUILD)
	cc $(CFLAGS) -fno-builtin -fno-stack-protector $(PIC_OPT) -S -o $@ $<
#生成对象文件，支持从源目录或者构建目录读取汇编文件
$(BUILD)/%.o: s/%.s $(BUILD)#此处不需要 $(STRIPPED)
	as $(CFLAGS) -c -o $@ $<
$(BUILD)/%.o: $(BUILD)/%.s$(STRIPPED)
	as $(CFLAGS) -c -o $@ $<
#生成静态链接库文件。选项 r 表示添加文件，选项 c 表示新建库文件，选项 s 表示创建符号表
$(BUILD)/%.a: $(BUILD)/%.o$(STRIPPED)
	ar rcs $@ $<
#生成动态链接库文件，-shared 表示产生共享对象
$(BUILD)/%.so: $(BUILD)/%.o$(STRIPPED)
	gcc $(CFLAGS) -shared $(PIC_OPT) -o $@ $<
#生成可执行文件
$(BUILD)/%.bin: $(BUILD)/%.o$(STRIPPED) $(DEPENDENCE)
	gcc $(CFLAGS) $(STATIC_OPT) -o $@ $^
# 生成 elf 解析文件
$(BUILD)/%.readelf: $(BUILD)/%$(STRIPPED)
#   true 防止 readelf 返回非 0 状态导致 make 警告
	readelf -a -W $< > $@ || true
	@readelf -p .strtab $< >> $@
	@readelf -p .shstrtab $< >> $@
$(BUILD)/%.data: $(BUILD)/%$(STRIPPED)
	readelf -x .rodata $< >> $@
#	readelf -x .data $< >> $@
#	readelf -x .data.rel.local $< >> $@
#	readelf -x .rela.data.rel.local $< >> $@
#	readelf -p .interp $< >> $@
# 生成反编译文件
$(BUILD)/%.objdump: $(BUILD)/%$(STRIPPED)
	objdump -d -f -z $< > $@
#	objdump -d -f -z $< | grep -E '>:|call ' > $@.fun
#readobj=[read]elf+[obj]dump，没有命令，末尾的分号必须
$(BUILD)/%.readobj: $(BUILD)/%.readelf $(BUILD)/%.objdump;
$(BUILD)/%.readobjo: $(BUILD)/%.o.readobj;
$(BUILD)/%.readobjso: $(BUILD)/%.readobjo $(BUILD)/%.so.readobj;
$(BUILD)/%.readobjbin: $(BUILD)/%.readobjo $(BUILD)/%.bin.readobj;

#删除无关内容
$(BUILD)/%.s.stripped: $(BUILD)/%.s
	sed '/\.file/d;/\.LF/d;/\.cfi/d;/endbr64/d;/\.ident/d;/.note.GNU-stack/d;/.note.gnu.property/,/4:/d;' $< > $@
SO_SECTIONS:=.comment .hash .note.ABI-tag .note.gnu.build-id .note.gnu.property
$(BUILD)/%.so.stripped: $(BUILD)/%.so
	objcopy $(addprefix --remove-section , $(SO_SECTIONS)) $< $@
# .gnu.version_r：包含符号版本的相关信息，用于动态链接器加载共享库时进行符号解析。
# ELF load command address/offset not page-aligned
# unsupported version 0 of Verneed record
SECTIONS=$(SO_SECTIONS) .gnu.hash .gnu.version .gnu.version_r .eh_frame_hdr .eh_frame
$(BUILD)/%.stripped: $(BUILD)/%
	objcopy $(addprefix --remove-section , $(SECTIONS)) $< $@

################# PIC 相关 #################
$(BUILD)/%.pic:
	 make $*.readobjbin SUBDIR=/$*-pic-no
	 make $*.readobjbin SUBDIR=/$*-pic-ye PIC=TRUE
# make empty.pic
# make singleton.pic
