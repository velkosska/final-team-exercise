// media_manager.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "media_manager.h"

void getValidInput(const char *prompt, char *buffer, int size, int (*validate)(const char *)) {
    do {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin) == NULL) {
            buffer[0] = '\0';
            return;
        }
        strtok(buffer, "\n");  // Remove newline
        if (!validate(buffer)) {
            printf("Invalid input, please try again.\n");
        }
    } while (!validate(buffer));
}

int validateInput(const char *input) {
    if (input == NULL) return 0;
    while (*input) {
        if (!isspace((unsigned char)*input))
            return 1;
        input++;
    }
    return 0;
}

int validateMediaType(const char *input) {
    if (!validateInput(input)) return 0;
    char temp[MAX_STRING];
    strncpy(temp, input, MAX_STRING);
    temp[MAX_STRING-1] = '\0';
    toLowerCase(temp);
    return (strcmp(temp, "movie") == 0 ||
            strcmp(temp, "book") == 0 ||
            strcmp(temp, "album") == 0 ||
            strcmp(temp, "show") == 0);
}

int validateLink(const char *input) {
    if (!validateInput(input)) return 0;
    return (strncmp(input, "http://", 7) == 0 ||
            strncmp(input, "https://", 8) == 0 ||
            strncmp(input, "www.", 4) == 0);
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void saveEntry(const char *filename) {
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    struct MediaEntry entry;

    // CSV file order: Title, Type, Author, Duration, Genre, Comment, Link
    getValidInput("Enter title: ", entry.title, MAX_STRING, validateInput);
    getValidInput("Enter type (Movie, Book, Album, Show): ", entry.type, MAX_STRING, validateMediaType);
    getValidInput("Enter author/director/artist: ", entry.author, MAX_STRING, validateInput);
    getValidInput("Enter duration/pages: ", entry.duration, MAX_STRING, validateInput);
    getValidInput("Enter genre: ", entry.genre, MAX_STRING, validateInput);
    getValidInput("Enter comments: ", entry.comment, MAX_STRING, validateInput);
    getValidInput("Enter link (e.g., http:// or www.): ", entry.link, MAX_STRING, validateLink);

    // Write fields in the order: Title, Type, Author, Duration, Genre, Comment, Link
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n",
            entry.title, entry.type, entry.author,
            entry.duration, entry.genre, entry.comment, entry.link);
    fclose(fp);
    printf("Entry saved successfully!\n");
}

void editEntry(const char *filename) {
    struct MediaEntry entries[MAX_ENTRIES];
    int count = 0;
    char line[1024];

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file for reading");
        return;
    }

    // Expect CSV order: Title, Type, Author, Duration, Genre, Comment, Link
    while (fgets(line, sizeof(line), fp) && count < MAX_ENTRIES) {
        int fieldsFilled = sscanf(line,
            "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^\n]",
            entries[count].title, entries[count].type, entries[count].author,
            entries[count].duration, entries[count].genre, entries[count].comment, entries[count].link);
        if (fieldsFilled == 7) {
            count++;
        }
    }
    fclose(fp);

    if (count == 0) {
        printf("No entries found to edit.\n");
        return;
    }

    printf("\nExisting Entries:\n");
    for (int i = 0; i < count; i++) {
        // Format: Title [Type] by Author
        printf("%d. %s [%s] by %s\n", i + 1, entries[i].title, entries[i].type, entries[i].author);
    }
    printf("Enter the entry number to edit (0 to cancel): ");
    int choice;
    scanf("%d", &choice);
    getchar();  // Clear newline

    if (choice <= 0 || choice > count) {
        printf("Editing cancelled.\n");
        return;
    }

    int index = choice - 1;
    char buffer[MAX_STRING];

    printf("Press Enter without typing to keep the current value.\n");

    printf("Current Title: %s\n", entries[index].title);
    printf("Enter new title: ");
    if (fgets(buffer, MAX_STRING, stdin)) {
        strtok(buffer, "\n");
        if (validateInput(buffer))
            strncpy(entries[index].title, buffer, MAX_STRING);
    }

    printf("Current Type: %s\n", entries[index].type);
    printf("Enter new type (Movie, Book, Album, Show): ");
    if (fgets(buffer, MAX_STRING, stdin)) {
        strtok(buffer, "\n");
        if (validateMediaType(buffer))
            strncpy(entries[index].type, buffer, MAX_STRING);
    }

    printf("Current Author/Director/Artist: %s\n", entries[index].author);
    printf("Enter new author/director/artist: ");
    if (fgets(buffer, MAX_STRING, stdin)) {
        strtok(buffer, "\n");
        if (validateInput(buffer))
            strncpy(entries[index].author, buffer, MAX_STRING);
    }

    printf("Current Duration/Pages: %s\n", entries[index].duration);
    printf("Enter new duration/pages: ");
    if (fgets(buffer, MAX_STRING, stdin)) {
        strtok(buffer, "\n");
        if (validateInput(buffer))
            strncpy(entries[index].duration, buffer, MAX_STRING);
    }

    printf("Current Genre: %s\n", entries[index].genre);
    printf("Enter new genre: ");
    if (fgets(buffer, MAX_STRING, stdin)) {
        strtok(buffer, "\n");
        if (validateInput(buffer))
            strncpy(entries[index].genre, buffer, MAX_STRING);
    }

    printf("Current Comments: %s\n", entries[index].comment);
    printf("Enter new comments: ");
    if (fgets(buffer, MAX_STRING, stdin)) {
        strtok(buffer, "\n");
        if (validateInput(buffer))
            strncpy(entries[index].comment, buffer, MAX_STRING);
    }

    printf("Current Link: %s\n", entries[index].link);
    printf("Enter new link (e.g., http:// or www.): ");
    if (fgets(buffer, MAX_STRING, stdin)) {
        strtok(buffer, "\n");
        if (validateLink(buffer))
            strncpy(entries[index].link, buffer, MAX_STRING);
    }

    fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open file for writing");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n",
                entries[i].title, entries[i].type, entries[i].author,
                entries[i].duration, entries[i].genre, entries[i].comment, entries[i].link);
    }
    fclose(fp);
    printf("Entry updated successfully!\n");
}

void readEntries(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    int choice;
    printf("Choose the field to search by:\n");
    // Display field names according to CSV order
    printf("1. Title\n2. Type\n3. Author\n4. Duration/Pages\n5. Genre\n6. Comment\n7. Link\n");
    printf("Enter your choice (1-7): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Exiting reading mode.\n");
        fclose(fp);
        return;
    }
    getchar();  // Clear leftover newline

    if (choice < 1 || choice > 7) {
        printf("Invalid field choice. Exiting.\n");
        fclose(fp);
        return;
    }

    char searchTerm[MAX_STRING];
    printf("Enter the search term: ");
    if (!fgets(searchTerm, MAX_STRING, stdin)) {
         printf("Error reading search term. Exiting.\n");
         fclose(fp);
         return;
    }
    // Remove newline and trim extra spaces
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    char *start = searchTerm;
    while (isspace((unsigned char)*start))
        start++;
    char *end = start + strlen(start) - 1;
    while (end >= start && isspace((unsigned char)*end))
        *end-- = '\0';
    memmove(searchTerm, start, strlen(start) + 1);
    toLowerCase(searchTerm);

    int matchCount = 0;
    char line[1024];

    while (fgets(line, sizeof(line), fp) != NULL) {
        // Skip empty or very short lines
        if (strlen(line) < 2)
            continue;

        struct MediaEntry entry;
        int fieldsFilled = sscanf(line,
            "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^\n]",
            entry.title, entry.type, entry.author,
            entry.duration, entry.genre, entry.comment, entry.link);
        if (fieldsFilled != 7)
            continue;

        char fieldValue[MAX_STRING] = "";
        switch (choice) {
            case 1:
                strncpy(fieldValue, entry.title, MAX_STRING);
                break;
            case 2:
                strncpy(fieldValue, entry.type, MAX_STRING);
                break;
            case 3:
                strncpy(fieldValue, entry.author, MAX_STRING);
                break;
            case 4:
                strncpy(fieldValue, entry.duration, MAX_STRING);
                break;
            case 5:
                strncpy(fieldValue, entry.genre, MAX_STRING);
                break;
            case 6:
                strncpy(fieldValue, entry.comment, MAX_STRING);
                break;
            case 7:
                strncpy(fieldValue, entry.link, MAX_STRING);
                break;
        }
        fieldValue[MAX_STRING - 1] = '\0';
        toLowerCase(fieldValue);

        if (strstr(fieldValue, searchTerm) != NULL) {
            matchCount++;
            printf("\nMatch #%d:\n", matchCount);
            printf("Title: %s\nType: %s\nAuthor: %s\nDuration/Pages: %s\nGenre: %s\nComments: %s\nLink: %s\n",
                   entry.title, entry.type, entry.author, entry.duration,
                   entry.genre, entry.comment, entry.link);
            printf("---------------------------------------------\n");

            char option[16];
            while (1) {
                printf("Choose an action:\n");
                printf("1 - Read next matching record\n");
                printf("2 - Exit reading mode\n");
                printf("Your choice: ");
                if (!fgets(option, sizeof(option), stdin)) {
                    printf("Error reading input. Exiting reading mode.\n");
                    fclose(fp);
                    return;
                }
                option[strcspn(option, "\n")] = '\0';
                if (strcmp(option, "1") == 0) {
                    break;
                } else if (strcmp(option, "2") == 0) {
                    printf("Exiting reading mode. Thank you!\n");
                    fclose(fp);
                    return;
                } else {
                    printf("Invalid input. Please try again.\n");
                }
            }
        }
    }

    if (matchCount == 0)
        printf("No matching entries found.\n");
    else
        printf("You've reached the end of the matching entries.\n");

    fclose(fp);
}
