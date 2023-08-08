################# GNU 类库相关 #################
#$(BUILD)/crt0.o /usr/lib/x86_64-linux-gnu/crt1.o /usr/lib/x86_64-linux-gnu/libc.a
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
