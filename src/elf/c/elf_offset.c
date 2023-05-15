/** 用于计算 ELF 文件内各个段的偏移。 */

#include <stdio.h>

int main() {
    int start_of_program_headers = 64;
    int number_of_program_headers = 13;
    int size_of_program_headers = 56;
    int total_size_of_program_headers = number_of_program_headers * size_of_program_headers;
    printf("total_size_of_program_headers: %i\n", total_size_of_program_headers);
    printf("end_of_program_headers: 0x%x\n", start_of_program_headers + total_size_of_program_headers);

    int start_of_section_headers = 118;
    int number_of_section_headers = 64;
    int size_of_section_headers = 7;
    int total_size_of_section_headers = number_of_section_headers * size_of_section_headers;
    printf("start_of_section_headers: 0x%x\n", start_of_section_headers);
    printf("total_size_of_section_headers: %i\n", total_size_of_section_headers);
    printf("end_of_section_headers: 0x%x\n", start_of_section_headers + total_size_of_section_headers);

    printf("temp: 0x%x\n", 0x0000000000000086 + 0x000000000000002c);
}
