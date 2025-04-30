// media_manager.h
// Header file for managing a media library with basic operations like saving, editing, and reading entries.
// Each media entry contains metadata such as type, title, author, duration, genre, comment, and link.

#ifndef MEDIA_MANAGER_H
#define MEDIA_MANAGER_H

// Maximum number of media entries the system can store
#define MAX_ENTRIES 1000

// Maximum length for each string field (e.g., title, author, etc.)
#define MAX_STRING 256

// Structure representing a single media entry in the library.
// Note: In the CSV file, the order will be: Title, Type, Author, Duration, Genre, Comment, Link.
struct MediaEntry {
    char type[MAX_STRING];     // e.g., Book, Movie, Podcast
    char title[MAX_STRING];    // Title of the media
    char author[MAX_STRING];   // Creator or author of the media
    char duration[MAX_STRING]; // Length of the media (optional for some types)
    char genre[MAX_STRING];    // Genre or category
    char comment[MAX_STRING];  // Personal comment or note
    char link[MAX_STRING];     // URL to the media or additional info
};

// Saves a new media entry to the file (appends to the CSV file)
void saveEntry(const char *filename);

// Allows editing an existing media entry in the file
void editEntry(const char *filename);

// Reads and displays all media entries from the file
void readEntries(const char *filename);

// Validates general user input (e.g., checks if it's not empty)
int validateInput(const char *input);

// Validates the type field (ensures it's a known/allowed media type)
int validateMediaType(const char *input);

// Validates the link field (basic check for a valid URL format)
int validateLink(const char *input);

// Converts all characters in a string to lowercase (useful for comparisons)
void toLowerCase(char *str);

// Prompts the user for input, validates it using the provided function, and stores it in buffer
void getValidInput(const char *prompt, char *buffer, int size, int (*validate)(const char *));

//Deletion function

void deleteEntry(const char *filename);

#endif
