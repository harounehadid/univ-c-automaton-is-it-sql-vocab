#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct vocabList {
    char* vocabGroup;
    char** vocabWords;
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
    
    // Initialize the array of char pointers --------------------------------
    newNode->vocabularyList->vocabWords = malloc(wordsNum * sizeof(char *));

    for (int i = 0; i < wordsNum; i++) {
        newNode->vocabularyList->vocabWords[i] = vocabs[i];
    }
    // ----------------------------------------------------

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
    char* keywordsVocabGroup = "keyword";
    int keywordsVocabNum = 7;
    char* keywordsVocab[7] = {
        "select",
        "from",
        "where",
        "order",
        "by",
        "desc",
        "asc"
    };

    myAutomaton = feedAutomaton(myAutomaton, keywordsVocabGroup, keywordsVocab, keywordsVocabNum);

    fflush(stdin);
    fflush(stdout);

    char* compOpVocabGroup = "comparison operator";
    int compOpVocabNum = 6;
    char* compOpVocab[6] = {
        "=",
        ">",
        "<",
        ">=",
        "<=",
        "<>"
    };

    myAutomaton = feedAutomaton(myAutomaton, compOpVocabGroup, compOpVocab, compOpVocabNum);

    

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

int main() {
    char* fileName;
    printf("---------- Welcome ----------------");
    printf("\nEnter the name of the file:  ");
    
    // I'll add this later ---------------------------
    // scanf("%c", &fileName);
    // printf("File Name is  %s", fileName);

    automaton* myAutomaton = createAutomaton(myAutomaton);
    automaton* temp = myAutomaton;
    temp = temp->next;

    fflush(stdout);

    printf("\n  %s", temp->vocabularyList->vocabWords[0]);
    printf("\n  %s", temp->vocabularyList->vocabWords[1]);
    printf("\n  %s", temp->vocabularyList->vocabWords[2]);
    // printf("\n  %s", temp->vocabularyList->vocabWords[3]);
    // printf("\n  %s", temp->vocabularyList->vocabWords[4]);
    // printf("\n  %s", temp->vocabularyList->vocabWords[5]);
    // printf("\n  %s", temp->vocabularyList->vocabWords[6]);

    fflush(stdout);

    // char* fileText = readFileAndReturnText("testing-file.txt");

    // printf("\nFILE TEXT: %s", fileName);
    // printf("\n");

    printf("\n\n");

    return 0;
}