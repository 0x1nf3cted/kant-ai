#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 1000

struct word_node
{
    char word[MAX_LENGTH];
    struct word_node *next;
};

struct word_node *generateMarkovChain(char *inputText)
{
    struct word_node *start = NULL;
    struct word_node *current = NULL;
    char *word = strtok(inputText, " ");

    while (word != NULL)
    {
        struct word_node *newNode = (struct word_node *)malloc(sizeof(struct word_node));
        strcpy(newNode->word, word);
        newNode->next = NULL;

        if (start == NULL)
        {
            start = newNode;
            current = newNode;
        }
        else
        {
            current->next = newNode;
            current = newNode;
        }

        word = strtok(NULL, " ");
    }

    return start;
}

char *getNextWord(struct word_node *start, char *currentWord)
{
    struct word_node *currentNode = start;
    struct word_node *possibilities = NULL;
    int count = 0;

    while (currentNode != NULL)
    {
        if (strcmp(currentNode->word, currentWord) == 0)
        {
            struct word_node *newNode = (struct word_node *)malloc(sizeof(struct word_node));
            strcpy(newNode->word, currentNode->next->word);
            newNode->next = NULL;

            if (possibilities == NULL)
            {
                possibilities = newNode;
            }
            else
            {
                struct word_node *temp = possibilities;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
            count++;
        }
        currentNode = currentNode->next;
    }

    if (count > 0)
    {
        int randomIndex = rand() % count;
        struct word_node *temp = possibilities;
        for (int i = 0; i < randomIndex; i++)
        {
            temp = temp->next;
        }
        char *nextWord = strdup(temp->word);
        free(possibilities);
        return nextWord;
    }

    free(possibilities);
    return NULL;
}

void generateText(struct word_node *start, int numWords)
{
    struct word_node *currentNode = start;
    int count = 0;

    while (currentNode != NULL && count < numWords)
    {
        printf("%s ", currentNode->word);
        currentNode = getNextWord(start, currentNode->word);
        count++;
    }

    printf("\n");
}

int main(int argc, char const *argv[])
{
    FILE *file = fopen("training_data.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long fileLength = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *inputText = (char *)malloc(fileLength + 1);
    if (inputText == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(file);
        return 1;
    }

    size_t bytesRead = fread(inputText, 1, fileLength, file);
    if (bytesRead != fileLength)
    {
        printf("Error reading file.\n");
        free(inputText);
        fclose(file);
        return 1;
    }
    inputText[fileLength] = '\0';

    fclose(file);

    struct word_node *start = generateMarkovChain(inputText);
    int nb_words = 100;
    if (argc > 1)
    {
        nb_words = atoi(argv[1]);
    }

    srand(time(NULL));
    generateText(start, nb_words);

    struct word_node *currentNode = start;
    while (currentNode != NULL)
    {
        struct word_node *temp = currentNode;
        currentNode = currentNode->next;
        free(temp);
    }

    free(inputText);

    return 0;
}
