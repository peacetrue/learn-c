/* 自定义链接过程 */
ENTRY(peace)/*设置入口函数为 peace*/
SECTIONS
{
    . = 0x08048000 + SIZEOF_HEADERS;/*设置当前虚拟地址，tinytext 从文件头底部开始*/
    tinytext  : { *(.text) *(.data) *(.rodata) }/*合并 3 个节到 tinytext 段中*/
    /DISCARD/ : { *(.comment) }/*丢弃 comment 段*/
}
