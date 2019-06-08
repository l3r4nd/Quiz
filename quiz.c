#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define size 150
#define max 10

typedef struct result{
    char Category;
    int mark, correct_opt;
} result;

void clr();
int menuController();
char* startQuiz();
int userAnswer();
int extract();
void readQuestion(FILE *, int , result *);
int* sort(int []);
int isUnique(int []);
int* generateSeq();
int getTotal(result *);
void report(result *, char *);
void freemem(char *, int [], result *);


int main() {
    FILE *file = fopen("quizes.txt", "r");
    result *score = (result*) malloc(sizeof(result) * max);
    int *seq, user_ans;
    char *name;
    int choice = menuController();
    
    if (choice) {
        name = startQuiz();
        seq = generateSeq();
        for(int i = 0; i < max; i ++){
            readQuestion(file, seq[i], (score + i));
            rewind(file);   //returns pointer to begining of the file.
            user_ans = userAnswer();
            if(user_ans == (score + i)->correct_opt){
                (score + i)->mark = 1;
            }
            else{
                (score + i)->mark = 0;
            }
        }
    }
    else {
        freemem(name, seq, score);
        fclose(file);
        exit(1);
    }

    report(score, name);

    freemem(name, seq, score);
    fclose(file);
    return 0;
}

void clr(){
    /*Clears screen */
    system("cls||clear");
}

int menuController(){
    /*
        Start of the quiz, Welcome dialog.
        proceeds further if true else exit the program.
     */
    int choice;
    
    printf("Welcome to Simple Quiz Game...\n");

    printf("Your Options\n");
    for(int i = 0; i < max; i++){
        printf("=");
    }
    printf("\n");

    printf("1. Start the Quiz\n");
    printf("2. Exit\n");

    printf("Pick:");
    scanf("%d", &choice);
    getchar();
    
    // return choice == 1 ? 1 : 0;
    if(choice == 1){
        return 1;
    }
    else{
        return 0;
    }
}

char* startQuiz(){
    /*
        This functions asks for name of the user and proceeds further if true otherwise exit.
        
        returns: 
            pointer to name of the user to main function.        
     */
    char *name = (char*) malloc(sizeof(char) * size);
    char choice;

    printf("Welcome to Sample Quiz Game\n");

    printf("Player Name:");
    gets(name);

    printf("\nReady (Y/N):");
    scanf(" %c", &choice);
    getchar();

    if (choice == 'Y' || choice == 'y'){
        return name;
    }
    else
        free(name);
        exit(1);
}

int userAnswer(){
    /*
        Asks for user input for the given question.

        returns: 
            integer option number.
     */

    char choice;
    int ans;
    printf("\nEnter your answer:");
    scanf(" %d", &ans);
    printf("\nLock it? (Y/N): ");
    scanf(" %c", &choice);
    getchar();
    if(choice == 'Y' || choice == 'y'){
        return ans;
    }
    else{
        userAnswer();
    }   
}

int extract(char *line, int len){
    /*
        Extracts correct option from line which is then stored in num array
        converts it to int.

        inputs: 
            char *line: pointer to line for extracting option.
            int len: length of the string.

        returns:
            converts char to int by calling atoi() function (in standard library under stdlib.)
        
     */
    char num[2];
    for(int i = 0; i <= 1; i++){
        num[1 - i] = line[len - i];
    }
    return atoi(num);
}

void readQuestion(FILE *file, int q_no, result *score){
    /*
        Displays question to the screen

        inputs:
            FILE *file: pointer to current file
            int q_no: which question number to display.
            result *score: used to store necessary information for further displaying
            of results.
     */
    char line[size];
    int low, high, len, cat_index, num = 0;
    
    if (q_no == 1){
        low = 1;
    }
    else if(q_no == 2){
        low = 11;
    }
    else {
        low = 11 * (q_no - 1);
    }
    high = low + max;
    cat_index = low + 1;

    while( fgets(line, sizeof(line), file)){
        if( (num >= (low - 1)) && (num < high - 1) ){
            len = strlen(line);
            
            if(len > 0 && line[len - 1] == '\n'){
                line[--len] = '\0';
            }
            puts(line);
        }
        if(num == high - 1){
            score->correct_opt = extract(line, len);
            // printf("correct opt: %d", score->correct_opt);
            // puts(line);
        }
        if(num == cat_index){
            score->Category = line[19];
        }
        num++;
    }
}

int* sort(int seq[]){
    /*
        sorts the given array in ascending order.
        
        inputs:
            int seq[]: given array

        returns:
            pointer to sorted array in ascending order
     */
    int temp;
    for(int i = 0; i < max; i++){
        for(int j = i + 1; j < max; j++){
            if(seq[i] > seq[j]){
                temp = seq[i];
                seq[i] = seq[j];
                seq[j] = temp;
            }
        }
    }
    return seq;
}

int isUnique(int seq[]){
    /*
        Checks if the given array is unique.

        intputs:
            int seq[]: given array

        returns:
            "true" if the array has all non repeated numbers otherwise "false".

     */
    int *arr = (int*) calloc(20, sizeof(int));
    for(int i = 0; i < max; i++){
        arr[seq[i]] += 1;
    }

    for(int i = 0; i < 20; i++){
        if(arr[i] > 1){
            free(arr);
            return 0;
        }
    }
    free(arr);
    return 1;
}

int* generateSeq(){
    /*
        generates sequence of int arrays

        FIXME: The array generated is always the same.

        returns:
            int pointer to array of unique numbers.
     */
    int *seq = (int*) calloc(max, sizeof(int));
    for(int i = 0; i < max; i++){
        seq[i] = ((rand() % 19) + 1);
    }

    if(!isUnique(seq)){
        free(seq);
        generateSeq();
    }

    seq = sort(seq);
    return seq;
}

int getTotal(result *score){
    /*
        Calculates the total score.

        inputs:
            result *score: struct result *score

        returns:
            total marks earned from the quiz.
     */
    int sum = 0;
    for(int i = 0; i < max; i++){
        sum += (score + i)->mark;
    }
    return sum;
}

void report(result *score, char *name) {
    /*
        Displays result of the user after completing the test

        inputs:
            result *score: pointer to struct result score
            char *name: pointer to name.
     */
    int total, temp_score;
    char cats[4] = "ABCD";
    clr();
    printf("\n");
    for(int i = 0; i < max; i++){
        printf("=");
    }
    
    printf("\nCongratulations %s . You have completed the quiz.\n", name);
    total = getTotal(score);
    printf("You have score %d out of %d.\n", total, max);

    printf("Category wise marks:\n");
    for(int cat_id = 0; cat_id < 4; cat_id++){
        temp_score = 0;
        for(int i = 0; i < max; i++){
            if(cats[cat_id] == (score + i)->Category){
                if((score + i)->mark){
                    temp_score++;
                }
            }
        }
        printf("Category %c \t:\t%d\n",cats[cat_id], temp_score);
    }
}

void freemem(char *name, int seq[], result *score){
    /*
        frees memory from given inputs.
     */
    free(name);
    free(seq);
    free(score);
}