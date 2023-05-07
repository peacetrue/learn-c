/** 空内容，作为分析的基类，可以和其他类做对比 */
int main() {
}

/*
# make empty-static.clean
# make empty.readobjbin SUBDIR=/empty-static STATIC=-static
# make empty.lldb SUBDIR=/empty-static
# make empty.bin.maps SUBDIR=/empty-static

# make empty-dynamic.clean
# make empty.readobjbin SUBDIR=/empty/dynamic
# make empty.lldb SUBDIR=/empty-dynamic
# make empty.bin.maps SUBDIR=/empty-dynamic
*/
