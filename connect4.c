#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define WIDTH 7
#define HEIGHT 6
#define EMPTY 0
#define RED 1
#define YELLOW 2
#define MASK_49_BITS 0x1FFFFFFFFFFFF
#define LEGAL_UINT49(bb) (((bb) & MASK_49_BITS) == (bb))
#define GET(bb, i, j) ((int)((bb >> ((6-i) + j*7)) & 1))

#define TEST(condition) do { \
    if (!(condition)) { \
        fprintf(stderr, "Test failed (%s) - Line %d\n", #condition, __LINE__); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

typedef unsigned long long bitboard;
typedef struct {bitboard non_empty; bitboard stones;} board;

void display_bitboard(bitboard bb){
    TEST(LEGAL_UINT49(bb));
    for(int i = 0; i < 7; i++){
        printf("|");
        for(int j = 0; j < 7; j++){
            printf("%d|", GET(bb, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void display_board(board* b){
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 7; j++){
            if(!GET(b->non_empty, i, j)){
                printf("🔘");
            }
            else if(GET(b->stones, i, j)){
                printf("🟡");
            }
            else{
                printf("🔴");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]){
    system("chcp 65001");
    system("cls");
    board* b = (board*)malloc(sizeof(board));
    b->non_empty = 0;
    b->stones = 0;
    display_bitboard(b->non_empty);
    display_bitboard(b->stones);
    display_board(b);
    return EXIT_SUCCESS;
}

/* Compilation command :
    - cd Programmation/Connect-4
    gcc -Wall connect4.c -o connect4.exe && connect4.exe
    gcc -Wall connect4.c -o connect4.exe && connect4.exe 2>nul 
*/