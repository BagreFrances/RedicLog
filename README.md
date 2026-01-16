# Log File Monitor and Redirector

This C++ program monitors a specified log file for any modifications, reads the last line from the file, and optionally redirects it to another file. The program also supports filtering the content using a regular expression (regex).

## Features

- Monitors a specified target file for modifications using `inotify`.
- Reads the last line of the target file when it's modified.
- Optionally filters the content using a regular expression before redirecting it to a log file.
- Redirection to a different log file based on user input.
- Supports command-line arguments for customization.

## Prerequisites

This program requires the following:

- A Linux environment (since it uses `inotify`).
- The `g++` compiler (or another C++ compiler) to compile the program.

## Compilation

To compile the program, run the following command:

```bash
g++ -o LogFileMonitor LogFileMonitor.cpp
