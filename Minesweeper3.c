// Author Name: Gideon Blank
// Registration Number: 230156317

// The program will takes user input to create an empty grid
// After the user inputs what their starting place will be, the program generates bombs around the matrix except in an area around their starting point
// Once the matrix has been fully generated, the user can choose co-ordinates to take an action at
// This action could be flagging a potential bomb, breaking a potentially empty space to reveal its values beneath, removing a flag or going back to choose again
// This continues until the user has flagged every bomb or accidentally breaks a bomb
// Then the user is prompted to either play again or end the program


#include <stdio.h>
#include <stdlib.h>

struct grid
{
    int gridLength; // stores the length of the matrix / x coordinate that the user input
    int gridWidth;   // stores the width of the matrix / y coordinate that the user input
    char gridVal[26][26]; // stores the value displayed in each cell of the matrix
    int isbomb[26][26];   // stores which grid coordinates are bombs
    int nearbomb[26][26];  // stores the value each non-bomb coordinate has which counts how many bombs are in a 3x3 area around it
    int isbroken[26][26];  // stores which matrix coordinates have been broken by the user
    int flagnum;    //counts how many flags the user starts with/ has left
    int start;      // used to check if the game has been initialised yet
    int end;        // used to check if the game has ended
    int win;        // 1 if the player has won, 0 if they have not yet
};

void gridArea(struct grid *userGrid);   // calculates the area of the grid from user input

void gridInit(struct grid *userGrid);  // initialises values of the code

void gridGen(struct grid *userGrid, struct grid *userChoice);  //generates the grid

void gridDisp(struct grid userGrid);  // prints grid values to screen

void userPick(struct grid *userChoice, struct grid *userGrid);  // stores user input

void gameover(struct grid *userGrid, struct grid *userChoice);  // is called when the played loses/wins/ends the game



main(){
    struct grid matrix[2]; // array of struct, there are 2 instances- first for values stored in grid, second for user input
    matrix[0].end=1;
    while(matrix[0].end){  // loops until the user stops the game with gameover()

        srand(time(NULL));
        int rand_num = rand() % 100;    //used to generate random bomb patterns

        matrix[0].start = 0;

        gridArea(&matrix[0]);
        gridInit(&matrix[0]);
        gridDisp(matrix[0]);
        userPick(&matrix[1], &matrix[0]);
    }

}

void gridArea(struct grid *userGrid){

    printf("\tWelcome to Mine Sweeper\n\n");
    do{
        userGrid->gridLength=-1;      //each time the loop occurs these variables are set to an invalid number
        userGrid->gridWidth =-1;
        printf("The game can be played on any rectangle from 5 by 5 to 26 by 26\n");
        printf("Please enter the length (x) of the rectangle: ");
        fseek(stdin, 0, SEEK_END);              // used to reject errors if user input is an incorrect type
        scanf(" %d", &userGrid->gridLength); //x
        printf("Please enter the width (y) of the rectangle:");
        fseek(stdin, 0, SEEK_END);
        scanf(" %d", &userGrid->gridWidth); // y
    }while((userGrid->gridLength<5 || userGrid->gridLength>26) || (userGrid->gridWidth<5 || userGrid->gridWidth>26));
                    // checks that user input is within correct range
}


void gridInit(struct grid *userGrid){
    int i, j;
    for(i=0; i<userGrid->gridWidth; i++){ // y
        for(j=0; j<userGrid->gridLength; j++){ // x
            userGrid->gridVal[i][j] = ' ';
            userGrid->isbomb[i][j] = 0;
            userGrid->nearbomb[i][j] = 0;
            userGrid->isbroken[i][j] = 0;
        }
    }

}

void gridDisp(struct grid userGrid){
    int i, j;
    float widthFLT;


    printf("\n\t");
    for(j=0; j<userGrid.gridLength; j++){
            printf("   %c ", (j + 97));      // +97 turns number into letters using ASCII
    }

    for(i=0; i<userGrid.gridWidth; i++){
        widthFLT = i;
        printf("\n%7.f  ", (widthFLT+1));    // float used to format the grid uniformly

        for(j=0; j<userGrid.gridLength;j++){
            printf("| %c |", userGrid.gridVal[i][j]);
        }

        printf("  %d", (i+1));

    }
    printf("\n\t");
    for(j=0; j<userGrid.gridLength; j++){
            printf("   %c ", (j + 97));
    }
    if(userGrid.start ==1){
        printf("\n\n \t \t\t You have %d flags left\n", userGrid.flagnum);
    }

}

void userPick(struct grid *choice, struct grid *userGrid){
    char x;
    int loop=1;


    do{
        choice->gridLength=-1;
        choice->gridWidth =-1;
        printf("\nWhich letter (x) co-ordinate would you like to select to begin the game?"); //needs to check its a cord which isnt already broken
        fseek(stdin, 0, SEEK_END);
        scanf(" %c", &x);
        x = tolower(x);

        choice->gridLength = (int)(x) - 96 ;    // convert the letter to a digit by doing like %d -97
        printf("\nPlease input a y co-ordinate as well");
        fseek(stdin, 0, SEEK_END);
        scanf(" %d", &choice->gridWidth);

    }while(isalpha(x)!=2 || choice->gridLength<1 || (choice->gridLength>userGrid->gridLength) || choice->gridWidth <1 || choice->gridWidth>userGrid->gridWidth);

    userGrid->gridVal[choice->gridWidth-1][choice->gridLength-1] = 'X'; // temporarily marks spot user chose


    gridDisp(*userGrid);
    sleep(1); // used to add a small wait so user can see where the X is
    printf("\n\n\n\n");
    userGrid->start = 1;
    gridGen(userGrid, choice);
    userGrid->win = userGrid->flagnum; // sets the number of correct flags needed to win to equal total number of flags

    while(loop){
        do{
            choice->gridLength=-1;
            choice->gridWidth =-1;
            printf("\nWhich letter (x) co-ordinate would you like to do next? ");
            fseek(stdin, 0, SEEK_END);
            scanf(" %c", &x);
            x=tolower(x);
            choice->gridLength = (int)(x) - 96 ;    // convert the letter to a digit by doing like %d -97
            printf("\nPlease input a y co-ordinate as well");
            fseek(stdin, 0, SEEK_END);
            scanf(" %d", &choice->gridWidth);
        }while(isalpha(x)!=2 || choice->gridLength<1 || (choice->gridLength>userGrid->gridLength) || choice->gridWidth <1 || choice->gridWidth>userGrid->gridWidth);

        printf("\nWhat would you like to do at this co-ordinate? b = break, f = flag, n = choose again (type f to remove a flag from an already flagged co-ordinate)");
        fseek(stdin, 0, SEEK_END);
        scanf(" %c", &x);
        switch (x){
            case 'b':
                if(userGrid->gridVal[choice->gridWidth-1][choice->gridLength-1] != 'F'){ // checks user is not trying to break a flag
                    if(userGrid->isbroken[choice->gridWidth-1][choice->gridLength-1] == 0){  // checks block not already broken
                        userGrid->isbroken[choice->gridWidth-1][choice->gridLength-1] = 1;
                        if(userGrid->isbomb[choice->gridWidth-1][choice->gridLength-1] == 1){
                            printf("\n\n\t Game Over!  You broke a bomb block (all bomb blocks (B) now revealed)");
                            gameover(userGrid, choice);
                            loop=0;
                            break;
                        }else{

                            userGrid->gridVal[choice->gridWidth-1][choice->gridLength-1] = userGrid->nearbomb[choice->gridWidth-1][choice->gridLength-1] + '0';
                            gridDisp(*userGrid);    //"breaks" the block and replaces it with it's nearbomb value

                        }
                    }else{printf("\nThis block is already broken");}
                 }else{
                        printf("\nYou cannot break a flag (if you want to break this block, remove the flag first)");}

                break;
            case 'f':
                if(userGrid->isbroken[choice->gridWidth-1][choice->gridLength-1] == 0){     // checks user is not trying to flag an already broken block
                    if(userGrid->gridVal[choice->gridWidth-1][choice->gridLength-1] =='F'){    // checks if user is trying to remove a flag, if so replace with empty space
                        userGrid->gridVal[choice->gridWidth-1][choice->gridLength-1] = (' ');
                        userGrid->flagnum++;    //if user removes a flag, give them back a flag

                        if(userGrid->isbomb[choice->gridWidth-1][choice->gridLength-1] == 1){
                            userGrid->win++;    // if user removed a flag which was successfully covering a bomb a point is added back to win (user wins the game when win =0)
                        }
                    } else if(userGrid->flagnum>0){
                        userGrid->gridVal[choice->gridWidth-1][choice->gridLength-1] = 'F';//replace highlighted block with flag
                        userGrid->flagnum--;    // user places down a flag so total flag number decreases

                        if(userGrid->isbomb[choice->gridWidth-1][choice->gridLength-1] == 1){
                            userGrid->win--;    // if user placed a flag correctly on a bomb, win decrements (when win = 0 player wins the game)
                        }
                    }else{printf("You cannot place anymore flags down, type F on an already placed flag to take it back");} // if user has already placed every flag available
                    gridDisp(*userGrid);
                }else{printf("\n you cannot flag an already broken space\n");}
                break;


            case 'n':
                break; // breaks back to loop to ask user to input another co-ordinate
            default:
                printf("\nPlease enter a valid character(b,f or n)");
                sleep(1);
        }
    if(userGrid->win==0){
        printf("\n\n\tWell done, you won!\n\n");
        loop =0;
        gameover(userGrid, choice);
        }
    }
}

void gridGen(struct grid *userGrid, struct grid *choice){

    int bombnum, i, j, k, randomL, randomW, more=1;

    bombnum = userGrid->gridLength * userGrid->gridWidth * 0.17; // calculates how many bombs will be in the grid
    userGrid->flagnum = bombnum; // number of flags = number of bombs

    for(i=0; i<bombnum; i++){
        do{
        randomL = rand() % (userGrid->gridLength);  // randomly picks a co-ordinate for a bomb
        randomW = rand() % (userGrid->gridWidth);

        // checks that the bomb co-ordinate is not near the user choice
        }while((randomL == choice->gridLength-2 || randomL == choice->gridLength-1 || randomL == choice->gridLength)&&
               (randomW == choice->gridWidth-2 || randomW == choice->gridWidth-1 || randomW == choice->gridWidth) || userGrid->isbomb[randomW][randomL] == 1); // probably should optimise this idk

        userGrid->isbomb[randomW][randomL] = 1; // stores where each bomb is in the grid

        for(j=-1; j<2; j++){
            for(k =-1; k<2; k++){
                userGrid->nearbomb[randomW+j][randomL+k] = userGrid->nearbomb[randomW+j][randomL+k] +1;

                userGrid->isbroken[choice->gridWidth-1+j][choice->gridLength-1+k] = 1;

                userGrid->gridVal[choice->gridWidth-1+j][choice->gridLength-1+k] =
                        userGrid->nearbomb[choice->gridWidth-1+j][choice->gridLength-1+k] + '0';

            }
        }
    }


    gridDisp(*userGrid);

}


void gameover(struct grid *userGrid, struct grid *choice){
    int check = 1, i, j;

    for(i=0; i<userGrid->gridWidth; i++){ // y
        for(j=0; j<userGrid->gridLength; j++){ // x
            if(userGrid->isbomb[i][j] ==1){
                userGrid->gridVal[i][j] = 'B';{ // shows each bomb's location
                }
            }
        }
    }

    gridDisp(*userGrid);


    do{
        check=1;
        userGrid->end = -1;
        printf("\n\nWould you like to play again? (1 for yes 0 for no)\n");
        fseek(stdin, 0, SEEK_END);
        scanf(" %d", &userGrid->end);


        switch (userGrid->end){

            case 1:
                check=0;
                break;

            case 0:
                check=0;
                break;

            default:
                break;

        }

    }while(check ==1);
    printf("\n\n");

}


