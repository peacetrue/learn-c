CPP:=./cpp
ASM:=./asm
BUILD:=./build
#编译选项
CFLAGS:=
CFLAGS+= -fno-builtin
#CFLAGS+= -masm=intel

all: init reorder
init:
	mkdir -p $(ASM)
	mkdir -p $(BUILD)
clean:
	rm -rf $(BUILD)
	rm -rf $(ASM)
%: $(CPP)/%.cpp
	gcc $(CFLAGS) -S -O0 $< -o $(ASM)/$@-O0.s
	gcc $(CFLAGS) -S -O1 $< -o $(ASM)/$@-O1.s
	gcc $(CFLAGS) -S -O1 -DVOLATILE=volatile $< -o $(ASM)/$@-O1-volatile.s
	gcc $(CFLAGS) -O0 $< -o $(BUILD)/$@-O0.bin
	gcc $(CFLAGS) -O1 $< -o $(BUILD)/$@-O1.bin
	gcc $(CFLAGS) -O1 -DVOLATILE=volatile $< -o $(BUILD)/$@-O1-volatile.bin

# ./build/reorder-O0.bin
# ./build/reorder-O1-volatile.bin
# ./build/reorder-O1.bin
