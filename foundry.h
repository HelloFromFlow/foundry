#ifndef FOUNDRY_H
#define FOUNDRY_H


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


typedef struct {
    char* appName;
    char* filepath;
} foundryApplicationData;


static inline foundryApplicationData foundryRegisterApplication(char* appName, char* filepath) {
    static foundryApplicationData data = {"n/a", "n/a"};

    if (appName == NULL && filepath == NULL && strcmp(data.appName, "n/a") != 0 && strcmp(data.filepath, "n/a") != 0) {
        return data;
    }

    if (appName != NULL) data.appName = appName;

    if (filepath != NULL) data.filepath = filepath;

    return data;
}

static inline bool foundryLog(const char* topic, const char* description) {
    foundryApplicationData appData = foundryRegisterApplication(NULL, NULL);

    if (!strcmp(appData.appName, "n/a") || !strcmp(appData.filepath, "n/a")) return false;

    FILE* file = fopen(appData.filepath, "a");
    if (file == NULL) return false;

    time_t unix_time;
    struct tm* formattable_time;
    char time_buf[100];

    time(&unix_time);
    
    formattable_time = localtime(&unix_time);
    strftime(time_buf, sizeof(time_buf), "%Y.%m.%d %H:%M:%S", formattable_time);

    fprintf(file, "%s [%s]: %s {%s}\n", appData.appName, topic, description, time_buf);

    fclose(file);

    return true;
}

static inline bool foundyClean() {
    foundryApplicationData appData = foundryRegisterApplication(NULL, NULL);

    if (!strcmp(appData.appName, "n/a") || !strcmp(appData.filepath, "n/a")) return false;

    FILE* rfile = fopen(appData.filepath, "r");
    if (rfile == NULL) return false;

    char **text = NULL;
    size_t lineCount = 0;

    char buf[512];

    while (fgets(buf, sizeof(buf), rfile)) {
        char** tmp = realloc(text, (lineCount + 1) * sizeof(char*));

        if (tmp == NULL) {
            fclose(rfile);
            for (size_t i = 0; i < lineCount; i++) free(text[i]);
            free(text);

            return false;
        }
        else {
            text = tmp;
        }

        text[lineCount] = malloc(strlen(buf) + 1);

        if (text[lineCount] == NULL) {
            fclose(rfile);
            for (size_t i = 0; i < lineCount; i++) free(text[i]);
            free(text);

            return false;
        }

        strcpy(text[lineCount], buf);

        lineCount++;
    }

    fclose(rfile);

    for (size_t i = 0; i < lineCount; i++) {
        if (!strncmp(text[i], appData.appName, strlen(appData.appName))) {
            free(text[i]);
            text[i] = NULL;
        }
    }

    FILE* wfile = fopen(appData.filepath, "w");

    if (wfile == NULL) {
        for (size_t i = 0; i < lineCount; i++) {
            free(text[i]);
        }

        free(text);

        return false;
    }

    for (size_t i = 0; i < lineCount; i++) {
        if (text[i] != NULL) {
            fprintf(wfile, "%s", text[i]);
        }
    }

    fclose(wfile);

    for (size_t i = 0; i < lineCount; i++) {
        free(text[i]);
    }

    free(text);

    return true;
}

#endif