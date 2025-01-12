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
#define INF 1000000

#define SGN(c) (1 - 2*(strlen(c->content)%2))
#define GET(array, i, j) (array[(i)*WIDTH + j])
#define ADDABLE(c, column) (c->heights[column] > 0)

typedef struct {char* content; int* heights;} connect4;

connect4* initialize_connect4(){
    connect4* c = (connect4*)malloc(sizeof(connect4));
    c->content = (char*)malloc(HEIGHT*WIDTH*sizeof(char));
    c->content[0] = '\0';
    c->heights = (int*)malloc(WIDTH*sizeof(int));
    for(int i = 0; i<WIDTH; i++){
        c->heights[i] = HEIGHT;
    }
    return c;
}

void free_connect4(connect4* c){
    free(c->content);
    free(c->heights);
    free(c);
}

connect4* copy(connect4* c) {
    connect4* copy = (connect4*)malloc(sizeof(connect4));
    int content_length = strlen(c->content);
    copy->content = (char*)malloc((content_length + 1) * sizeof(char));
    strcpy(copy->content, c->content);
    copy->heights = (int*)malloc(WIDTH * sizeof(int));
    for (int i = 0; i < WIDTH; i++) {
        copy->heights[i] = c->heights[i];
    }
    return copy;
}

bool can_play(connect4* c, int column){
    return c->heights[column] > 0;
}

int play(connect4* c, int column){
    char column_char = column + '0';
    if(!can_play(c, column)){
        fprintf(stderr, "Trying to add to a full column.\n");
        return EXIT_FAILURE;
    }
    c->heights[column] --;
    int length = strlen(c->content);
    c->content[length] = column_char;
    c->content[length + 1] = '\0';
    return EXIT_SUCCESS;
}

int* get_grid(connect4* c){
    int* grid = (int*)calloc(WIDTH*HEIGHT, sizeof(int));
    for (int i = 0; c->content[i] != '\0'; i++) {
        int column = c->content[i] - '0';
        for (int row = HEIGHT - 1; row >= 0; row--) {
            if (GET(grid, row, column) == 0) {
                GET(grid, row, column) = (i % 2 == 0) ? 1 : 2;
                break;
            }
        }
    }
    return grid;
}

void display_grid(connect4* c){
    int* grid = get_grid(c);
    fprintf(stdout,"\n");
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            switch(GET(grid, row, col)){
                case EMPTY: fprintf(stdout,"🔘"); break;
                case RED: fprintf(stdout,"🔴"); break;
                case YELLOW: fprintf(stdout,"🟡"); break;
                default: fprintf(stdout,"?"); break;
            }
        }
        fprintf(stdout,"\n");
    }
}

connect4** get_children(connect4* c, int* size){
    *size = 0;
    connect4** children = (connect4**)malloc(WIDTH*sizeof(connect4*));
    for(int i = 0; i < WIDTH; i++){
        if(can_play(c, i)){
            children[*size] = copy(c);
            play(children[*size], i);
            *size+=1;
        }
    }
    return children;
}

int is_winning(connect4* c){
    int* grid = get_grid(c);
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH - 3; j++){
            int color = GET(grid, i, j);
            if(color && GET(grid, i, j+1) == color && GET(grid, i, j+2) == color && GET(grid, i, j+3) == color){
                return color;
            }
        }
    }
    for(int i = 0; i < HEIGHT - 3; i++){
        for(int j = 0; j < WIDTH; j++){
            int color = GET(grid, i, j);
            if(color && GET(grid, i+1, j) == color && GET(grid, i+2, j) == color && GET(grid, i+3, j) == color){
                return color;
            }
        }
    }
    for(int i = 0; i < HEIGHT - 3; i++){
        for(int j = 0; j < WIDTH - 3; j++){
            int color = GET(grid, i, j);
            if(color && GET(grid, i+1, j+1) == color && GET(grid, i+2, j+2) == color && GET(grid, i+3, j+3) == color){
                return color;
            }
        }
    }
    return 0;
}

int evaluate_grid(connect4* c) {
    int* grid = get_grid(c);
    int score = 0;
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH - 3; col++) {
            int count_red = 0;
            int count_yellow = 0;
            for (int k = 0; k < 4; k++) {
                if (GET(grid, row, col + k) == RED) count_red++;
                else if (GET(grid, row, col + k) == YELLOW) count_yellow++;
            }
            if (count_red > 0 && count_yellow == 0) score += count_red * count_red * count_red;
            if (count_yellow > 0 && count_red == 0) score -= count_yellow * count_yellow * count_yellow;
        }
    }
    for (int col = 0; col < WIDTH; col++) {
        for (int row = 0; row < HEIGHT - 3; row++) {
            int count_red = 0, count_yellow = 0;
            for (int k = 0; k < 4; k++) {
                if (GET(grid, row + k, col) == RED) count_red++;
                else if (GET(grid, row + k, col) == YELLOW) count_yellow++;
            }
            if (count_red > 0 && count_yellow == 0) score += count_red * count_red * count_red;
            if (count_yellow > 0 && count_red == 0) score -= count_yellow * count_yellow * count_yellow;
        }
    }
    for (int row = 0; row < HEIGHT - 3; row++) {
        for (int col = 0; col < WIDTH - 3; col++) {
            int count_red = 0, count_yellow = 0;
            for (int k = 0; k < 4; k++) {
                if (GET(grid, row + k, col + k) == RED) count_red++;
                else if (GET(grid, row + k, col + k) == YELLOW) count_yellow++;
            }
            if (count_red > 0 && count_yellow == 0) score += count_red * count_red * count_red;
            if (count_yellow > 0 && count_red == 0) score -= count_yellow * count_yellow * count_yellow;
        }
    }

    for (int row = 0; row < HEIGHT - 3; row++) {
        for (int col = 3; col < WIDTH; col++) {
            int count_red = 0, count_yellow = 0;
            for (int k = 0; k < 4; k++) {
                if (GET(grid, row + k, col - k) == RED) count_red++;
                else if (GET(grid, row + k, col - k) == YELLOW) count_yellow++;
            }
            if (count_red > 0 && count_yellow == 0) score += count_red * count_red * count_red;
            if (count_yellow > 0 && count_red == 0) score -= count_yellow * count_yellow * count_yellow;
        }
    }

    free(grid);
    return score;
}

int best_move(connect4* c){
    int best = -INF;
    int move = -1;
    for(int i = 0; i < WIDTH; i++){
        if(can_play(c, i)){
            connect4* child = copy(c);
            play(child, i);
            int eval = evaluate_grid(child);
            if(SGN(c)*eval > best){
                best = eval;
                move = i;
            }
            free_connect4(child);
        }
    }
    return move;
}

/*
int minimax(connect4* c, int depth, int alpha, int beta, int maximizingPlayer) {
    int winner = is_winning(c);
    if (winner) {
        return (winner == maximizingPlayer) ? INF : -INF;
    }

    if (depth == 0) {
        return evaluate_grid(c, maximizingPlayer);
    }

    int size;
    connect4** children = get_children(c, &size);

    int value = (maximizingPlayer) ? -INF : INF;

    for (int i = 0; i < size; i++) {
        int child_value = minimax(children[i], depth - 1, alpha, beta, !maximizingPlayer);

        if (maximizingPlayer) {
            value = (child_value > value) ? child_value : value;
            alpha = (alpha > value) ? alpha : value;
        } else {
            value = (child_value < value) ? child_value : value;
            beta = (beta < value) ? beta : value;
        }

        if (beta <= alpha) {
            break;
        }

        free(children[i]->content);
        free(children[i]->heights);
        free(children[i]);
    }
    free(children);

    return value;
}*/


void game(){
    srand(time(NULL));
    connect4* c = initialize_connect4();
    display_grid(c);
    int column;
    while(true){
        printf("\n\033[4mChoose the column too place your stone\033[0m : ");
        scanf("%d",&column);
        system("cls");
        play(c, column);
        if(is_winning(c)){
            display_grid(c);
            printf("You won !");
            break;
        }
        play(c, rand()%WIDTH);
        display_grid(c);
        if(is_winning(c)){
            display_grid(c);
            printf("You lost !");
            break;
        }
    }
}

int main(){
    system("chcp 65001"); 
    system("cls");
    connect4* c = initialize_connect4();
    display_grid(c);
    int column;
    while(true){
        printf("\n\033[4mChoose the column too place your stone\033[0m : ");
        scanf("%d",&column);
        system("cls");
        play(c, column);
        play(c, best_move(c));
        display_grid(c);
    }
    return EXIT_SUCCESS;
}

/*
int add(connect4* c, int color, int column){
    if(c->heights[column]){
        c->heights[column] --;
        SET(c, c->heights[column], column, color);
        return EXIT_SUCCESS;
    }
    fprintf(stdout,"Trying to add a color to an already full column !\n");
    return EXIT_FAILURE;
}

bool addable(connect4* c, int column){
    return (c->heights[column] > 0);
}

connect4* copy_game(connect4* c){
    connect4* copy = (connect4*)malloc(sizeof(connect4));
    copy->content = (int*)malloc(WIDTH*HEIGHT*sizeof(int));
    copy->heights = (int*)malloc(WIDTH*sizeof(int));
    for(int i = 0; i < WIDTH; i++){
        copy->heights[i] = c->heights[i];
        for(int j = 0; j < HEIGHT; j++){
            copy->content[i] = c->content[i];
        }
    }
    return c;
}

connect4* apply_move(connect4* c, int column, int color){
    connect4* copy = copy_game(c);
    add(copy, color, column);
    return copy;
}

connect4** get_children(connect4* c, int* nb_children, int color){
    connect4** children = (connect4*)malloc(WIDTH*sizeof(connect4*));
    *nb_children = 0;
    for(int i = 0; i < WIDTH; i++){
        if(addable(c, i)){
            children[*nb_children] = apply_move(c, i, color);
            nb_children ++;
        }
    }
}

int heuristic(connect4* c){
    return 1;
}

int minimax(connect4* c, int depth, bool maximizing_player){
    int value;
    if (depth == 0){
        return heuristic(c);
    }
    if(maximizing_player){
        value = - __INT_MAX__;
        int nb_children = 0;
        connect4** children = get_children(c, &nb_children, RED);
        for(int child = 0; child < nb_children; child++){
            int m = minimax(children[child], depth - 1, false);
            if(value < m){value = m;}
        }
    }
    else{
        value = __INT_MAX__;
        int nb_children = 0;
        connect4** children = get_children(c, &nb_children, YELLOW);
        for(int child = 0; child < nb_children; child++){
            int m = minimax(children[child], depth - 1, true);
            if(value > m){value = m;}
        }
    }  
    return value;
}
*/