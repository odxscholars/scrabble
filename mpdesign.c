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
#define BOARDHEIGHT 11;
#define BOARDWIDTH 11;


/*
This struct is used to store the information of a letter tile.

@var letter: the letter of the tile which is a character
@var value: the value or score of the letter which is an integer
*/

struct letterTile{
    char letter;
    int value;
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
tiles - an 11 by 11  2d array of board tiles
*/
struct board{
    boardTile tiles[11][11];
};

typedef struct board board;

/*
The player struct is used to store the information of a player.
@var name - a string that contains the name of the player
@var score - an integer that contains the score of the player
@var letters - an array of letter tiles that contains the letters of the player that are initially availble


*/

struct player{
    char name[30];
    int score;
    letterTile letters[7];
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
    char longestWord[30];
    int highestScore;
    int gamesPlayed;
    double averageScore;
};

typedef struct record record;

/*
The recordFile struct is used to store the different records that are available.
@var filePath - a string that contains the path of the file
@var file - a pointer to the file
@var records - an array of records that contains the records of the players

*/
struct recordFile{
    char filePath[100];
    FILE *file;
    record records[10];

};

typedef struct recordFile recordFile;


/*
The dict struct is used to store the different words that are considered "valid"
@var words - an array of 100 strings that contains the words that are considered valid
*/
struct dict{
    char words[100][10];
    int wordCount;
};

typedef struct dict dict;

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
@var recordFile - a variable of the recordFile struct that contains the record file of the game
*/
typedef struct game{
    board gameBoard;
    player players[4];
    dict dictionary;
    recordFile records;
    int currentPlayer;
    letterBank letterList;
} game;



void loadDictionary(dict *dictionary){
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
    for(int i = 0; i < dictionary->wordCount; i++){
        if(strcmp(key, dictionary->words[i]) == 0){
            return true;
        }
    }
    return false;
}



void initializeBoard(board *gameBoard){
    int i,j;
    for(i=0;i<11;i++){
        for(j=0;j<11 ;j++){
            gameBoard->tiles[i][j].isEmpty = true;
        }
    }
}


int getScoreOfCharacter(char letter){
    char letters[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', ' '};
    int score[] = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10,0};
    for(int i = 0; i < 27;i ++){
        if (letters[i] == letter){
            return score[i];
        }
    }
    return -1;
}


void displayBoard(game * mainGame){
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
    for(int i = 0; i < size; i++){
        if (i == 0){
            if (yArr[i] != yArr[i+1] && xArr[i] != xArr[i+1]){ //if the first and second coordinates are not connected
                return false;
            }
        }else if(i == size - 1){
            if (yArr[i] != yArr[i-1] && xArr[i] != xArr[i-1]){ //if the last and second last coordinates are not connected
                return false;
            }
        }else{
            if ((yArr[i] != yArr[i-1] && xArr[i] != xArr[i-1]) && (yArr[i] != yArr[i+1] && xArr[i] != xArr[i+1])){ //if the current coordinate is not connected to the previous and next coordinate
                return false;
            }
        }
    }
    return true;

}

char getRandomLetter(game * mainGame){
    int random = 0;
    char letter;
    do{
        random = rand() % 27;
        letter = mainGame->letterList.letters[random];
    }while(mainGame->letterList.letterCount[random] > 0);
    mainGame->letterList.letterCount[random]--;
    return letter;
}
void swapPlayerLetterWithNewOne(game * mainGame, char oldLetter){
    for(int i = 0; i < 7; i++){
        if(mainGame->players[mainGame->currentPlayer].letters[i].letter == oldLetter){
            mainGame->players[mainGame->currentPlayer].letters[i].letter = getRandomLetter(mainGame);
            break;
        }
    }
    
}

void displayHUD(game * mainGame){
    printf("Player %s's turn", mainGame->players[mainGame->currentPlayer].name);
    printf("\n");
    printf("These are your letters: \n");
    for(int i = 0; i < 7; i++){
        printf("%c ", mainGame->players[mainGame->currentPlayer].letters[i].letter);
    }
    printf("\n");
    char temp[8];
    int tempX[8];
    int tempY[8];
    bool continueLoop = true;
    
    do{
        while(continueLoop){
            printf("Enter the letters you want to use: ");
            scanf("%s", temp);
            if(strlen(temp) > 7){
                printf("You can only use 7 letters at a time. Please try again.\n");
            }else{
                continueLoop = false;
            }
        }
            
        for(int i = 0; i< strlen(temp); i++){
            printf("Where will %c go? x, y: ", temp[i]);
                
            scanf("%d %d", &tempX[i], &tempY[i]);
        }
        if(!checkIfCoordinatesAreConnected(tempY, tempX, strlen(temp))){
            printf("The coordinates you entered are not connected. Please try again.\n");
        }
    }while(checkIfCoordinatesAreConnected(tempY, tempX, strlen(temp)) == false);
    for(int i = 0; i < strlen(temp); i++){
        mainGame->gameBoard.tiles[tempY[i]][tempX[i]].letter.letter = temp[i];
        mainGame->gameBoard.tiles[tempY[i]][tempX[i]].isEmpty = false;
        swapPlayerLetterWithNewOne(mainGame, temp[i]);
    }
}



void givePlayerTiles(game * mainGame){
    int randNum = 0;
    for(int i = 0; i < 7; i++){
       
        
        do{
            randNum = rand() % 27;
            char letter = mainGame->letterList.letters[randNum];
            if (mainGame->letterList.letterCount[randNum] > 0){
                mainGame->players[mainGame->currentPlayer].letters[i].letter = letter;
                mainGame->letterList.letterCount[randNum]--;
            }

        }while(mainGame->letterList.letterCount[randNum] <= 0);
    }


}

void initializeLetterList(game *mainGame){
    char letter[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', ' '};
    int quantityOfEachChar[] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1,2};
    for(int i = 0; i < 27; i++){
        mainGame->letterList.letters[i] = letter[i];
        mainGame->letterList.letterCount[i] = quantityOfEachChar[i];
    }
}

int askForName(game *mainGame){
    char yesOrNo = 'y';
    int i = 0;
    while (yesOrNo == 'y' && i <= 4){
        printf("Enter name of player : ");
        scanf("%s", mainGame->players[i].name);
        printf("Do you want to add another player? (y/n) : ");
        scanf(" %c", &yesOrNo);
        i++;
    }
    
    
    return i;

}

void driver(game * mainGame){
    mainGame->currentPlayer = 0;
    initializeBoard(&mainGame->gameBoard);
    initializeLetterList(mainGame);
    int numPlayers = askForName(mainGame);
    for(int i = 0; i < numPlayers; i++){
        givePlayerTiles(mainGame);
    }
    loadDictionary(&mainGame->dictionary);
    
    bool continuePlaying = true;
    do{
        displayBoard(mainGame);
        displayHUD(mainGame);

    }while(continuePlaying);
}

int main(){
    srand(2);
    game mainGame;
    driver(&mainGame);


}

