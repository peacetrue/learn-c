#include <stdio.h>
#include <string.h>

 void vulnerableFunction(char* input) {
    char buffer[10];
    strcpy(buffer, input);
    printf("Buffer content: %s\n", buffer);
}

int main() {
    char input[20];
    printf("Enter a string: ");
    scanf("%s", input);
    vulnerableFunction(input);
    return 0;
}
