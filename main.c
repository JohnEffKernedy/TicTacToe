#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mem.h>

//every Field is addressable as a struct and contains its status
struct Field {
    int coordinates[2];
    bool played;
    char contains;
    char draw[4];

};


char getWinner(struct Field fields[3][3]) {
    //return 'x' or 'o' if there's a winner

    return NULL;
}

void drawMatrix(struct Field fields[3][3]){
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%s", fields[i][j].draw);
        }
        printf("\n");
    }
}

int main(void) {

    //setting up 2D struct array of unplayed Fields
    struct Field fields[3][3];
    int fieldCounter = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            struct Field currentField = {
                    .coordinates = {j, i},
                    .played = false,
                    .contains = NULL,
                    .draw = "[ ]"
            };
            fields[i][j] = currentField;
            fieldCounter++;
        }
    }


    //setting up game
    int round = 1;
    char winner = NULL;
    int inputX;
    int inputY;
    bool xTurn = true;


    //game loop
    while (round < 10) {

        //whose turn
        if(round % 2 != 0){
            xTurn = true;
        } else xTurn = false;

        //message based on turn
        if(xTurn){
            printf("X's Turn. Please enter coordinates as 'x,y';\n");
        }   else {
            printf("O's Turn. Please enter coordinates as 'x,y';\n");
        }

        //get Coordinates
        scanf("%d,%d", &inputX, &inputY);

        //update matrix based on turn and coordinates
        if(xTurn){
            fields[inputY][inputX].contains = 'x';
            strncpy(fields[inputY][inputX].draw, "[X]", 3);
        } else {
            fields[inputY][inputX].contains = 'o';
            strncpy(fields[inputY][inputX].draw, "[O]", 3);
        }

        drawMatrix(fields);

        //determine if somebody won
        winner = getWinner(fields);
        if (winner) {
            printf("%c is the winner!", winner);
            return 0;
        }
        round++;
    }
    printf("Draw!");

}