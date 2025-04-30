#include <stdio.h>
#include <string.h>
#include "media_manager.h"

// Main function to launch the Media Database Manager application
int main(int argc, char *argv[]) {
    // Default filename for the media database
    char filename[MAX_STRING] = "full_media_list.csv";

    // Allow the user to provide a filename as a command-line argument
    if (argc > 1) {
        strncpy(filename, argv[1], MAX_STRING);
        filename[MAX_STRING - 1] = '\0';
    } else {
        // Ask the user to input a file name or press Enter to use the default
        char temp[MAX_STRING];
        printf("Enter CSV file name (or press Enter for default '%s'): ", filename);
        if (fgets(temp, MAX_STRING, stdin)) {
            strtok(temp, "\n"); // Remove trailing newline
            if (validateInput(temp)) {
                strncpy(filename, temp, MAX_STRING);
                filename[MAX_STRING - 1] = '\0';
            }
        }
    }

    // Display main menu options
    int mode;
    printf("\nWelcome to the Media Database Manager\n");
    printf("Select mode:\n");
    printf("1. Saving Mode\n");
    printf("2. Reading Mode\n");
    printf("Enter your choice: ");
    scanf("%d", &mode);
    getchar();  // Clear leftover newline

    if (mode == 1) {
        // Submenu for saving-related operations
        int submode;
        printf("\nSaving Mode:\n");
        printf("1. Create New Entry\n");
        printf("2. Edit Existing Entry\n");
        printf("3. Delete Entry\n"); 
        printf("Enter your choice: ");
        scanf("%d", &submode);
        getchar();  // Clear leftover newline

        if (submode == 1) {
            saveEntry(filename);
        } else if (submode == 2) {
            editEntry(filename);
        } else if (submode == 3) {
            deleteEntry(filename);
        } else {
            printf("Invalid choice. Exiting.\n");
        }
    } else if (mode == 2) {
        // Start reading/searching media entries
        readEntries(filename);
    } else {
        printf("Invalid mode. Exiting.\n");
    }

    return 0;
}
