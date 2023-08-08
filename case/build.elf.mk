# 构建 ELF 文件：预处理、汇编、编译、链接
# CFLAGS+=-g # 生成调试信息

# 预处理文件
$(BUILD)/%.i: $(SRC)/%.c $(BUILD)
	$(CC) $(CFLAGS) -E -o $@ $<
$(BUILD)/%.i: $(SRC)/%.cpp $(BUILD)
	$(CXX) $(CFLAGS) -E -o $@ $<
# 汇编文件
$(BUILD)/%.s: $(BUILD)/%.i
	$(CC) $(CFLAGS) -S -o $@ $<
# 对象文件
$(BUILD)/%.o: $(BUILD)/%.s
	$(CC) $(CFLAGS) -c -o $@ $<

debug.vars+=SECONDARY
ifndef SECONDARY
# 直接构建可执行文件，从源文件编译
$(BUILD)/%.bin: $(SRC)/%.c $(BUILD)
	$(CC) $(CFLAGS)    -o $@ $<
$(BUILD)/%.bin: $(SRC)/%.cpp $(BUILD)
	$(CXX) $(CFLAGS)   -o $@ $<
else
# 分步构建可执行文件，从对象文件编译
$(BUILD)/%.bin: $(BUILD)/%.o
	$(CC) $(CFLAGS)    -o $@ $<
endif
