#include <stdio.h>
#include <stdbool.h>
#include <mem.h>
#include <stdlib.h>
#include <time.h>

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

int miniMax(struct Field fields[3][3], int depth, char currPlayer, char oppPlayer, bool isMaximiserMove, int maxDepth, int alpha, int beta) {
    int rating = 0;
    rating = rate(fields, depth, currPlayer, oppPlayer);
    if (rating != 0 || depth == maxDepth) {
        return rating;
    } else if (boardIsFull(fields)) {
        return 0;
    }

    if (isMaximiserMove) {
        int maxValue = alpha;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (!fields[i][j].contains) {
                    fields[i][j].contains = currPlayer;
                    int value = MAX(maxValue, miniMax(fields, depth + 1, currPlayer, oppPlayer, !isMaximiserMove,maxDepth, maxValue, beta));
                    fields[i][j].contains = NULL;
                    if (value > maxValue) {
                        maxValue = value;
                        if (maxValue >= beta)
                            break;
                    }
                }
            }
        }
        return maxValue;

    } else {
        int minValue = beta;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (!fields[i][j].contains) {
                    fields[i][j].contains = oppPlayer;
                    int value = MIN(minValue, miniMax(fields, depth + 1, currPlayer, oppPlayer, !isMaximiserMove,maxDepth, alpha, minValue));
                    fields[i][j].contains = NULL;
                    if (value < minValue) {
                        minValue = value;
                        if (minValue <= alpha)
                            break;
                    }
                }
            }
        }
        return minValue;

    }
}

int *getAiDecision(struct Field fields[3][3], char currPlayer, char oppPlayer, int complex) {
    int *r = malloc(2);
    int bestRating = -1000;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!fields[i][j].contains) {
                fields[i][j].contains = currPlayer;
                int currentRating = miniMax(fields, 0, currPlayer, oppPlayer, false, complex,-1000,1000);
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

int difficulty(int scanned){
    printf("Select difficulty (easy(e),medium(m),impossible(i))\n");
    char diff;
    scanned = scanf("%c", &diff);
    int maxDepth = 0;
    while (scanned != 1 || (diff != 'e' && diff != 'E' && diff != 'm' && diff != 'M' && diff != 'i' && diff != 'I')) {
        printf("Please enter 'e', 'm' or 'i'.\n");
        fflush(stdin);
        scanned = scanf("%c", &diff);
    }
    if (diff == 'e' || diff == 'E'){
        maxDepth = 1;
    } else if (diff == 'm' || diff == 'M'){
        maxDepth = 3;
    } else maxDepth = 10;
    return maxDepth;
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
    int maxDepth = 0;
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
        maxDepth = difficulty(scanned);
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
            if (round == 1 && maxDepth != 10) {
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
                double start = clock();
                int *coord = getAiDecision(fields, currentPlayer, oppPlayer, maxDepth);
                double end = clock();
                printf("%lf seconds passed\n", (end-start)/1000);
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