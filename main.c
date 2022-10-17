#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct vocabList {
    char* vocabType;
    char* vocabWords[];
}vocabList;

typedef struct automaton {
    vocabList* vocabularyList;
}automaton;


automaton* feedAutomaton(char* vocabs[]) {


    return NULL;
}

char** createAutomaton() {
    printf("\nCreating automaton...");
    automaton* myAutomaton = NULL;

    char* keywordsVocab[7] = {
        "select",
        "from",
        "where",
        "order",
        "by",
        "desc",
        "asc"
    };

    myAutomaton = feedAutomaton(keywordsVocab);

    char* vocabulary[100] = {
        "select",
        "from",
        "where",
        "order",
        "by",
        "desc",
        "asc",
        ",",
        "and",
        "or",
        "not"
    };

    // The first word in the vocab array gonna be vocabList's name, example: keywordVocab[0] = "keyword"

    // This variable should be equal to the number of vocabulary variable above
    // To some limitations this declaration had to be here
    const int vocabsToAdd = 100;

    char* automaton[vocabsToAdd];

    automaton[0] = "select";
    automaton[1] = "from";
    automaton[2] = "where";
    automaton[3] = "order";
    automaton[4] = "by";
    automaton[5] = "desc";
    automaton[6] = "asc";
    automaton[7] = ",";

    printf(automaton[0]);
    printf("\n");
    printf(automaton[1]);

    return NULL;
}

char* readFileAndReturnText(char* fileName) {
    FILE* file;
    char ch;

    // Opening the file
    file = fopen(fileName, "r");

    if (file == NULL) printf("\nThe file can't be opened\n");

    printf("\n\nChecking file...\n");

    // Count how many character in the file
    int charCount = 0;

    for (char c = fgetc(file); c != EOF; c = fgetc(file)) {
        charCount++;
    }

    // Going back to the begining of the file
    rewind(file);

    // Creating the variable that's gonna hold all file's text
    char* str = NULL;

    do {
        str = malloc(charCount * sizeof(char));

    } while (str == NULL);

    str[0] = '\0';

    // Append characters from the file to form a string
    do {
        ch = fgetc(file);
        strncat(str, &ch, 1);

    } while (ch != EOF);

    // Closing the file
    fclose(file);

    return str;
}

// bool checkAgainstAutomaton(automaton* chain, char* word) {
//     bool isRecognizable = true;
//     automaton* temp = chain;
//     int automatonLength = getAutomatonLength(chain);

//     // The -1 is added because the final state doesn't have a transition, I made '-' to indicate no transition exists
//     if (strlen(word) == automatonLength - 1) {
//         for (int i = 0; i < strlen(word); i++) {
//             if (word[i] != temp->transition || temp == NULL) {
//                 isRecognizable = false;
//                 break;
//             }

//             temp = temp->next;
//         }
//     }
//     else {
//         isRecognizable = false;
//     }

//     return isRecognizable;
// }

// I could've made this function better but it's way too out of this project's scope
// void analyzeAndFilter(automaton* chain, char* text) {
//     bool isRecognizable;
    
//     for (int i = 0; i < strlen(text) - 1; i++) {
//         char word[50] = "";
        
//         while (text[i] != ' ' && i < strlen(text) - 1) {
//             strncat(word, &text[i], 1);
//             i++;
//         }

//         printf("\nWord: %s", word);

//         isRecognizable = checkAgainstAutomaton(chain, word);

//         // Here dipaly the recognized and non recognized words
//         printf(isRecognizable ? "  Recognized" : "  Lexical Error");
//     }
// }

int main() {
    char* fileName = NULL;
    printf("---------- Welcome ----------------");
    printf("\nEnter the name of the file:  ");
    
    // I'll add this later ---------------------------
    // scanf("%c", &fileName);
    // printf("File Name is  %s", fileName);

    createAutomaton();

    char* fileText = readFileAndReturnText("testing-file.txt");

    printf("\nFILE TEXT: %s", fileName);
    printf("\n");

    printf("\n\n");

    return 0;
}