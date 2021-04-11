#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hangman.h"


//global variables
char secretWord[WORD_SIZE];
char givenLetters[20];  //array to store given letters that are part of the secret word
int attempts = 0;       //to store number of attempts

int main()
{
    GameTitle();
    SelectSecretWord();

    do {
        DrawGallow();
        ReadLetter();
    } while (!Win() && !Hanged());

    if(Win())
        printf("\nCongratulations, you WIN!\n\n");
    else {
        printf("\nYou were hanged! More luck in the next life!\n\n");
        printf("The secret word was << %s >>", secretWord);
    }

    AddNewWord();

    return 0;
}

void AddNewWord(){
    char option;

    printf("\nDo you want to add a new word to the database? (y/n) >> ");
    scanf(" %c", &option);

    if(option == 'y') {
        char newWord[WORD_SIZE];

        printf("Enter the new word: ");
        scanf("%s", newWord);

        FILE* f;

        f = fopen("words.txt", "r+");
        if(f == 0) {
            printf("Word database not available!\n\n");
            exit(1);
        }

        int total;
        fscanf(f, "%d", &total);
        total++;

        fseek(f, 0, SEEK_SET);
        fprintf(f, "%d", total);

        fseek(f, 0, SEEK_END);
        fprintf(f, "\n%s", newWord);

        fclose(f);
    }
}

void SelectSecretWord(){
    FILE* f;

    f = fopen("words.txt", "r");
    if(f == 0) {
        printf("Word database not available!\n\n");
        exit(1);
    }

    int totalWords;
    fscanf(f, "%d", &totalWords);

    srand(time(0));
    int randomNumber = rand() % totalWords;

    for(int i = 0; i <= randomNumber; i++) {
        fscanf(f, "%s", secretWord);
    }

    fclose(f);
}

int FailedAttempts(){
    int failures = 0;

    for(int i = 0; i < attempts; i++) {

        if(!LetterExists(givenLetters[i])) {
            failures++;
        }
    }

    return failures;
}

int Hanged(){
    return FailedAttempts() >= 5;
}

int Win(){
    for(int i = 0; i < strlen(secretWord); i++) {
        if(!LetterInPlace(secretWord[i])) {
            return 0;
        }
    }

    return 1;
}

int LetterInPlace(char letter){
    int foundLetter = 0;

    for (int i = 0; i < attempts; i++){
        if (givenLetters[i] == letter){
            foundLetter = 1;
            break;
        }
    }

    return foundLetter;
}

void DrawGallow(){
    system("cls");

    int failures = FailedAttempts();

    printf("  _______       \n");
    printf(" |/      |      \n");
    printf(" |      %c%c%c  \n", (failures >= 1?'(':' '), (failures >= 1?'_':' '), (failures >= 1?')':' '));
    printf(" |      %c%c%c  \n", (failures >= 3?'\\':' '), (failures >= 2?'|':' '), (failures >= 3?'/': ' '));
    printf(" |       %c     \n", (failures >= 2?'|':' '));
    printf(" |      %c %c   \n", (failures >= 4?'/':' '), (failures >= 4?'\\':' '));
    printf(" |              \n");
    printf("_|___           \n");
    printf("\n\n");

    //if letter is in place, keep it, otherwise write _
    for (int i = 0; i < strlen(secretWord); i++){
        if (LetterInPlace(secretWord[i]))
            printf("%c ", secretWord[i]);
        else
            printf("_ ");
    }

    printf("\n\n");
}

void GameTitle(){
    printf("******************\n");
    printf("*  HANGMAN GAME  *\n");
    printf("******************\n\n");
}

void ReadLetter(){

    char givenLetter;

    //read letter from player
    printf("Enter a letter >> ");
    scanf(" %c", &givenLetter);

    if (LetterExists(givenLetter))
        printf("\nGood shot! Letter << %c >> IS part of the secret word!\n\n", givenLetter);
    else
        printf("\nSorry! Letter << %c >> is NOT part of the secret word!\n\n", givenLetter);

    givenLetters[attempts] = givenLetter;
    attempts++;

    getch();
}

int LetterExists(char letter){

    //check if letter is within the secret word
    for (int i = 0; i < strlen(secretWord); i++){
        if (secretWord[i] == letter)
            return 1;
    }

    return 0;
}
