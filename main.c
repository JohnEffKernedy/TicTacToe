#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//every Field is addressable as a struct and contains its status
struct Field {
    int coordinates[2];
    bool played;
    char contains;
    char draw[4];

};


char getWinner(struct Field fields[3][3]){
    //return 'x' or 'o' if there's a winner

    return NULL;
}

int main(void) {

    //setting up 2D struct array of unplayed Fields
    struct Field fields[3][3];
    int fieldCounter = 0;
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            struct Field currentField = {
                    .coordinates = {i, j},
                    .played = false,
                    .contains = NULL,
                    .draw = "[ ]"
            };
            fields[i][j] = currentField;
            fieldCounter++;
        }
    }

    //draws unplayed matrix
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            printf("%s",fields[i][j].draw);
        }
        printf("\n");
    }

    //setting up game
    int round = 1;
    char winner = NULL;

    //game loop
    while(round < 10){
        //stuff happening
        winner = getWinner(fields);
        if(winner){
            printf("%c is the winner!", winner);
            return 0;
        }
        round++;
    }
    printf("Draw!");
}