# Makefile 内部逻辑调试信息
# 此文件需要放在 Makefile 末尾，需要打印的变量信息添加到 debug.vars 中
debug.vars+=CC CFLAGS CXX CXXFLAGS
#错误示例，进入 shell 执行阶段，无法获取 Makefile 中的变量
debug.echo.error:
	@for var in $(debug.vars) ; do \
	    echo "$$var: $($$var)"; \
	done

debug.echo.var.%:; @echo "$*: $($*)"
debug.echo: $(addprefix debug.echo.var.,$(debug.vars));
