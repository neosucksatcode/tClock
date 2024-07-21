#include <stdio.h>
#include <time.h>

#ifdef _WIN64
#include <windows.h>
#elif _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <unistd.h>
#endif

unsigned long long time_m() {
#ifdef _WIN64
  return GetTickCount64();
#elif _WIN32
  return GetTickCount();
#elif __linux__
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
#elif __APPLE__
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts); // Use CLOCK_MONOTONIC
  return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
#endif
}

void GetCurrentTimeFormatted(char *buffer, size_t bufferSize) {
#ifdef _WIN32
  SYSTEMTIME st;
  GetLocalTime(&st);
  snprintf(buffer, bufferSize, "%02d:%02d:%02d", st.wHour, st.wMinute,
           st.wSecond);
#else
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  snprintf(buffer, bufferSize, "%02d:%02d:%02d", timeinfo->tm_hour,
           timeinfo->tm_min, timeinfo->tm_sec);
#endif
}
// Function to enable ANSI escape codes in Windows CMD and PowerShell
void EnableANSI() {
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) {
    puts("Error getting standard output handle.");
    return;
  }

  DWORD dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode)) {
    printf("Error getting console mode.");
    return;
  }

  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(hOut, dwMode)) {
    printf("Error setting console mode.");
    return;
  }
}

int main() {
  EnableANSI();       // Enable ANSI escape codes
  char timeBuffer[9]; // Buffer to hold the formatted time

  puts("\033[1m"
       "\033[32m"); // Set terminal text color to green
  for (;;) {
#ifdef _WIN32
    Sleep(1000); // Sleep for 1 second
#else
    sleep(1); // Sleep for 1 second
#endif

    GetCurrentTimeFormatted(timeBuffer, sizeof(timeBuffer));
    printf("\r%s", timeBuffer); // Print the current time
    fflush(stdout);             // Ensure the output is displayed immediately
  }
  return 0;
}
