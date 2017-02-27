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


char getWinner(struct Field fields[3][3], int yCoord, int xCoord) {
    char currentValue= fields[yCoord][xCoord].contains;
    int victoryReq = 3;
    int victoryCount;
    for(int i = -1; i < 2; i++){
        for(int j = -1; j < 2; j++){
            if(!(i == 0 && j == 0)){
                victoryCount = 1;
                for(int k = 1; k < victoryReq; k++){
                    int currentColumn = xCoord + k * i;
                    if (currentColumn < 3 && currentColumn >= 0){
                        int currentRow = yCoord + k * j;
                        if ( currentRow < 3 && currentRow >=0){
                          if (fields[currentRow][currentColumn].contains == currentValue) {
                              victoryCount++;
                          } else{ break;}
                        }
                    }
                }
                for(int l = 1; l < victoryReq; l++){
                    int currentColumn = xCoord - l * i;
                    if (currentColumn < 3 && currentColumn >= 0){
                        int currentRow = yCoord - l * j;
                        if (currentRow < 3 && currentRow >= 0){
                            if (fields[currentRow][currentColumn].contains == currentValue) {
                                victoryCount++;
                            } else{ break;}
                        }
                    }
                }
                if (victoryCount == victoryReq){
                    return currentValue;
                }
            }
        }
    }
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

void resetMatrix(struct Field fields[3][3]){
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            struct Field currentField = {
                    .coordinates = {j, i},
                    .played = false,
                    .contains = NULL,
                    .draw = "[ ]"
            };
            fields[i][j] = currentField;
        }
    }
}


int * getAiDecision(struct Field fields[3][3]){
    int *r = malloc(2);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if(fields[i][j].played == false){
                r[0] = i;
                r[1] = j;
                return r;
            }
        }
    }
    return NULL;
}

int main(void) {

    //setting up 2D struct array of unplayed Fields
    struct Field fields[3][3];
    resetMatrix(fields);

    //setting up game
    int round = 1;
    char winner = NULL;
    int inputX, inputY;
    bool xTurn = true, aiTurn = false, aiPlays = false;
    int scanned = 0;
    char yesNo;

    printf("Do you want an AI opponent? (Y/N)\n");
    scanned = scanf("%c", &yesNo);
    while(scanned != 1 || (yesNo != 'n' && yesNo != 'N' && yesNo != 'y' && yesNo != 'Y')){
        printf("Please enter 'y' or 'n'.\n");
        fflush(stdin);
        scanned = scanf("%c", &yesNo);
    }
    if(yesNo == 'y' || yesNo == 'Y'){
        aiPlays = true;
    }

    //game loop
    while (round < 10) {

        //whose turn
        xTurn = !xTurn;
        if(aiPlays) aiTurn = !aiTurn;

        if(!aiTurn) {
            //message based on turn
            if (xTurn) {
                printf("X's Turn. Please enter coordinates from '0,0' to '2,2' as 'x,y' ;\n");
            } else {
                printf("O's Turn. Please enter coordinates from '0,0' to '2,2' as 'x,y' ;\n");
            }

            //get Coordinates
            scanned = scanf("  %d,  %d", &inputX, &inputY);
            while (scanned != 2 || ((inputX > 2) || (inputX < 0) || (inputY > 2) || (inputY < 0))) {
                printf("Error. Please enter valid coordinates\n");
                fflush(stdin);
                scanned = scanf("%d,%d", &inputX, &inputY);
            }

            //update matrix based on turn and coordinates
            if (fields[inputY][inputX].played) {
                printf("Field already played. Choose an empty one.\n");
                continue;
            }
        } else {
            int *coord = getAiDecision(fields);
            inputX = coord[1];
            inputY = coord[0];
        }
        fields[inputY][inputX].played = true;
        if(xTurn){
            fields[inputY][inputX].contains = 'x';
            strncpy(fields[inputY][inputX].draw, "[X]", 3);
        } else {
            fields[inputY][inputX].contains = 'o';
            strncpy(fields[inputY][inputX].draw, "[O]", 3);
        }

        drawMatrix(fields);
        printf("\n");

        //determine if somebody won
        winner = getWinner(fields, inputY, inputX);
        if (winner || round == 9) {
            fflush(stdin);
            if(round == 9 && !winner){
                printf("Draw!\n");
            }
            if(winner){
                printf("%c is the winner!\n", winner);
            }
            printf("Play again? (Y/N)?\n");
            char input;
            int numScanned = scanf("%c", &input);
            if(numScanned == 1 && (input == 'Y' || input == 'y')){
                main();
            } else return 0;

        }
        round++;
    }


}