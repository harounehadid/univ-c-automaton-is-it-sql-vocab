#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct vocabList {
    char* vocabGroup;
    char** vocabWords;
    int wordCount;
}vocabList;

typedef struct automaton {
    struct vocabList* vocabularyList;
    struct automaton* next;
}automaton;


automaton* createNewNode(char* vocabsGroup, char** vocabs, int wordsNum) {
    // To prevent jumpig lines
    fflush(stdin);

    automaton* newNode = NULL;

    // The while loop to account for memory allocation failures
    do {
        newNode = malloc(sizeof(automaton));
    
    } while (newNode == NULL);

    do {
        newNode->vocabularyList = malloc(sizeof(vocabList));
    
    } while (newNode->vocabularyList == NULL);
    
    newNode->vocabularyList->vocabGroup = vocabsGroup;

    int wordCounter = 0;
    
    // Initialize the array of char pointers and count the number of words --------------------------------
    newNode->vocabularyList->vocabWords = malloc(wordsNum * sizeof(char *));

    for (int i = 0; i < wordsNum; i++) {
        // This the count the number of words in the vocab
        wordCounter++;
        // Fill the array with words
        newNode->vocabularyList->vocabWords[i] = vocabs[i];
    }
    // ----------------------------------------------------

    newNode->vocabularyList->wordCount = wordCounter;

    newNode->next = NULL;

    return newNode;
}

automaton* addToAutomatonChain(automaton* chain, automaton* node) {
    // To prevent jumping lines
    fflush(stdin);

    if (chain == NULL) {
        chain = node;
        node = NULL;
    }
    else {
        automaton* temp = chain;

        while (temp->next != NULL) {
            temp = temp->next;  
        }

        temp->next = node;
        temp = NULL;
        node = NULL;
    }

    return chain;
}

automaton* feedAutomaton(automaton* myAutomaton, char* vocabsGroup, char* vocabs[], int wordsNum) {
    automaton* newNode = createNewNode(vocabsGroup, vocabs, wordsNum);
    myAutomaton = addToAutomatonChain(myAutomaton, newNode);
    
    return myAutomaton;
}

automaton* createAutomaton(automaton* myAutomaton) {
    printf("\nCreating automaton...\n");
    myAutomaton = NULL;

    // There will be errors if you added more vocab without upping array's limit
    // Getting the number of rows and columns of a matrix would need much more work due to C limitations and since
    // it's out of scope I'm gonna just use a variable that should be equal to the number of words in the vocab
    char* vocabGroup;
    int vocabNum;

    vocabGroup = "keyword";
    vocabNum = 6;
    char* keywordsVocab[6] = {
        "select",
        "from",
        "where",
        "orderby",
        "desc",
        "asc"
    };

    myAutomaton = feedAutomaton(myAutomaton, vocabGroup, keywordsVocab, vocabNum);


    vocabGroup = "comparison operator";
    vocabNum = 6;
    char* compOpVocab[6] = {
        "=",
        ">",
        "<",
        ">=",
        "<=",
        "<>"
    };

    myAutomaton = feedAutomaton(myAutomaton, vocabGroup, compOpVocab, vocabNum);


    vocabGroup = "logical operator";
    vocabNum = 10;
    char* logicOpVocab[10] = {
        "all",
        "and",
        "any",
        "between",
        "exists",
        "in",
        "like",
        "not",
        "or",
        "some"
    };

    myAutomaton = feedAutomaton(myAutomaton, vocabGroup, logicOpVocab, vocabNum);

    
    vocabGroup = "seperator";
    vocabNum = 1;
    char* seperatorVocab[1] = {
        ","
    };

    myAutomaton = feedAutomaton(myAutomaton, vocabGroup, seperatorVocab, vocabNum);


    vocabGroup = "property/value";
    vocabNum = 3;
    char* propValVocab[3] = {
        "a",
        "1",
        "_"
    };

    myAutomaton = feedAutomaton(myAutomaton, vocabGroup, propValVocab, vocabNum); 

    return myAutomaton;
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

void displayAutomaton(automaton* myAutomaton) {
    automaton* temp = myAutomaton;

    while (temp) {
        printf("\n\n");

        for (int i = 0; i < temp->vocabularyList->wordCount; i++) {
            printf("%s  ", temp->vocabularyList->vocabWords[i]);
        }

        temp = temp->next;
    }
}

int main() {
    printf("---------- Welcome ----------------");

    automaton* myAutomaton = createAutomaton(myAutomaton);

    char* inputFileName = "testing-file.txt";
    // printf("\nEnter the name of the file:  ");
    // I'll add this later ---------------------------
    // scanf("%c", &fileName);
    printf("\nInput file name is: %s", inputFileName);

    char* fileText = readFileAndReturnText(inputFileName);

    printf("\n\n");

    return 0;
}