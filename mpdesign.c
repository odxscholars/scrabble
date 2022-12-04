/*
Scrabble Game for CCPROG2
Name:
Section:
ID:
*/


#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<ctype.h>
#define BOARDHEIGHT 11;
#define BOARDWIDTH 11;


/*
This struct is used to store the information of a letter tile.

@var letter: the letter of the tile which is a character
@var value: the value or score of the letter which is an integer
@var owner: the owner of the tile which refers to a playerIndex
*/

struct letterTile{
    char letter;
    int value;
    int owner;
};


//typedef to avoid typing struct letterTile everytime
typedef struct letterTile letterTile;

/*
This struct is used to store the information of a board tile, it contains the letter tile itself.
@var isEmpty - a boolean value that indicates if the tile is empty or not
@var letter - a variable of the letterTile struct which contains the letter and its value
*/
struct boardTile{
    bool isEmpty;
    letterTile letter;
};

typedef struct boardTile boardTile;

/*
The board struct is used the struct that is made up of individual board tiles.
@var tiles - an 11 by 11  2d array of board tiles
@var boardWords - amount of words in the board
*/
struct board{
    boardTile tiles[11][11];
    int boardWords;
};

typedef struct board board;

/*
The player struct is used to store the information of a player.
@var name - a string that contains the name of the player
@var score - an integer that contains the score of the player
@var letters - an array of letter tiles that contains the letters of the player that are initially availble
@var recordIndex - the player's index in the records file.
@var wordCount - the amount of words the player has made.

*/

struct player{
    char name[30];
    int score;
    letterTile letters[7];
    int recordIndex;
    int wordCount;
};

typedef struct player player;


/*
The record struct is used to store the information of a player's record. It is either new or extracted from a file
@var name - a string that contains the name of the player
@var words - an array of strings that contains the words that the player has played
@var longestWord - a string that contains the longest word that the player has played
@var highestScore - an integer that contains the highest score that the player has achieved
@var gamesPlayed - an integer that contains the number of games that the player has played
@var averageScore - a float that contains the average score of the player

*/
struct record{
    char name[30];
    char words[15][30];
    char longestWord[15];
    int highestScore;
    int gamesPlayed;
    double averageScore;
};

typedef struct record record;

/*
The recordFile struct is used to store the different records that are available.
@var filePath - a string that contains the path of the file
@var records - an array of records that contains the records of the players
@var recordCount - the amount of records in the record file
*/
struct recordFile{
    char filePath[100];
    record records[10];
    int recordCount;
};

typedef struct recordFile recordFile;


/*
The dict struct is used to store the different words that are considered "valid"
@var words - an array of 100 strings that contains the words that are considered valid
@var wordCount - amount of words in word count;
*/
struct dict{
    char words[100][10];
    int wordCount;
};

typedef struct dict dict;


/*
The letterBank struct is used to store the different letter tiles that are available in the game. Also stores the amount of letters that can still be allotted to players.
@var letters - an array of letter tiles that contains the letters that are available in the game
@var letterCount - the amount of letters that are available in the game



*/
typedef struct letterBank{
    char letters[27];
    int letterCount[27];
} letterBank;


/*
The game struct is used to store the information of the game. It is initialized before a game even starts.
Values of the different struct variables are then initialized throughout the game.

@var gameBoard - a variable of the board struct that contains the board of the game
@var players - an array of players that contains the players of the game
@var dictionary - a variable of the dict struct that contains the dictionary of the game
@var records - a variable of the recordFile struct that contains the record file of the game
@var currentPlayer - an integer that points to the current player of the game
@var letterList - a variable of the letterBank struct that stores the different available letters in the game.
*/
typedef struct game{
    int numPlayers;
    board gameBoard;
    player players[4];
    dict dictionary;
    recordFile records;
    int currentPlayer;
    letterBank letterList;
} game;



void loadDictionary(dict *dictionary){
    /*
    This function loads the dictionary of the game.
    @param dictionary - a pointer to the dictionary of the game
    */
    dictionary->wordCount = 0;
    FILE *file = fopen("dict.txt", "r");
    int i = 0;
    while(fscanf(file, "%s", dictionary->words[i]) != EOF){
        i++;
    }
    dictionary->wordCount = i;
    fclose(file);
}


bool searchWordInDictionary(char * key, dict *dictionary){
    /*
    This function searches for a word in the dictionary.
    @param key - a string that contains the word to be searched
    @param dictionary - a pointer to the dictionary of the game
    @return - a boolean value that indicates if the word is found or not
    */

    char newKey[10];
    strcpy(newKey, "");
    for(int i = 0; i < strlen(key); i++){
        newKey[i] = tolower(key[i]);
    }
    newKey[strlen(key)] = '\0';
    for(int i = 0; i < dictionary->wordCount; i++){
        if(strcmp(newKey, dictionary->words[i]) == 0){
            return true;
        }
    }
    return false;
}



void initializeBoard(board *gameBoard){
    /*
    This function initializes the board of the game.
    @param gameBoard - a pointer to the board of the game
    */

    int i,j;
    for(i=0;i<11;i++){
        for(j=0;j<11 ;j++){
            gameBoard->tiles[i][j].isEmpty = true;
        }
    }
    gameBoard->boardWords =0;

}


int getScoreOfCharacter(char letter){
    /*
    This function returns the score of a letter.
    @param letter - a character that contains the letter
    @return - an integer that contains the score of the letter
    */

    char letters[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', '_'};
    int score[] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10,0};
    for(int i = 0; i < 27;i ++){
        if (letters[i] == toupper(letter)){
            printf("added %d score for %c\n", score[i], letter);
            return score[i];
        }
    }
    
    return -1;
}



void displayBoard(game * mainGame){
    /*
    This function displays the board of the game.
    @param mainGame - a pointer to the game
    */

    printf("   ");
    for(int i = 0; i < 10; i++){
        printf(" %d ", i);
    }
    printf(" 10");
    printf("\n");
    for(int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++){
            if (j == 0 && i != 10){
                printf("%d  ", i);
            }else if(j == 0 && i == 10){
                printf("%d ", i);
            }
            if(mainGame->gameBoard.tiles[i][j].isEmpty){
                printf(" - ");
            }
            else{
                printf(" %c ", mainGame->gameBoard.tiles[i][j].letter.letter);
            }
            if (j == 10){
                printf(" %d ", i);
            }
        }
        printf("\n");
    }
    printf("\n");

}
bool checkIfCoordinatesAreConnected(int yArr[], int xArr[], int size){
    /*
    This function checks if the coordinates of the tiles are connected.
    @param yArr - an array of integers that contains the y coordinates of the tiles
    @param xArr - an array of integers that contains the x coordinates of the tiles
    @param size - an integer that contains the size of the arrays
    @return - a boolean value that indicates if the coordinates are connected or not
    */
    for(int i = 0; i < size; i++){
        if (size > i + 1){
            //compute for distance between two points
            int xDistance = abs(xArr[i] - xArr[i+1]);
            int yDistance = abs(yArr[i] - yArr[i+1]);
            if (xDistance + yDistance > 1){
                
                return false;
            }

        }else if (size == 1){
            return false;
        }
        
    }
    
    return true;

}

char getRandomLetter(game * mainGame){
    /*
    This function returns a random letter from the letter bank.
    @param mainGame - a pointer to the game
    @return - a character that contains the random letter
    */

    int random = 0;
    char letter;
    do{
        random = rand() % 27;
        letter = mainGame->letterList.letters[random];
       
    }while(mainGame->letterList.letterCount[random] == 0);
    mainGame->letterList.letterCount[random]--;
    return letter;
}
void swapPlayerLetterWithNewOne(game * mainGame, char oldLetter){
    for(int i = 0; i < 7; i++){
        if(mainGame->players[mainGame->currentPlayer].letters[i].letter == oldLetter){
            mainGame->players[mainGame->currentPlayer].letters[i].letter = getRandomLetter(mainGame);
        }
    }
    
}
void addScorePlayer(char letter, game * mainGame){
    /*
    This function adds the score of a letter to the player's score.
    @param letter - a character that contains the letter
    @param mainGame - a pointer to the game
    */

    int score = getScoreOfCharacter(letter);
    mainGame->players[mainGame->currentPlayer].score += score;
}

void changeTurn(game * mainGame){
    
    if (mainGame->currentPlayer == mainGame->numPlayers - 1){
        
        mainGame->currentPlayer = 0;
    }else{
        printf("incremented player\n");
        mainGame->currentPlayer++;
    }
}



void setStringToEmpty(int n, char *string){
    for(int i = 0; i < n; i++){
        string[i] = '\0';
    }
}

bool checkIfWordIsInLetters(char * temp, game * mainGame){
    bool found = false;
    for(int i = 0; i < strlen(temp); i++){
       
        for(int j = 0; j < 7; j++){
            if(temp[i] == mainGame->players[mainGame->currentPlayer].letters[j].letter){
                found = true;
                j = 7;
            }
        }
        if(!found){
            printf("went here\n");
            return false;
        }
    }
    return found;
    
}

void choiceController(game * mainGame){
    /*
    This function controls the choices of the player.
    @param mainGame - a pointer to the game
    */
   
    char temp[15] = "\0";
    char temp1[20];
    for(int i = 0; i < 20; i++){
        temp1[i] = '\0';
    }
    strcpy(temp, "");
    strcpy(temp1, "\0");
    int choice;
    

        do{
            printf("[1] Add letters to board\n[2]Replace your letters\n");
            scanf("%d", &choice);

        }while(choice > 2 || choice < 1);
       
        if (choice == 1){
            int repeat = 1;
            int originX, originY;
            int direction;
            while (repeat == 1){
                
                int repeatDoWhile = 1;
                do{
                    repeatDoWhile = 1;
                    do{
                        printf("Letters to use(input is case sensitive, should not have spaces, should be consistent with order): ");
                        scanf("%s", temp);
                        if (checkIfWordIsInLetters(temp, mainGame) == false){
                            printf("You do not have all the letters in your word\n");
                        }
                    }while(checkIfWordIsInLetters(temp, mainGame) == false);
                    printf("X: ");
                    scanf("%d", &originY);
                    printf("Y: ");
                    scanf("%d", &originX);
                    printf("1. Down\n");
                    printf("2. Right\n");
                    printf("Direction: ");
                    scanf("%d", &direction);
                    int count = 0;
                    //count the wordCount of all players
                    for(int i = 0; i < mainGame->numPlayers; i++){
                        count += mainGame->players[i].wordCount;
                    }
                    if (count == 0 ){
                        if (direction == 1 && ((originX == 5) || (originX + strlen(temp) > 5 && originX < 5))){
                            repeatDoWhile = 0;
                        
                        }else if(direction == 2 && ((originY == 5) || (originY + strlen(temp) > 5 && originY < 5))){
                            repeatDoWhile = 0;
                        }else{
                            printf("As your first move, first inputs must be in the middle of the board\n");
                        }
                    }else{
                        repeatDoWhile = 0;
                    }

                }while(repeatDoWhile == 1);
                
                if ((originY + strlen(temp) > 11 && direction == 2) || originY < 0 || originY > 10){
                    printf("Invalid move, out of bounds\n");
                    
                }else if((originX + strlen(temp) > 11 && direction == 1) || originX < 0 || originX > 10){
                    printf("Invalid move, out of bounds\n");
                    
                }else{
                    int count = 0;
                    int exit = 1;
                    int score = 0;
                    if (direction == 1){
                        for(int i = 0; i < strlen(temp);){
                            if (originX + count <= 10){
                                if (mainGame->gameBoard.tiles[originX+count][originY].isEmpty == false){
                                    temp1[count] = mainGame->gameBoard.tiles[originX+count][originY].letter.letter;
                                    score += getScoreOfCharacter(temp1[count]);
                                    count++;
                                    
                                }else if(mainGame->gameBoard.tiles[originX+count][originY].isEmpty == true){
                                    temp1[count] = temp[i];
                                    score += getScoreOfCharacter(temp1[count]);
                                    i++;
                                    count++;
                                    
                                }
                                
                            }else{
                                exit = 0;
                                i = strlen(temp);
                            }
                        }
                        if (exit == 1){
                            //check if word is valid after
                            
                            //if valid
                            if (searchWordInDictionary(temp1, &(mainGame->dictionary)) == true){
                                count = 0;
                                for(int i = 0; i < strlen(temp);){
                                    if (mainGame->gameBoard.tiles[originX+count][originY].isEmpty == false){
                                        count++;
                                        
                                    }else if(mainGame->gameBoard.tiles[originX+count][originY].isEmpty == true){
                                        
                                        mainGame->gameBoard.tiles[originX+count][originY].letter.letter = temp[i];
                                        mainGame->gameBoard.tiles[originX+count][originY].isEmpty = false;
                                        swapPlayerLetterWithNewOne(mainGame, temp[i]);
                                        mainGame->gameBoard.tiles[originX+count][originY].letter.owner = mainGame->currentPlayer;
                                        mainGame->gameBoard.tiles[originX+count][originY].letter.value = getScoreOfCharacter(temp[i]);
                                        i++;
                                        count++;
                                        
                                    }
                                }
                                mainGame->players[mainGame->currentPlayer].wordCount++;
                                mainGame->players[mainGame->currentPlayer].score += score;
                                int playerIndex = mainGame->players[mainGame->currentPlayer].recordIndex;
                                strcpy(mainGame->records.records[playerIndex].words[mainGame->players[mainGame->currentPlayer].wordCount], temp1);
                                changeTurn(mainGame);
                                repeat = 0;
                            }else{
                                printf("Word not found. Go again\n");
                            }
                        }else{
                            printf("Invalid move, went out of bounds\n");
                        }
                        

                        //if not valid, continue
                    }else if(direction == 2){
                        for(int i = 0; i < strlen(temp);){
                            if (originY + count <= 10){
                                if (mainGame->gameBoard.tiles[originX][originY+count].isEmpty == false){
                                    temp1[count] = mainGame->gameBoard.tiles[originX][originY+count].letter.letter;
                                    score += getScoreOfCharacter(temp1[count]);
                                    count++;
                                    
                                }else if(mainGame->gameBoard.tiles[originX][originY+count].isEmpty == true){
                                    temp1[count] = temp[i];
                                    score += getScoreOfCharacter(temp1[count]);
                                    i++;
                                    count++;
                                    
                                }
                                
                            }else{
                                exit = 0;
                            }
                        }
                        if (exit == 1){
                           
                            //if valid
                            if(searchWordInDictionary(temp1, &(mainGame->dictionary)) == true){
                                count = 0;
                                for(int i = 0; i < strlen(temp);){
                                    if (mainGame->gameBoard.tiles[originX][originY+count].isEmpty == false){
                                        count++;

                                        
                                    }else if(mainGame->gameBoard.tiles[originX][originY+count].isEmpty == true){
                                    
                                        mainGame->gameBoard.tiles[originX][originY+count].letter.letter = temp[i];
                                        swapPlayerLetterWithNewOne(mainGame,temp[i]);
                                        mainGame->gameBoard.tiles[originX][originY+count].isEmpty = false;
                                        mainGame->gameBoard.tiles[originX][originY+count].letter.owner = mainGame->currentPlayer;
                                        mainGame->gameBoard.tiles[originX][originY+count].letter.value = getScoreOfCharacter(temp[i]);
                                        i++;
                                        count++;
                                        
                                    }
                                    
                                }
                                int playerIndex = mainGame->players[mainGame->currentPlayer].recordIndex;
                                strcpy(mainGame->records.records[playerIndex].words[mainGame->players[mainGame->currentPlayer].wordCount], temp1);

                                
                                mainGame->players[mainGame->currentPlayer].wordCount++;
                                mainGame->players[mainGame->currentPlayer].score += score;
                                changeTurn(mainGame);
                                repeat = 0;
                            }else{
                                printf("Word not found. Go again\n");
                            }
                        }
                    
                    }
                    



                }

                //case 1: letters are in empty space and are not connected
                //case 2: letters cross another letter
                //case 3: letters are in empty space and are connected

            }
            
        }else if (choice == 2){
            printf("Replace letters\n");
            int continueAsk = 1;
            while(continueAsk == 1){
                int index = 0;
                printf("Which letter do you want to replace? (input index 0 - 6) -1 for exit: ");
                scanf("%d", &index);
                if (index != -1){
                    char letter = mainGame->players[mainGame->currentPlayer].letters[index].letter;
                    printf("old letter: %c\n", letter);
                    for(int i = 0; i < 27; i++){
                        if(letter == mainGame->letterList.letters[i]){
                            mainGame->letterList.letterCount[i]++;
                            i = 27;
                        }
                        
                    }
                    mainGame->players[mainGame->currentPlayer].letters[index].letter = getRandomLetter(mainGame);
                    mainGame->players[mainGame->currentPlayer].letters[index].value = getScoreOfCharacter(mainGame->players[mainGame->currentPlayer].letters[index].letter);
                    printf("New letter: %c\n", mainGame->players[mainGame->currentPlayer].letters[index].letter);
                }else{
                    continueAsk = 0;
                }
                printf("Continue replacing letters? (1 for yes, 0 for no): ");
                scanf("%d", &continueAsk);
            }
            changeTurn(mainGame);
        }
        
    


    
}
void displayHUD(game * mainGame){
    /*
    This function displays the HUD that is important to the player.
    @param mainGame: the game struct that contains all the information about the game

    */
    printf("Player's high score: %d\n", mainGame->records.records[mainGame->players[mainGame->currentPlayer].recordIndex].highestScore);
    printf("Player %d's turn\n", mainGame->currentPlayer);
    printf("Player %s's turn\n", mainGame->players[mainGame->currentPlayer].name);
    printf("Player score: %d\n", mainGame->players[mainGame->currentPlayer].score);
    
    printf("Player's words: %d\n", mainGame->players[mainGame->currentPlayer].wordCount);
    printf("\n");
    printf("These are your letters: \n");
    for(int i = 0; i < 7; i++){
        printf("%c ", mainGame->players[mainGame->currentPlayer].letters[i].letter);
    }
    printf("\n");
    choiceController(mainGame);
    
}



void givePlayerTiles(game * mainGame){
    /*
    This function gives the player 7 tiles at the start of the game.
    @param mainGame: the game struct that contains all the information about the game

    */
    int randNum = 0;
    for(int j = 0; j < mainGame->numPlayers; j++){
        for(int i = 0; i < 7; i++){
       
        
            do{
                randNum = rand() % 27;
                char letter = mainGame->letterList.letters[randNum];
                if (mainGame->letterList.letterCount[randNum] > 0){
                    mainGame->players[j].letters[i].letter = letter;
                    mainGame->letterList.letterCount[randNum]--;
                }

            }while(mainGame->letterList.letterCount[randNum] <= 0);
        }
        mainGame->players[j].score = 0;
        mainGame->players[j].wordCount = 0;

    }


}

void initializeLetterList(game *mainGame){
    /*
    This function initializes the letter list struct.
    @param mainGame: the game struct that contains all the information about the game

    */
    char letter[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', '_'};
    int quantityOfEachChar[] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1,2};
    for(int i = 0; i < 27; i++){
        mainGame->letterList.letters[i] = letter[i];
        mainGame->letterList.letterCount[i] = quantityOfEachChar[i];
    }
}

int askForName(game *mainGame){
    /*
    This function asks the user for the name of the player.
    @param mainGame: the game struct that contains all the information about the game
    @return i which is the number of players that were added
    */
    char yesOrNo = 'y';
    int i = 0;
    while ((yesOrNo == 'y' && i <= 4)|| i < 2){
        printf("Enter name of player : ");
        scanf("%s", mainGame->players[i].name);
        mainGame->players[i].recordIndex = -1;
        printf("Do you want to add another player? (y/n) : ");
        if (i < 2){
            printf("There must be at least two players to start a game.\n");
        }
        scanf(" %c", &yesOrNo);
        i++;
    }
    
    
    return i;

}
void initializeRecords(game * mainGame){
    /*
    This function initializes the records struct.
    @param mainGame: the game struct that contains all the information about the game

    */
    FILE *fp;
    do{
        printf("Enter file path: ");
        scanf("%s", mainGame->records.filePath);
        fp = fopen(mainGame->records.filePath, "r");
        if (fp == NULL){
            printf("Error opening file\n");
            exit(1);
        }else{
            
            fscanf(fp, "%d", &mainGame->records.recordCount);
            
            for(int i = 0; i < mainGame->records.recordCount; i++){
                fscanf(fp, "%s", mainGame->records.records[i].name);
            
                fscanf(fp, "%s", mainGame->records.records[i].longestWord);
                fscanf(fp, "%d", &mainGame->records.records[i].highestScore);
                fscanf(fp, "%d", &mainGame->records.records[i].gamesPlayed);
                fscanf(fp, "%lf", &mainGame->records.records[i].averageScore);
            }
        }

    }while(fp == NULL);

}


bool checkWinCondition(game *mainGame){
    /*
    This function checks if the win condition has been met.
    @param mainGame: the game struct that contains all the information about the game
    @return true if the win condition has been met, false otherwise
    */
    int maxScore = 0;
    int wordCount = 0;
    for (int i = 0; i < mainGame->numPlayers; i++){
        if (mainGame->players[i].score > maxScore){
            maxScore = mainGame->players[i].score;
        }
        wordCount += mainGame->players[i].wordCount;
    }
    if (maxScore >= 20 || wordCount >= 12){ //set back to 88
        return true;
    }
    return false;


}



void matchRecords(game *mainGame){
    /*
    This function matches the records struct with the players struct.
    @param mainGame: the game struct that contains all the information about the game

    */
    for(int i = 0; i < mainGame->records.recordCount; i++){
        for(int j = 0; j < mainGame->numPlayers; j++){
            if (strcmp(mainGame->players[j].name, mainGame->records.records[i].name) == 0){
                printf("Match found for %s\n", mainGame->players[j].name);
                mainGame->players[j].recordIndex = i;
                
            }
        }
    }

}

void createNewRecord(game * mainGame){
    /*
    This function creates a new record for a player.
    @param mainGame: the game struct that contains all the information about the game

    */
    for(int i = 0; i < mainGame->numPlayers; i++){
        if (mainGame->players[i].recordIndex == -1){
            printf("Creating new record for %s\n", mainGame->players[i].name);
            mainGame->players[i].recordIndex = mainGame->records.recordCount;
            strcpy(mainGame->records.records[mainGame->records.recordCount].name, mainGame->players[i].name);
            
            mainGame->records.recordCount++;
        }
    }

}
void initPlayersToEmpty(game *mainGame){
    /*
    This function initializes the players struct to empty.
    @param mainGame: the game struct that contains all the information about the game

    */
    for(int i = 0; i < mainGame->numPlayers; i++){
        mainGame->players[i].score = 0;
        mainGame->players[i].wordCount = 0;
        mainGame->players[i].recordIndex = -1;
    }
    for(int i = 0; i < 10; i++){
        mainGame->records.records[i].averageScore = 0;
        mainGame->records.records[i].gamesPlayed = 0;
        mainGame->records.records[i].highestScore = 0;
        strcpy(mainGame->records.records[i].longestWord, "");
        for(int j = 0; j < 15; j++){
            setStringToEmpty(30, mainGame->records.records[i].words[j]);
        }
    }
    

}

void lookForLongestWord(int index, game* mainGame){
    /*
    This function looks for the longest word in the words array.
    @param index: the index of the player in the players struct
    @param mainGame: the game struct that contains all the information about the game

    */
    char longestWord[15];
    strcpy(longestWord, "");
    int playerIndex = mainGame->players[index].recordIndex;
    for(int i = 0; i < 15; i++){
        
        if (strlen(mainGame->records.records[playerIndex].words[i]) > strlen(mainGame->records.records[i].longestWord)){
            strcpy(longestWord, mainGame->records.records[playerIndex].words[i]);
            
            strcpy(mainGame->records.records[i].longestWord, longestWord);
            mainGame->records.records[i].longestWord[strlen(longestWord)] = '\0';
        }
        
    }

}

void updateRecords(game *mainGame){
    /*
    This function updates the records struct.
    @param mainGame: the game struct that contains all the information about the game

    */

    printf("update records Player's high score: %d\n", mainGame->records.records[mainGame->players[mainGame->currentPlayer].recordIndex].highestScore);
    for(int i = 0; i < mainGame->numPlayers; i++){
        lookForLongestWord(i, mainGame);
        if (mainGame->players[i].score > mainGame->records.records[mainGame->players[i].recordIndex].highestScore){
            
            mainGame->records.records[mainGame->players[i].recordIndex].highestScore = mainGame->players[i].score;
        }
        mainGame->records.records[mainGame->players[i].recordIndex].gamesPlayed++;
        mainGame->records.records[mainGame->players[i].recordIndex].averageScore = (mainGame->records.records[mainGame->players[i].recordIndex].averageScore + mainGame->players[i].score) / mainGame->records.records[mainGame->players[i].recordIndex].gamesPlayed;

    }



}

void writeToRecordFile(game *mainGame){
    /*
    This function writes the records struct to the records file.
    @param mainGame: the game struct that contains all the information about the game

    */

    FILE *fp;
    fp = fopen(mainGame->records.filePath, "w");
    if (fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }else{
        fprintf(fp, "%d\n", mainGame->records.recordCount);
        for(int i = 0; i < mainGame->records.recordCount; i++){
            fprintf(fp, "%s\n", mainGame->records.records[i].name);
            fprintf(fp, "%s", mainGame->records.records[i].longestWord);
            fprintf(fp, "%s", "\n");
            fprintf(fp, "%d\n", mainGame->records.records[i].highestScore);
            fprintf(fp, "%d\n", mainGame->records.records[i].gamesPlayed);
            fprintf(fp, "%lf\n", mainGame->records.records[i].averageScore);
        }
    
    }
        

}

void driver(game * mainGame){
    /*
    This function is the driver function for the game.
    @param mainGame: the game struct that contains all the information about the game

    */
    mainGame->currentPlayer = 0;
    //INIT GAME ELEMS
    initializeBoard(&mainGame->gameBoard);
    initializeLetterList(mainGame);

    //ASK FOR NAMES
    mainGame->numPlayers = askForName(mainGame);
    initPlayersToEmpty(mainGame);

    //RECORD INIT
    initializeRecords(mainGame);
    matchRecords(mainGame);
    createNewRecord(mainGame);


    givePlayerTiles(mainGame);
    loadDictionary(&mainGame->dictionary);
    
   
    do{
        displayBoard(mainGame);
        displayHUD(mainGame);

    }while(checkWinCondition(mainGame) == false);
    if (checkWinCondition(mainGame) == true){
        updateRecords(mainGame);
        writeToRecordFile(mainGame);
        printf("Game over\n");
    }
}

int main(){
    srand(time(NULL));
    game mainGame;
    driver(&mainGame);


}

