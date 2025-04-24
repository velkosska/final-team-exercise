# Media Database Manager (C Console Project)

**Team Members**: Marija Velkoska, Dana Dabbah, Kristina Mamalashvili  
**Instructor**: Miguel Odin Soler Rus  
**Date**: April 2025

---

## Overview

Our program is a simple console-based **Media Database Manager** written in C. It allows users to:

- Add a new media entry (Movie, Book, Album, Show) to a CSV file
- Search existing entries by various fields
- Edit existing entries
- Choose the filename to use via command-line or user prompt

---

## Functionality Breakdown

### User Interaction and Menu

On startup, the program offers two main modes:

1. **Saving Mode**
   - Add new media entry
   - Edit existing entry
2. **Reading Mode**
   - Search entries by a selected field

String inputs are handled using `fgets` for safety, and menu selections use `scanf`.

---

### Data Storage

- Media entries are stored in a CSV file (`full_media_list.csv` by default).
- Each record includes the following fields:
  - Title
  - Type
  - Author/Director/Artist
  - Duration/Pages
  - Genre
  - Comments
  - Link
- A C struct `MediaEntry` holds these fields, matching the CSV layout for ease of parsing.

---

### Adding New Entries

- The `saveEntry()` function collects validated input from the user.
- Inputs are stored safely with multi-word support and written in CSV format.
- File errors (e.g., if the file can't be opened) are handled using `perror`.

---

### Searching Entries

- Users can search by any of the 7 fields.
- Searches are case-insensitive.
- The user is shown matches one-by-one and can choose whether to continue or exit.

---

## Additional Enhancements

### Editing Existing Entries

- Loads all entries from the file into memory.
- Displays entries with index numbers.
- Allows users to update any field, or skip updates by pressing Enter.
- Updates are saved by rewriting the CSV.

### File Naming Flexibility

- Accepts CSV filename as a command-line argument.
- If no filename is provided, users are prompted to input one or use the default.

### Advanced Input Validation

- Ensures inputs are non-empty.
- Media type must be one of: Movie, Book, Album, Show.
- Link must start with `http://`, `https://`, or `www.`

---

## Justification of Design Choices

### CSV for Storage

- Human-readable and editable.
- Compatible with Excel, Google Sheets, etc.
- No need for additional libraries.

### Fixed-Size Buffers

- Uses `MAX_STRING = 256` to avoid dynamic memory complexity.
- Simplifies management for a small-scale project.

### Modular Design

- Functions like `saveEntry`, `editEntry`, and `readEntries` are separated.
- Improves readability and reusability.

### Safe String Handling

- Uses `fgets` instead of `scanf` for strings to prevent buffer overflows.
- Cleans newline characters using `strtok`.

### Case-Insensitive Search

- Both input and field values are converted to lowercase.
- Ensures consistent matching.

### Minimal Error Handling

- File operations report issues immediately.
- User input is validated to avoid crashes or logic errors.

---

## Testing & Edge Cases

The program was tested with:

- Valid and invalid media types and links
- Empty input
- Overly long strings
- Invalid menu choices

The menu and validation logic handled these edge cases gracefully.

---

## Limitations

- Uses fixed-size memory buffers, limiting scalability.
- Assumes single-user access; no concurrency or locking implemented.

---

## Future Improvements

- Add delete functionality
- Sort/filter by genre or author
- Export search results to a new CSV file
- Use dynamic memory for better scalability

---

## Conclusion

The Media Database Manager provides a clear example of managing structured data in C using file I/O and validation techniques. Its simplicity, modular design, and extra features (like editing and flexible file usage) make it a strong beginner-friendly project that also demonstrates good programming practices and teamwork.
