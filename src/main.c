#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SEAT_COUNT 15
#define SEATS_PER_CATEGORY 5
#define INPUT_SIZE 128
#define DATA_FILE "seats.txt"

static int read_number(const char *prompt, int min, int max) {
    char line[INPUT_SIZE], *end;
    long value;
    for (;;) {
        printf("%s", prompt);
        if (!fgets(line, sizeof line, stdin)) return -1;
        if (!strchr(line, '\n') && !feof(stdin)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            puts("Input too long. Try again.");
            continue;
        }
        errno = 0;
        value = strtol(line, &end, 10);
        if (end == line || errno == ERANGE) {
            puts("Please enter a valid number.");
            continue;
        }
        while (*end == ' ' || *end == '\t' || *end == '\r') end++;
        if (*end != '\n' && *end != '\0') {
            puts("Please enter a valid number.");
            continue;
        }
        if (value < min || value > max) {
            printf("Please enter a number from %d to %d.\n", min, max);
            continue;
        }
        return (int)value;
    }
}

static const char *category_name(int category) {
    switch (category) {
        case 1: return "VIP";
        case 2: return "Category 1";
        case 3: return "Category 2";
        default: return "Unknown";
    }
}

/* Missing file means a fresh installation. Invalid data is rejected. */
static int load_seats(int seats[SEAT_COUNT]) {
    FILE *file = fopen(DATA_FILE, "r");
    int value;
    if (!file) {
        if (errno != ENOENT) {
            perror("Could not open seat data");
            return 0;
        }
        return 1;
    }
    for (int i = 0; i < SEAT_COUNT; i++) {
        if (fscanf(file, "%d", &value) != 1 || (value != 0 && value != 1)) {
            fprintf(stderr, "Invalid seat data in %s.\n", DATA_FILE);
            fclose(file);
            return 0;
        }
        seats[i] = value;
    }
    if (fscanf(file, "%d", &value) == 1) {
        fprintf(stderr, "Unexpected extra seat data in %s.\n", DATA_FILE);
        fclose(file);
        return 0;
    }
    if (ferror(file)) {
        perror("Error reading seat data");
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

/* Write to a temporary file, then rename to avoid partially written data. */
static int save_seats(const int seats[SEAT_COUNT]) {
    const char *temp_file = "seats.tmp";
    FILE *file = fopen(temp_file, "w");
    if (!file) { perror("Could not create seat data"); return 0; }
    for (int i = 0; i < SEAT_COUNT; i++) {
        if (fprintf(file, "%d\n", seats[i]) < 0) {
            perror("Could not write seat data");
            fclose(file);
            remove(temp_file);
            return 0;
        }
    }
    if (fclose(file) != 0) {
        perror("Could not close seat data");
        remove(temp_file);
        return 0;
    }
    if (rename(temp_file, DATA_FILE) != 0) {
        perror("Could not replace seat data");
        remove(temp_file);
        return 0;
    }
    return 1;
}

static void show_seats(const int seats[SEAT_COUNT]) {
    puts("\nSeat availability (O = available, X = reserved):");
    for (int i = 0; i < SEAT_COUNT; i++) {
        printf("%2d [%c]  ", i + 1, seats[i] ? 'X' : 'O');
        if ((i + 1) % SEATS_PER_CATEGORY == 0) {
            printf("  %s\n", category_name(i / SEATS_PER_CATEGORY + 1));
        }
    }
}

int main(void) {
    int seats[SEAT_COUNT] = {0};
    if (!load_seats(seats)) return EXIT_FAILURE;
    puts("=== Theater Ticket Reservation System ===");
    for (;;) {
        puts("\n1. VIP (seats 1-5)\n2. Category 1 (seats 6-10)\n3. Category 2 (seats 11-15)\n4. View all seats\n5. Exit");
        int option = read_number("Choose an option: ", 1, 5);
        if (option == -1 || option == 5) break;
        if (option == 4) { show_seats(seats); continue; }

        int start = (option - 1) * SEATS_PER_CATEGORY + 1;
        int end = start + SEATS_PER_CATEGORY - 1;
        int available = 0;
        for (int i = start - 1; i < end; i++) available += !seats[i];
        if (!available) {
            printf("Sorry, %s is fully booked.\n", category_name(option));
            continue;
        }
        show_seats(seats);
        char prompt[INPUT_SIZE];
        snprintf(prompt, sizeof prompt, "Choose a seat (%d-%d): ", start, end);
        int seat = read_number(prompt, start, end);
        if (seat == -1) break;
        if (seats[seat - 1]) {
            puts("That seat is already reserved. Please try again.");
            continue;
        }
        seats[seat - 1] = 1;
        if (!save_seats(seats)) {
            seats[seat - 1] = 0;
            fputs("Reservation not saved. No booking was confirmed.\n", stderr);
            return EXIT_FAILURE;
        }
        printf("\nReservation confirmed!\nCategory: %s\nSeat: %d\n", category_name(option), seat);
        puts("No personal identification data is collected or stored.");
    }
    puts("Goodbye!");
    return EXIT_SUCCESS;
}
