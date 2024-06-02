#include "utils.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_FILEPATH_RECORDED 4096
#define MAX_FILEPATH_SIZE 2048

int main(void) {
  InitWindow(840, 480, "vidmixer");
  SetTargetFPS(60);
  SetExitKey(KEY_Q);

  int filePathCounter = 0, option = 0, rendering = 0;
  char *filePaths[MAX_FILEPATH_RECORDED] = {0};

  for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
    filePaths[i] = (char *)RL_CALLOC(MAX_FILEPATH_SIZE, 1);
  }

  // Main loop
  while (!WindowShouldClose()) {
    if (IsFileDropped() && filePathCounter != 2) {
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

    // Clear files by pressing escape
    if (IsKeyPressed(KEY_ESCAPE) && filePathCounter >= 1) {
      rendering = false;
      filePathCounter = 0;
      for (int i = 0; i < filePathCounter; i++) {
        filePaths[i][0] = '\0';
      }
    }

    BeginDrawing();
    ClearBackground(CLITERAL(Color){42, 42, 42, 255});

    if (filePathCounter == 0) {
      const char *text = "Drag your mp4 and srt files...";
      DrawText(text, 840 / 2 - (MeasureText(text, 32) / 2), 480 / 2, 32, WHITE);
    } else if (filePathCounter == 2) {
      const char *vidFormats[] = {"mp4", "mkv"};
      const char *textFormat[] = {"srt", "ass"};
      int vidCorrect =
          stringContains(vidFormats, 2, findFormat(filePaths[0])) ||
          stringContains(vidFormats, 2, findFormat(filePaths[1]));
      int srtCorrect =
          stringContains(textFormat, 2, findFormat(filePaths[0])) ||
          stringContains(textFormat, 2, findFormat(filePaths[1]));

      if (vidCorrect && srtCorrect) {
	const char *text = "Nice. Combining... \n\nPlease wait, won't even take a minute.";
	DrawText(text, 840 / 2 - (MeasureText(text, 32) / 2), 480 / 2, 32,
		 WHITE);
        
        // Process
        if (!rendering) {
          char command[1024];
          if (strcmp(findFormat(filePaths[0]), "mkv") == 0 || strcmp(findFormat(filePaths[0]), "mp4") == 0) {
            snprintf(command, sizeof(command),
                     "ffmpeg -i \"%s\" -f %s -i \"%s\" -c:v copy -c:a copy -c:s %s %s/output.mkv",
                     filePaths[0], findFormat(filePaths[1]), filePaths[1], findFormat(filePaths[1]), getVideosPath());
          } else {
            snprintf(command, sizeof(command),
                     "ffmpeg -i \"%s\" -f %s -i \"%s\" -c:v copy -c:a copy -c:s %s %s/output.mkv",
                     filePaths[1], findFormat(filePaths[0]), filePaths[0], findFormat(filePaths[0]), getVideosPath());
          }
          printf("%s\n", command);
          system(command);
          rendering = true;
        }

        // Reset
        const char *text2 = "Done, check your home folder.\n\nPress ESC to reset.";
	DrawText(text2, 840 / 2 - (MeasureText(text2, 32) / 2), 480 / 2, 32, WHITE);
      } else {
        const char *text =
            "Drop only one mp4 and one srt file.\n\nPress ESC to reset.";
        DrawText(text, 840 / 2 - (MeasureText(text, 32) / 2), 480 / 2, 32,
                 WHITE);
      }
    } else if (filePathCounter == 1) {
      const char *text = "That's one file. Put one more.";
      DrawText(text, 840 / 2 - (MeasureText(text, 32) / 2), 480 / 2, 32, WHITE);
    } else if (filePathCounter > 2) {
      const char *text =
          "Drop only one mp4 and one srt file.\n\nPress ESC to reset.";
      DrawText(text, 840 / 2 - (MeasureText(text, 32) / 2), 480 / 2, 32, WHITE);
    }

    EndDrawing();
  }

  for (int i = 0; i < MAX_FILEPATH_RECORDED; i++) {
    RL_FREE(filePaths[i]); // Free allocated memory for all filepaths
  }

  CloseWindow();

  return 0;
}
