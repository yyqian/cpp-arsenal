#include <stdio.h>

int little_endian(){
    int x = 1;
    return *(char*)&x;
}
int big_endian(){
    return !little_endian();
}

int main(){
    if(big_endian())
        printf("big_endian\n");
    if(little_endian())
        printf("little_endian\n");
    return 0;
}