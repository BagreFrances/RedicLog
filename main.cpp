
#include <cstring>
#include <errno.h>
#include <features.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <sys/inotify.h>
#include <unistd.h>

bool _d = false;
bool _t = false;
bool _r = false;

// File paths and data
const char *target;
const char *redirect;
const char *regex_;

// Help function
void help_p() {
  std::cout << "Usage: program -t <target> -d <redirect> -r <regex>\n";
  std::cout << "-t <target>: File to be monitored.\n";
  std::cout << "-d <redirect>: Log file to redirect output.\n";
  std::cout << "-r <regex>: Regular expression to filter the content.\n";
}

// Function to extract arguments
void ExtractArgs(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') { // Check if it's an option
      switch (argv[i][1]) {
      case 't':
        _t = true;
        if (i + 1 < argc)
          target = argv[++i];
        break;
      case 'd':
        _d = true;
        if (i + 1 < argc)
          redirect = argv[++i];
        break;
      case 'r':
        _r = true;
        if (i + 1 < argc)
          regex_ = argv[++i];
        break;
      default:
        help_p();
        exit(1);
      }
    }
  }
}

// Function to read the last line of a file
std::string lerUltimaLinha(const std::string &nomeArquivo) {
  std::ifstream file(nomeArquivo,
                     std::ios::in | std::ios::binary | std::ios::ate);
  if (!file.is_open())
    return ""; // File could not be opened

  std::streamoff pos = file.tellg(); // Position at the end of the file
  if (pos == 0)
    return ""; // Empty file

  std::string linha;
  char ch;

  // Ignore any newline at the end of the file
  file.seekg(pos - 1);
  file.get(ch);
  if (ch == '\n' || ch == '\r')
    pos--;

  // Read the file backwards to find the last line
  while (pos > 0) {
    file.seekg(pos - 1);
    file.get(ch);
    if (ch == '\n')
      break;            // Beginning of the last line found
    linha = ch + linha; // Add character at the beginning
    pos--;
  }

  // Check if the first line does not end with a newline
  if (pos == 0) {
    file.seekg(0);
    getline(file, linha);
  }

  return linha;
}

// Function to monitor if the file was modified
bool arquivoModificado(const std::string &arquivo) {
  int fd = inotify_init();
  if (fd < 0) {
    std::cerr << "Error initializing inotify: " << strerror(errno) << std::endl;
    return false;
  }

  int wd = inotify_add_watch(fd, arquivo.c_str(), IN_MODIFY);
  if (wd == -1) {
    std::cerr << "Error adding watch: " << strerror(errno) << std::endl;
    close(fd);
    return false;
  }

  char buffer[1024];
  int length = read(fd, buffer, sizeof(buffer)); // Block until an event occurs
  if (length < 0) {
    std::cerr << "Error reading event: " << strerror(errno) << std::endl;
    close(fd);
    return false;
  }

  struct inotify_event *event =
      reinterpret_cast<struct inotify_event *>(buffer);
  bool modificado = (event->mask & IN_MODIFY);
  if (modificado) {
    std::cout << "File " << arquivo << " has been modified!" << std::endl;
  }

  close(fd); // Close the descriptor after the event
  return modificado;
}

// Function to redirect logs based on regex
void RedirectLog(std::string red) {
  std::ofstream outFile(redirect, std::ios::app);

  if (!outFile.is_open()) {
    perror("Error opening log file");
    exit(1);
  }

  if (_r) {
    std::regex query1(regex_);
    if (std::regex_search(red, query1)) {
      outFile << red << '\n';
    }
  } else {
    outFile << red << '\n';
  }
}

// Debug function for parameters
void debug_t() {
  std::cout << "-t : " << target << std::endl;
  std::cout << "-d : " << redirect << std::endl;
  std::cout << "-r : " << regex_ << std::endl;
}

// Main function
int main(int argc, char **argv) {
  if (argc > 3) {
    ExtractArgs(argc, argv);

    // File monitoring loop
    while (true) {
      if (arquivoModificado(target)) {
        RedirectLog(lerUltimaLinha(target));
      }
      usleep(100000); // Sleep for 0.1 second to prevent high CPU usage
    }
    return 0;
  }
  help_p();
  return 1;
}
