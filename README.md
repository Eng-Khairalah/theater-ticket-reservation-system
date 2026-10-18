# Theater Ticket Reservation System 🎭

A console-based theater seat reservation application built in **C**. This academic project demonstrates functions, arrays, input validation and file I/O.

## Features

- **15 seats**, divided into VIP (1–5), Category 1 (6–10) and Category 2 (11–15)
- View availability and reserve a seat in its category
- Reject invalid menu choices, seat numbers and duplicate bookings
- Save reservations in a local `seats.txt` file to persist across runs
- **No customer names, ID numbers or other personal information collected**

## Run on Windows (PowerShell)

Install GCC (for example MSYS2 UCRT64), open a terminal in the repository root and run:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c -o theater.exe
.\theater.exe
```

If GCC is installed but not available in this PowerShell session, first run `$env:Path = "C:\msys64\ucrt64\bin;" + $env:Path`.

On Linux or macOS with GCC:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic src/main.c -o theater
./theater
```

Run from the repository root each time because the program reads and writes `seats.txt` in the **current working directory**.

## Example

```text
=== Theater Ticket Reservation System ===
1. VIP (seats 1-5)
2. Category 1 (seats 6-10)
3. Category 2 (seats 11-15)
4. View all seats
5. Exit
Choose an option: 1
Choose a seat (1-5): 2

Reservation confirmed!
Category: VIP
Seat: 2
```

## Project structure

```text
├── src/main.c             # C application
├── tests/smoke_test.py    # Automated CLI smoke tests
├── .gitignore             # Excludes local seat records and binaries
└── README.md
```

## Test

With Python and GCC installed:

```bash
python tests/smoke_test.py
```

Note: The automated test uses a Unix-style executable name. On Windows, run the compiled program manually using the commands above; you can test a booking, restart the application, and try booking the same seat again.

## Limitations

- Supports one running instance at a time; there is no database or file locking.
- No authentication, payments, cancellations or pricing.
- Reservation data is stored locally, not shared across devices.
- On some Windows configurations, replacing an existing `seats.txt` with `rename()` may fail. The application reports that error rather than confirming an unsaved booking.
- This version deliberately excludes the original customer records and compiled executables.

## Author

**Khairallah Abdullah** — Software Engineering student, Atatürk University · [GitHub](https://github.com/Eng-Khairalah)

*License not yet selected. All rights reserved unless otherwise stated.*
