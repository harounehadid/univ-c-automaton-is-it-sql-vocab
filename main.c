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
    printf("\n...Creating automaton\n");
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
        "order by",
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
    vocabNum = 63;
    char* propValVocab[63] = {
        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
        "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
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

    printf("\n\n...Checking file\n");

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

// Return a copy text into all lowercase
char* getWordInAllLowercase(char* text) {
    char* textInLowercase = malloc(strlen(text) * sizeof(char));

    textInLowercase[0] = '\0';
    for (int i = 0; i < strlen(text); i++) {
        if (isalpha(text[i]) != 0) {
            textInLowercase[i] = tolower(text[i]);
        }
        else {
            textInLowercase[i] = text[i];
        }
    }

    return textInLowercase;
}

char* checkAgainstAutomaton(automaton* chain, char* word) {
    automaton* temp = chain;
    char* wordGroup = NULL;

    char* propValGroup = "property/value";

    char* wordInLowercase = getWordInAllLowercase(word);

    while (temp) {
        for (int i = 0; i < temp->vocabularyList->wordCount; i++) {
            // Check if the first character of a string is an integer, if it is than break out
            if (isdigit(word[0])) break;

            if (temp->vocabularyList->vocabGroup != propValGroup) {
                char* curWord = temp->vocabularyList->vocabWords[i];

                if (strcmp(curWord, wordInLowercase) == 0) {
                    wordGroup = temp->vocabularyList->vocabGroup;
                    break;
                }
            }
            else {
                // Check if it is property/value
                char* pvStr;
                char pvCh;
                int counter = 0;

                do {
                    pvStr = malloc(temp->vocabularyList->wordCount * sizeof(char*));

                } while (pvStr == NULL);

                pvStr[0] = '\0';

                for (int j = 0; j < temp->vocabularyList->wordCount; j++) {
                    pvCh = temp->vocabularyList->vocabWords[j][0];
                    strncat(pvStr, &pvCh, 1);
                }

                for (int j = 0; j < strlen(wordInLowercase); j++) {
                    if (strchr(pvStr, wordInLowercase[j]) != NULL) counter++;
                }

                if (counter != 0 && counter == strlen(wordInLowercase)) wordGroup = temp->vocabularyList->vocabGroup;
            }
        }

        if (wordGroup) break;

        temp = temp->next;
    }

    return wordGroup;
}

// I could've made this function better but it's way too out of this project's scope
void analyzeFilterAndOutput(automaton* chain, char* inputText, char* outputFileName) {
    printf("\n...Checking with the Automaton\n");

    FILE* file = fopen(outputFileName, "w");
    
    for (int i = 0; i < strlen(inputText) - 1; i++) {
        char word[25] = "";
        
        while (inputText[i] != ' ' && i < strlen(inputText) - 1) {
            strncat(word, &inputText[i], 1);
            i++;

            // This to get the whole of "order by" keyword
            if (strcmp(getWordInAllLowercase(word), "order") == 0) {
                if (word[i] == ' ') break;

                char iPlus1Ch = inputText[i + 1];
                char iPlus2Ch = inputText[i + 2];

                // If still not the end of string check if it's a space
                bool thirdPosInRange = i + 3 < strlen(inputText) - 1;

                if (tolower(iPlus1Ch) == 'b' && tolower(iPlus2Ch) == 'y') {

                    if (thirdPosInRange) {
                        if (inputText[i + 3] != ' ') break;
                    }

                    strncat(word, &inputText[i], 1);
                    fflush(stdin);
                    i++;
                }
            }
        }

        // This shows the type of word, example Keyword or logical operator
        char* wordGroup = checkAgainstAutomaton(chain, word);

        // If the word is not recognized by the lexecal analyzer print "Unknown"
        if (wordGroup == NULL) wordGroup = "Unknown";
        
        // print to the file
        fprintf(file, "%s(%s) ", word, wordGroup);
    }

    fclose(file);
}

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
    printf("\n---------- Welcome ----------------\n");

    automaton* myAutomaton = createAutomaton(myAutomaton);

    // You can give the user the option to enter the name of the input and output file
    char* inputFileName = "input-file.txt";
    char* outputFileName = "output-file.txt";

    printf("\nInput file name is: %s", inputFileName);
    printf("\nOutput file name is: %s", outputFileName);

    char* fileText = readFileAndReturnText(inputFileName);

    printf("\nFile text: %s", fileText);
    printf("\n");

    analyzeFilterAndOutput(myAutomaton, fileText, outputFileName);

    printf("\nText analyzing is done you can check the output file");

    printf("\n\n\n");

    return 0;
}