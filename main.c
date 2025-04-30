#include <stdio.h>
#include <string.h>
#include "media_manager.h"

int main(int argc, char *argv[]) {
    char filename[MAX_STRING] = "full_media_list.csv";

    // Allow user to set filename via command-line or prompt
    if (argc > 1) {
        strncpy(filename, argv[1], MAX_STRING);
        filename[MAX_STRING - 1] = '\0';
    } else {
        char temp[MAX_STRING];
        printf("Enter CSV file name (or press Enter for default '%s'): ", filename);
        if (fgets(temp, MAX_STRING, stdin)) {
            strtok(temp, "\n");
            if (validateInput(temp)) {
                strncpy(filename, temp, MAX_STRING);
                filename[MAX_STRING - 1] = '\0';
            }
        }
    }

    while (1) {
        int mode;
        printf("\n=== Media Database Manager ===\n");
        printf("1. Saving Mode (Create, Edit, Delete)\n");
        printf("2. Reading Mode (Search & View)\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &mode) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        getchar();  // Clear newline

        if (mode == 1) {
            int submode;
            printf("\n--- Saving Mode ---\n");
            printf("1. Create New Entry\n");
            printf("2. Edit Existing Entry\n");
            printf("3. Delete Entry\n");
            printf("4. Return to Main Menu\n");
            printf("Enter your choice: ");
            scanf("%d", &submode);
            getchar();

            if (submode == 1) {
                saveEntry(filename);
            } else if (submode == 2) {
                editEntry(filename);
            } else if (submode == 3) {
                deleteEntry(filename);
            } else if (submode == 4) {
                continue;  // Return to main menu
            } else {
                printf("Invalid choice. Returning to main menu.\n");
            }

        } else if (mode == 2) {
            readEntries(filename);
        } else if (mode == 3) {
            printf("Exiting program. Goodbye!\n");
            break;
        } else {
            printf("Invalid mode. Try again.\n");
        }
    }

    return 0;
}
