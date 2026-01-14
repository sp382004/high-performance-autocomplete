#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 256
#define MAX_SUGGESTIONS 10

typedef struct item {
    char *word;
    int weight;
} Item;

void mergeSort(Item *items, int left, int right);
void merge(Item *items, int left, int mid, int right);
void sortByWeight(Item *items, int count);
int startsWith(const char *prefix, const char *word);
int findFirstPrefixMatch(Item *items, int n, const char *prefix);

void mergeSort(Item *items, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSort(items, left, mid);
    mergeSort(items, mid + 1, right);
    merge(items, left, mid, right);
}

void merge(Item *items, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Item *L = malloc(n1 * sizeof(Item));
    Item *R = malloc(n2 * sizeof(Item));

    for (int i = 0; i < n1; i++) L[i] = items[left + i];
    for (int j = 0; j < n2; j++) R[j] = items[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (strcmp(L[i].word, R[j].word) <= 0)
            items[k++] = L[i++];
        else
            items[k++] = R[j++];
    }

    while (i < n1) items[k++] = L[i++];
    while (j < n2) items[k++] = R[j++];

    free(L);
    free(R);
}

void sortByWeight(Item *items, int count) {
    for (int i = 1; i < count; i++) {
        Item temp = items[i];
        int j = i - 1;
        while (j >= 0 && (
            items[j].weight < temp.weight ||
            (items[j].weight == temp.weight && strcmp(items[j].word, temp.word) > 0)
        )) {
            items[j + 1] = items[j];
            j--;
        }
        items[j + 1] = temp;
    }
}

int startsWith(const char *prefix, const char *word) {
    return strncmp(prefix, word, strlen(prefix)) == 0;
}

int findFirstPrefixMatch(Item *items, int n, const char *prefix) {
    int low = 0, high = n - 1, answer = -1;
    size_t len = strlen(prefix);
    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strncmp(items[mid].word, prefix, len);
        if (cmp >= 0) {
            if (startsWith(prefix, items[mid].word)) answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return answer;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <dictionary> <queryfile>\n", argv[0]);
        return 1;
    }

    FILE *dictFile = fopen(argv[1], "r");
    FILE *queryFile = fopen(argv[2], "r");
    if (!dictFile || !queryFile) {
        fprintf(stderr, "Error opening files.\n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    int wordCount = 0;
    while (getline(&line, &len, dictFile) != -1) wordCount++;
    rewind(dictFile);

    Item *dictionary = malloc(wordCount * sizeof(Item));
    char word[BUFSIZE];
    int weight;

    for (int i = 0; i < wordCount; i++) {
        fscanf(dictFile, "%s %d\n", word, &weight);
        dictionary[i].word = malloc(strlen(word) + 1);
        strcpy(dictionary[i].word, word);
        dictionary[i].weight = weight;
    }
    fclose(dictFile);
    mergeSort(dictionary, 0, wordCount - 1);

    while (getline(&line, &len, queryFile) != -1) {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        printf("Query word:%s\n", line);

        int start = findFirstPrefixMatch(dictionary, wordCount, line);
        if (start == -1) {
            printf("No suggestion!\n");
            continue;
        }

        Item *matches = malloc(wordCount * sizeof(Item));
        int matchCount = 0;
        for (int i = start; i < wordCount; i++) {
            if (startsWith(line, dictionary[i].word)) {
                matches[matchCount++] = dictionary[i];
            } else {
                break;
            }
        }

        sortByWeight(matches, matchCount);
        for (int i = 0; i < matchCount && i < MAX_SUGGESTIONS; i++) {
            printf("%s %d\n", matches[i].word, matches[i].weight);
        }

        free(matches);
    }

    fclose(queryFile);
    free(line);
    for (int i = 0; i < wordCount; i++) free(dictionary[i].word);
    free(dictionary);
    return 0;
}