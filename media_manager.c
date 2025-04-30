#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "media_manager.h"

// Prompts the user until they provide valid input based on the given validation function.
void getValidInput(const char *prompt, char *buffer, int size, int (*validate)(const char *)) {
    do {
        printf("%s", prompt);
        if (fgets(buffer, size, stdin) == NULL) {
            buffer[0] = '\0'; // In case of input failure, clear buffer
            return;
        }
        strtok(buffer, "\n");  // Remove newline character
        if (!validate(buffer)) {
            printf("Invalid input, please try again.\n");
        }
    } while (!validate(buffer));
}

// Checks if the input is not just empty or made up of spaces
int validateInput(const char *input) {
    if (input == NULL) return 0;
    while (*input) {
        if (!isspace((unsigned char)*input))
            return 1;
        input++;
    }
    return 0;
}

// Validates that the media type is one of the allowed categories
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

// Checks if the link starts with http://, https://, or www.
int validateLink(const char *input) {
    if (!validateInput(input)) return 0;
    return (strncmp(input, "http://", 7) == 0 ||
            strncmp(input, "https://", 8) == 0 ||
            strncmp(input, "www.", 4) == 0);
}

// Converts a string to lowercase (used to make validation and comparison easier)
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Saves a new entry to the file by collecting user input and writing it in CSV format
void saveEntry(const char *filename) {
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    struct MediaEntry entry;

    // Get validated input for each field
    getValidInput("Enter title: ", entry.title, MAX_STRING, validateInput);
    getValidInput("Enter type (Movie, Book, Album, Show): ", entry.type, MAX_STRING, validateMediaType);
    getValidInput("Enter author/director/artist: ", entry.author, MAX_STRING, validateInput);
    getValidInput("Enter duration/pages: ", entry.duration, MAX_STRING, validateInput);
    getValidInput("Enter genre: ", entry.genre, MAX_STRING, validateInput);
    getValidInput("Enter comments: ", entry.comment, MAX_STRING, validateInput);
    getValidInput("Enter link (e.g., http:// or www.): ", entry.link, MAX_STRING, validateLink);

    // Write the entry to file in CSV format
    fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n",
            entry.title, entry.type, entry.author,
            entry.duration, entry.genre, entry.comment, entry.link);
    fclose(fp);
    printf("Entry saved successfully!\n");
}

// Edits an existing entry based on user selection and input
void editEntry(const char *filename) {
    struct MediaEntry entries[MAX_ENTRIES];
    int count = 0;
    char line[1024];

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file for reading");
        return;
    }

    // Load all entries from file into memory
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

    // Display all entries to let the user choose one to edit
    printf("\nExisting Entries:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s [%s] by %s\n", i + 1, entries[i].title, entries[i].type, entries[i].author);
    }
    printf("Enter the entry number to edit (0 to cancel): ");
    int choice;
    scanf("%d", &choice);
    getchar();  // Clear leftover newline character

    if (choice <= 0 || choice > count) {
        printf("Editing cancelled.\n");
        return;
    }

    int index = choice - 1;
    char buffer[MAX_STRING];

    // Allow user to update each field, or press Enter to skip
    printf("Press Enter without typing to keep the current value.\n");

    #define UPDATE_FIELD(promptText, currentValue, validateFunc) do { \
        printf("Current %s: %s\n", promptText, currentValue); \
        printf("Enter new %s: ", promptText); \
        if (fgets(buffer, MAX_STRING, stdin)) { \
            strtok(buffer, "\n"); \
            if (validateFunc(buffer)) \
                strncpy(currentValue, buffer, MAX_STRING); \
        } \
    } while(0)

    UPDATE_FIELD("title", entries[index].title, validateInput);
    UPDATE_FIELD("type", entries[index].type, validateMediaType);
    UPDATE_FIELD("author/director/artist", entries[index].author, validateInput);
    UPDATE_FIELD("duration/pages", entries[index].duration, validateInput);
    UPDATE_FIELD("genre", entries[index].genre, validateInput);
    UPDATE_FIELD("comments", entries[index].comment, validateInput);
    UPDATE_FIELD("link", entries[index].link, validateLink);

    // Overwrite the file with updated entries
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

// Reads entries from the file and allows the user to search by a specific field
void readEntries(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return;
    }

    int choice;
    // Prompt user to choose a field to search by
    printf("Choose the field to search by:\n");
    printf("1. Title\n2. Type\n3. Author\n4. Duration/Pages\n5. Genre\n6. Comment\n7. Link\n");
    printf("Enter your choice (1-7): ");
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input. Exiting reading mode.\n");
        fclose(fp);
        return;
    }
    getchar();  // Remove leftover newline

    if (choice < 1 || choice > 7) {
        printf("Invalid field choice. Exiting.\n");
        fclose(fp);
        return;
    }

    // Ask for search term and clean it
    char searchTerm[MAX_STRING];
    printf("Enter the search term: ");
    if (!fgets(searchTerm, MAX_STRING, stdin)) {
        printf("Error reading search term. Exiting.\n");
        fclose(fp);
        return;
    }
    searchTerm[strcspn(searchTerm, "\n")] = '\0';
    char *start = searchTerm;
    while (isspace((unsigned char)*start)) start++;
    char *end = start + strlen(start) - 1;
    while (end >= start && isspace((unsigned char)*end)) *end-- = '\0';
    memmove(searchTerm, start, strlen(start) + 1);
    toLowerCase(searchTerm);

    int matchCount = 0;
    char line[1024];

    // Read each entry and check if the selected field contains the search term
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strlen(line) < 2) continue;

        struct MediaEntry entry;
        int fieldsFilled = sscanf(line,
            "%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^,],%255[^\n]",
            entry.title, entry.type, entry.author,
            entry.duration, entry.genre, entry.comment, entry.link);
        if (fieldsFilled != 7) continue;

        char fieldValue[MAX_STRING] = "";
        switch (choice) {
            case 1: strncpy(fieldValue, entry.title, MAX_STRING); break;
            case 2: strncpy(fieldValue, entry.type, MAX_STRING); break;
            case 3: strncpy(fieldValue, entry.author, MAX_STRING); break;
            case 4: strncpy(fieldValue, entry.duration, MAX_STRING); break;
            case 5: strncpy(fieldValue, entry.genre, MAX_STRING); break;
            case 6: strncpy(fieldValue, entry.comment, MAX_STRING); break;
            case 7: strncpy(fieldValue, entry.link, MAX_STRING); break;
        }

        toLowerCase(fieldValue);

        if (strstr(fieldValue, searchTerm)) {
            matchCount++;
            printf("\nMatch #%d:\n", matchCount);
            printf("Title: %s\nType: %s\nAuthor: %s\nDuration/Pages: %s\nGenre: %s\nComments: %s\nLink: %s\n",
                   entry.title, entry.type, entry.author, entry.duration,
                   entry.genre, entry.comment, entry.link);
            printf("---------------------------------------------\n");

            // Let user decide whether to keep searching
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
                if (strcmp(option, "1") == 0) break;
                else if (strcmp(option, "2") == 0) {
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
void deleteEntry(const char *filename) {
    struct MediaEntry entries[MAX_ENTRIES];
    int count = 0;
    char line[1024];

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file for reading");
        return;
    }

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
        printf("No entries found to delete.\n");
        return;
    }

    printf("\nExisting Entries:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s [%s] by %s\n", i + 1, entries[i].title, entries[i].type, entries[i].author);
    }

    printf("Enter the entry number to delete (0 to cancel): ");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice <= 0 || choice > count) {
        printf("Deletion cancelled.\n");
        return;
    }

    int indexToDelete = choice - 1;

    printf("Are you sure you want to delete '%s' by %s? (y/n): ", entries[indexToDelete].title, entries[indexToDelete].author);
    char confirm[4];
    fgets(confirm, sizeof(confirm), stdin);
    if (tolower(confirm[0]) != 'y') {
        printf("Deletion cancelled.\n");
        return;
    }

    fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to open file for writing");
        return;
    }

    for (int i = 0; i < count; i++) {
        if (i == indexToDelete) continue;
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n",
                entries[i].title, entries[i].type, entries[i].author,
                entries[i].duration, entries[i].genre, entries[i].comment, entries[i].link);
    }
    fclose(fp);
    printf("Entry deleted successfully.\n");
}
