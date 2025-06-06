#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SENSORS  10

typedef struct {
    int id;
    int threshold;
    int reps;
    int run;
} Sensor;

// Your sensor-handling function
void process_sensor(int id, int threshold, int reps, int run) {
    printf("Id: %d: threshold=%d, reps=%d, run=%d\n",
           id, threshold, reps, run);
}

// Read entire file into a malloc’d buffer (caller must free)
char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { perror(path); return NULL; }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    char *buf = malloc(len + 1);
    if (!buf) { perror("malloc"); fclose(f); return NULL; }
    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);
    return buf;
}

// Skip forward until you hit a digit or minus sign
char *skip_to_number(char *p) {
    while (*p && !isdigit((unsigned char)*p) && *p != '-') p++;
    return p;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <json_file>\n", argv[0]);
        return 1;
    }

    char *json = read_file(argv[1]);
    if (!json) return 1;

    Sensor sensors[MAX_SENSORS];
    int count = 0;
    char *cursor = json;

    // Find each "id" and parse the block of four fields
    while (count < MAX_SENSORS && (cursor = strstr(cursor, "\"id\"")) != NULL) {
        char *p;

        // id
        p = strchr(cursor, ':');  if (!p) break;
        p = skip_to_number(p);
        sensors[count].id = strtol(p, NULL, 10);

        // threshold
        cursor = p;
        cursor = strstr(cursor, "\"threshold\"");  if (!cursor) break;
        p = strchr(cursor, ':');  if (!p) break;
        p = skip_to_number(p);
        sensors[count].threshold = strtol(p, NULL, 10);

        // reps
        cursor = p;
        cursor = strstr(cursor, "\"reps\"");  if (!cursor) break;
        p = strchr(cursor, ':');  if (!p) break;
        p = skip_to_number(p);
        sensors[count].reps = strtol(p, NULL, 10);

        // run
        cursor = p;
        cursor = strstr(cursor, "\"run\"");  if (!cursor) break;
        p = strchr(cursor, ':');  if (!p) break;
        p = skip_to_number(p);
        sensors[count].run = strtol(p, NULL, 10);

        count++;
        cursor = p;
    }

    free(json);

    // Dispatch to your handler
    for (int i = 0; i < count; i++) {
        process_sensor(
            sensors[i].id,
            sensors[i].threshold,
            sensors[i].reps,
            sensors[i].run
        );
    }

    return 0;
}
