#include <stdio.h>
#include <stdbool.h>
#include <mem.h>
#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))


//every Field is addressable as a struct and contains its status
struct Field {
    int coordinates[2];
    char contains;
    char draw[4];
};


bool isWinningState(struct Field fields[3][3], int yCoord, int xCoord, char playerToCheck) {
    if (fields[yCoord][xCoord].contains != playerToCheck) {
        return false;
    }
    int victoryReq = 3;
    int victoryCount;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (!(i == 0 && j == 0)) {
                victoryCount = 1;
                for (int k = 1; k < victoryReq; k++) {
                    int currentColumn = xCoord + k * i;
                    if (currentColumn < 3 && currentColumn >= 0) {
                        int currentRow = yCoord + k * j;
                        if (currentRow < 3 && currentRow >= 0) {
                            if (fields[currentRow][currentColumn].contains == playerToCheck) {
                                victoryCount++;
                            } else { break; }
                        }
                    }
                }
                for (int l = 1; l < victoryReq; l++) {
                    int currentColumn = xCoord - l * i;
                    if (currentColumn < 3 && currentColumn >= 0) {
                        int currentRow = yCoord - l * j;
                        if (currentRow < 3 && currentRow >= 0) {
                            if (fields[currentRow][currentColumn].contains == playerToCheck) {
                                victoryCount++;
                            } else { break; }
                        }
                    }
                }
                if (victoryCount == victoryReq) {
                    return true;
                }
            }
        }
    }
    return false;
}

void drawMatrix(struct Field fields[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%s", fields[i][j].draw);
        }
        printf("\n");
    }
}

void resetMatrix(struct Field fields[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            struct Field currentField = {
                    .coordinates = {j, i},
                    .contains = NULL,
                    .draw = "[ ]"
            };
            fields[i][j] = currentField;
        }
    }
}

int rate(struct Field fields[3][3], int depth, char currPlayer, char oppPlayer) {
    bool winningState = false;
    bool losingState = false;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            winningState = isWinningState(fields, i, j, currPlayer);
            losingState = isWinningState(fields, i, j, oppPlayer);
            if (losingState) {
                return depth - 10;
            } else if (winningState) return 10 - depth;
        }
    }
    return 0;
}

bool boardIsFull(struct Field fields[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!fields[i][j].contains) {
                return false;
            }
        }
    }
    return true;
}

int miniMax(struct Field fields[3][3], int depth, char currPlayer, char oppPlayer, bool isMaximiserMove) {
    int rating = 0;
    rating = rate(fields, depth, currPlayer, oppPlayer);
    if (rating != 0) {
        return rating;
    } else if (boardIsFull(fields)) {
        return 0;
    }

    if (isMaximiserMove) {
        int best = -1000;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (!fields[i][j].contains) {
                    fields[i][j].contains = currPlayer;
                    best = MAX(best, miniMax(fields, depth + 1, currPlayer, oppPlayer, !isMaximiserMove));
                    fields[i][j].contains = NULL;
                }
            }
        }
        return best;

    } else {
        int best = 1000;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (!fields[i][j].contains) {
                    fields[i][j].contains = oppPlayer;
                    best = MIN(best, miniMax(fields, depth + 1, currPlayer, oppPlayer, !isMaximiserMove));
                    fields[i][j].contains = NULL;
                }
            }
        }
        return best;

    }
}

int *getAiDecision(struct Field fields[3][3], char currPlayer, char oppPlayer) {
    int *r = malloc(2);
    int bestRating = -1000;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!fields[i][j].contains) {
                fields[i][j].contains = currPlayer;
                int currentRating = miniMax(fields, 0, currPlayer, oppPlayer, false);
                fields[i][j].contains = NULL;
                if (currentRating > bestRating) {
                    r[0] = i;
                    r[1] = j;
                    bestRating = currentRating;
                }
            }
        }
    }
    printf("The best rated move has %d points\n", bestRating);
    return r;
}

int main(void) {
    //setting up 2D struct array of unplayed Fields
    struct Field fields[3][3];
    resetMatrix(fields);

    //setting up game
    int round = 1;
    bool winningState = false;
    int inputX, inputY;
    bool xTurn = true, aiTurn = false, aiPlays = false;
    int scanned = 0;
    char yesNo;
    char currentPlayer;
    char oppPlayer;

    printf("Do you want an AI opponent? (Y/N)\n");
    scanned = scanf("%c", &yesNo);
    while (scanned != 1 || (yesNo != 'n' && yesNo != 'N' && yesNo != 'y' && yesNo != 'Y')) {
        printf("Please enter 'y' or 'n'.\n");
        fflush(stdin);
        scanned = scanf("%c", &yesNo);
    }
    if (yesNo == 'y' || yesNo == 'Y') {
        aiPlays = true;
    }

    if (aiPlays) {
        printf("Do you want to begin? (Y/N)\n");
        scanned = scanf("%c", &yesNo);
        while (scanned != 1 || (yesNo != 'n' && yesNo != 'N' && yesNo != 'y' && yesNo != 'Y')) {
            printf("Please enter 'y' or 'n'.\n");
            fflush(stdin);
            scanned = scanf("%c", &yesNo);
        }
        if (yesNo == 'y' || yesNo == 'Y') {
            aiTurn = false;
        } else aiTurn = true;
    }


    currentPlayer = 'x';
    oppPlayer = 'o';

    if (!aiTurn) drawMatrix(fields);
    //game loop
    while (round < 10) {

        if (!aiTurn) {
            //message based on turn
            if (currentPlayer == 'x' && !aiPlays) {
                printf("X's Turn. Please enter coordinates from '0,0' to '2,2' as 'x,y'.\n");
            } else if (!aiPlays) {
                printf("O's Turn. Please enter coordinates from '0,0' to '2,2' as 'x,y'.\n");
            } else {
                printf("Player's turn (%c). Please enter coordinates from '0,0' to '2,2' as 'x,y'.\n", currentPlayer);
            }

            //get Coordinates
            scanned = scanf("  %d,  %d", &inputX, &inputY);
            while (scanned != 2 || ((inputX > 2) || (inputX < 0) || (inputY > 2) || (inputY < 0))) {
                printf("Error. Please enter valid coordinates.\n");
                fflush(stdin);
                scanned = scanf("%d,%d", &inputX, &inputY);
            }

            //update matrix based on turn and coordinates
            if (fields[inputY][inputX].contains) {
                printf("Field already played, please choose an empty one.\n");
                continue;
            }
        } else {
            if (round == 1) {
                int numField = rand() % 10;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (numField == 0) {
                            inputX = i;
                            inputY = j;
                        }
                        numField -= 1;
                    }
                }

            } else {
                printf("The AI plays: \n");
                int *coord = getAiDecision(fields, currentPlayer, oppPlayer);
                inputX = coord[1];
                inputY = coord[0];
            }

        }
        if (currentPlayer == 'x') {
            fields[inputY][inputX].contains = 'x';
            strncpy(fields[inputY][inputX].draw, "[X]", 3);
        } else {
            fields[inputY][inputX].contains = 'o';
            strncpy(fields[inputY][inputX].draw, "[O]", 3);
        }

        drawMatrix(fields);
        printf("\n");

        //determine if somebody won
        winningState = isWinningState(fields, inputY, inputX, currentPlayer);
        if (winningState || round == 9) {
            fflush(stdin);
            if (round == 9 && !winningState) {
                printf("Draw!\n");
            }
            if(winningState){
                if (!aiPlays) {
                    printf("%c is the winner!\n", currentPlayer);
                } else if(!aiTurn){
                    printf("Player (%c) is the winner!\n", currentPlayer);
                } else printf("AI is the winner!\n");

            }
            printf("Play again? (Y/N)?\n");
            char input;
            int numScanned = scanf("%c", &input);
            if (numScanned == 1 && (input == 'Y' || input == 'y')) {
                main();
            } else return 0;

        }
        if (currentPlayer == 'x') {
            currentPlayer = 'o';
            oppPlayer = 'x';
        } else {
            currentPlayer = 'x';
            oppPlayer = 'o';
        }
        if (aiPlays) aiTurn = !aiTurn;
        round++;
    }
}