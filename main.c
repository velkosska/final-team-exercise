// main.c

#include <stdio.h>
#include <string.h>
#include "media_manager.h"

int main(int argc, char *argv[]) {
    char filename[MAX_STRING] = "full_media_list.csv";

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

    int mode;
    printf("\nWelcome to the Media Database Manager\n");
    printf("Select mode:\n1. Saving Mode\n2. Reading Mode\nEnter your choice: ");
    scanf("%d", &mode);
    getchar();

    if (mode == 1) {
        int submode;
        printf("\nSaving Mode:\n1. Create New Entry\n2. Edit Existing Entry\nEnter your choice: ");
        scanf("%d", &submode);
        getchar();

        if (submode == 1) {
            saveEntry(filename);
        } else if (submode == 2) {
            editEntry(filename);
        } else {
            printf("Invalid choice. Exiting.\n");
        }
    } else if (mode == 2) {
        readEntries(filename);
    } else {
        printf("Invalid mode. Exiting.\n");
    }

    return 0;
}
