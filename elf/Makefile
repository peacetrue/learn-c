include common.mk
#主线：构建可执行文件，分析文件内容。
include build-gnu.mk    #构建时使用 gnu 提供的类库，包括：crt1.o、libc.a 等
include build-mini.mk   #构建时使用 个人 提供的类库，包括：crtmini.c、libmini.c。gnu 提供的类库内容庞大不便于分析，个人类库很小。
include build-case.mk   #构建出分析时需要的针对性案例，基于个人类库 构建以便分析。
#支线：
include gnu.mk   		#看看 gnu 类库的内容
include command.mk		#测试可能的命令组合方式
include other.mk  		#其他
