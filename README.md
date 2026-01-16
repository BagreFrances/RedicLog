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
- The `g++` compiler (or another C++ compiler) > c++11 to compile the program.

## Compilation

To compile the program, run the following command:

```bash
g++ -std=c++17 main.cpp -o RedicLog

```

## Usage

Once compiled, you can run the program with the following options:
```bash
./LogFileMonitor -t <target_file> -d <redirect_file> -r <regex>
```
## Examples
### Capture IP 
´´´bash
./LogFileMonitor -t /var/log/auth.log -d redirected_log.txt -r  "(\b25[0-5]|\b2[0-4][0-9]|\b[01]?[0-9][0-9]?)(\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3}"
```

