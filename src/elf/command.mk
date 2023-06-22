################# 测试可能的命令组合方式 #################
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
# /lib/ld64.so.1 不存在
# ln /lib64/ld-linux-x86-64.so.2 /lib/ld64.so.1
# ld  -dynamic-linker /lib64/ld-linux-x86-64.so.2
