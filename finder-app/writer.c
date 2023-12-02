#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]) {
    openlog("writer", LOG_PID | LOG_PERROR, LOG_USER);

    if (argc != 3) {
        syslog(LOG_ERR, "Error: Insufficient arguments. Please provide <file> and <string>.");
        closelog();
        exit(EXIT_FAILURE);
    }

    const char *file_path = argv[1];
    const char *write_str = argv[2];

    FILE *file = fopen(file_path, "w");

    if (file == NULL) {
        syslog(LOG_ERR, "Error: Unable to open file %s for writing.", file_path);
        closelog();
        exit(EXIT_FAILURE);
    }

    // Ensure write_str is not NULL before attempting to write
    if (write_str != NULL) {
        fprintf(file, "%s", write_str);
        syslog(LOG_DEBUG, "Writing \"%s\" to \"%s\"", write_str, file_path);
    } else {
        syslog(LOG_ERR, "Error: Attempted to write NULL string to file %s.", file_path);
    }

    fclose(file);

    closelog();
    return EXIT_SUCCESS;
}
