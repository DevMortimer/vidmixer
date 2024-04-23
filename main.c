#include <raylib.h>
#include <string.h>

#define MAX_FILEPATH_RECORDED 4096
#define MAX_FILEPATH_SIZE 2048

char *findFormat(const char *text);
int stringContains(char **array, int length, const char *target);

int main(void) {
  InitWindow(840, 480, "vidmixer");
  SetTargetFPS(60);

  int filePathCounter = 0;
  char *filePaths[MAX_FILEPATH_RECORDED] = {
      0}; // We will register a maximum of filepaths

  // Allocate space for the required file paths
  for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
    filePaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
  }

  while (!WindowShouldClose()) {
    // Check for dropped files.
    if (IsFileDropped()) {
      FilePathList droppedFiles = LoadDroppedFiles();

      for (int i = 0, offset = filePathCounter; i < (int)droppedFiles.count;
           i++) {
        if (filePathCounter < (MAX_FILEPATH_RECORDED - 1)) {
          TextCopy(filePaths[offset + i], droppedFiles.paths[i]);
          filePathCounter++;
        }
      }

      UnloadDroppedFiles(droppedFiles);
    }

    // TODO: Handle options

    BeginDrawing();
    ClearBackground(CLITERAL(Color){42, 42, 42, 255});

    if (filePathCounter == 0) {
      const char *text = "Drag your file/s...";
      DrawText(text, 840 / 2 - (MeasureText(text, 32) / 2), 480 / 2, 32, WHITE);
    } else {
      const char *vidFormats[] = {"mp4", "mkv", "webm"};
      const char *textFormat[] = {"srt", "ass"};

      // Single videos
      if (filePathCounter == 1 &&
          stringContains(vidFormats, 3, findFormat(filePaths[0]))) {
        // For videos
        const char *options[] = {"Choose an option:",     "1. Raise audio.",
                                 "2. Remove audio.",      "3. Compress.",
                                 "4. Change video speed", "5. Trim video"};
        int numOptions = sizeof(options) / sizeof(options[0]);

        for (int i = 0; i < numOptions; i++) {
          int textWidth = MeasureText(options[i], (i == 0) ? 32 : 16);
          DrawText(options[i], 840 / 2 - (textWidth / 2), 38 * (i + 1),
                   (i == 0) ? 32 : 16, WHITE);
        }
      }

      // Double files
      else if (filePathCounter == 2) {
        // TODO
      }
    }

    EndDrawing();
  }

  for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
    RL_FREE(filePaths[i]); // Free allocated memory for all filepaths
  }

  CloseWindow();

  return 0;
}

char *findFormat(const char *text) {
  char *extension = strrchr(text, '.');
  return extension + 1;
}

int stringContains(char **array, int length, const char *target) {
  for (int i = 0; i < length; i++) {
    if (strcmp(array[i], target) == 0) {
      return 1; // Found
    }
  }
  return 0; // Not found
}
