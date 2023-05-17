#查看变量值，调试时使用。单引号方便看出变量中是否包含空格。
vars:
	@echo "BUILD: '$(BUILD)'"
	@echo "SUBDIR: '$(SUBDIR)'"
	@echo "CFLAGS: '$(CFLAGS)'"
	@echo "STATIC: '$(STATIC)', STATIC_OPT: '$(STATIC_OPT)', LIB_SUFFIX: '$(LIB_SUFFIX)', DYNAMIC_OPT: '$(DYNAMIC_OPT)'"
	@echo "CRT_DIR: '$(CRT_DIR)'"
	@echo "STRIPPED: '$(STRIPPED)'"
	@echo "CUS_LIB: '$(CUS_LIB)', CRT_LIB: '$(CRT_LIB)', CRT_LIB_PATH: '$(CRT_LIB_PATH)', C_LIB: '$(C_LIB)', C_LIB_PATH: '$(C_LIB_PATH)'"
	@echo "ENTRY: '$(ENTRY)', ENTRY_OPT: '$(ENTRY_OPT)', OR ENTRY_OPT: '$(or $(ENTRY_OPT),$(CUS_LIB))'"
	@echo "DEPENDENCE: '$(DEPENDENCE)'"

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
