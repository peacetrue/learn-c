################# 案例 #################
include run.mk

BUILD_TYPES:=default none custom
BUILD_WAYS:=static dynamic
EMPTY_TYPE_WAYS:=$(foreach type,$(BUILD_TYPES),$(foreach way,$(BUILD_WAYS),empty-$(type)-$(way)))
EMPTY_TYPE_WAYS_CLEAN:=$(addprefix clean/, $(EMPTY_TYPE_WAYS))
EMPTY_TYPE_WAYS_RUN:=$(addsuffix /empty.run, $(EMPTY_TYPE_WAYS))

%.case:
	make $(MAKEFLAGS) $*.readobjbin SUBDIR=/$*-default-static STATIC=TRUE
	make $(MAKEFLAGS) $*.readobjbin SUBDIR=/$*-default-dynamic PIC=TRUE
	make $(MAKEFLAGS) $*.readobjbin SUBDIR=/$*-none-static  CUS_LIB='$(if $(filter $*,empty),NONE,C)' STATIC=TRUE ENTRY=main
	make $(MAKEFLAGS) $*.readobjbin SUBDIR=/$*-none-dynamic CUS_LIB='$(if $(filter $*,empty),NONE,C)' PIC=TRUE ENTRY=main
	make $(MAKEFLAGS) $*.readobjbin SUBDIR=/$*-custom-static CUS_LIB='CRT $(if $(filter $*,empty),,C)' STATIC=TRUE
	make $(MAKEFLAGS) $*.readobjbin SUBDIR=/$*-custom-dynamic CUS_LIB='CRT $(if $(filter $*,empty),,C)' PIC=TRUE
#	make $(MAKEFLAGS) $*$(STRIPPED).bin.bytes SUBDIR=/$*-none-static START=0xb0 END=0x1000
# make empty.case
# make variable.case
# make class.case

%.case.run:
	@make $(MAKEFLAGS) $*.run SUBDIR=/$*-default-static		|| true
	@make $(MAKEFLAGS) $*.run SUBDIR=/$*-default-dynamic	|| true
	@make $(MAKEFLAGS) $*.run SUBDIR=/$*-none-static  		|| true
	@make $(MAKEFLAGS) $*.run SUBDIR=/$*-none-dynamic 		|| true
	@make $(MAKEFLAGS) $*.run SUBDIR=/$*-custom-static		|| true
	@make $(MAKEFLAGS) $*.run SUBDIR=/$*-custom-dynamic		|| true

clean/%.case:
	make $(MAKEFLAGS) $(addprefix $(BUILD)/,$(patsubst empty%, $*%,$(EMPTY_TYPE_WAYS_CLEAN)))

#统计文件行数
$(BUILD)/%.wc: $(BUILD)/%
	@wc -l $<
empty.case.wc: $(addprefix $(BUILD)/, $(addsuffix /empty.bin$(STRIPPED).readelf.wc, $(EMPTY_TYPE_WAYS)));

variable.case:
	make $(MAKEFLAGS) variable.readobjbin SUBDIR=/variable-default-static  STATIC=TRUE
	make $(MAKEFLAGS) variable.readobjbin SUBDIR=/variable-default-dynamic PIC=TRUE

singleton.case:
	make singleton.readobjbin singleton.run SUBDIR=/singleton-none-static CUS_LIB=NONE ENTRY=nomain STATIC=TRUE
	make singleton.readobjbin singleton.run SUBDIR=/singleton-none-dynamic CUS_LIB=NONE ENTRY=nomain #TODO 没有输出打印字符
crtmini.case:
	make crtmini.readobjbin crtmini.run SUBDIR=/crtmini-none-static  CUS_LIB=NONE STATIC=TRUE
	make crtmini.readobjbin crtmini.run SUBDIR=/crtmini-none-dynamic CUS_LIB=NONE
lifecycle.case:
	make init_fini.o$(STRIPPED) SUBDIR=/lifecycle-default-static #TODO 此处可以构建成功，为何下面无法通过依赖项构建成功？没有 $(STRIPPED) 可以成功
	make lifecycle.readobjbin lifecycle.run SUBDIR=/lifecycle-default-static DEPENDENCE=$(BUILD)/lifecycle-default-static/init_fini.o$(STRIPPED) STATIC=TRUE
#	make lifecycle.readobjbin lifecycle.run SUBDIR=/lifecycle-default-static DEPENDENCE=$(BUILD)/lifecycle-default-static/init_fini.o$(STRIPPED)
#   上面命令出错：.preinit_array section is not allowed in DSO
#STRIPPED:=
log.case:
#	make libmini.readobjso log_lib.readobjso  SUBDIR=/log-custom-dynamic-pic-ye CUS_LIB=C PIC=TRUE
#	make log_main_info.readobjbin  log_main_info.run  SUBDIR=/log-custom-dynamic-pic-ye CUS_LIB='CRT C' PIC=TRUE STRIPPED=$(STRIPPED) DEPENDENCE=$(shell pwd)/$(BUILD)/log-custom-dynamic/log_lib.so$(STRIPPED)
#	make log_main_debug.readobjbin log_main_debug.run SUBDIR=/log-custom-dynamic-pic-ye CUS_LIB='CRT C' PIC=TRUE STRIPPED=$(STRIPPED) DEPENDENCE=$(shell pwd)/$(BUILD)/log-custom-dynamic/log_lib.so$(STRIPPED)
	make libmini.readobjso log_lib.readobjso  SUBDIR=/log-custom-dynamic-pic-no CUS_LIB=C
	make log_main_info.readobjbin  log_main_info.run  SUBDIR=/log-custom-dynamic-pic-no CUS_LIB='CRT C' STRIPPED=$(STRIPPED) DEPENDENCE=$(shell pwd)/$(BUILD)/log-custom-dynamic/log_lib.so$(STRIPPED)
	make log_main_debug.readobjbin log_main_debug.run SUBDIR=/log-custom-dynamic-pic-no CUS_LIB='CRT C' STRIPPED=$(STRIPPED) DEPENDENCE=$(shell pwd)/$(BUILD)/log-custom-dynamic/log_lib.so$(STRIPPED)

#TODO PIC=TRUE? ld: warning: creating DT_TEXTREL in a PIE？进一步探索

thread.case:
	#make $(MAKEFLAGS) thread.readobjbin SUBDIR=/thread-default-static STATIC=TRUE
	make $(MAKEFLAGS) thread.run SUBDIR=/thread-default-static BACK='&'
	ps -T -p `ps -af | grep -E "thread.bin" | grep -v "grep" | head -n 1 | awk '{printf "%s\n", $$2}'`
	top -H -n 1 -p `ps -af | grep -E "thread.bin" | grep -v "grep" | head -n 1 | awk '{printf "%s\n", $$2}'`

cases:
	make singleton.case
	make crtmini.case
	make lifecycle.case
	make empty.case
	make variable.case
	make log.case
