// media_manager.h

#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

#define MAX_ENTRIES 1000
#define MAX_STRING 256

// Although the struct fields remain in the same order in memory,
// the CSV file order will be: Title, Type, Author, Duration, Genre, Comment, Link.
struct MediaEntry {
    char type[MAX_STRING];
    char title[MAX_STRING];
    char author[MAX_STRING];
    char duration[MAX_STRING];
    char genre[MAX_STRING];
    char comment[MAX_STRING];
    char link[MAX_STRING];
};

void saveEntry(const char *filename);
void editEntry(const char *filename);
void readEntries(const char *filename);
int validateInput(const char *input);
int validateMediaType(const char *input);
int validateLink(const char *input);
void toLowerCase(char *str);
void getValidInput(const char *prompt, char *buffer, int size, int (*validate)(const char *));

#endif
