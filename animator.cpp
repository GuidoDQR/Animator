#include <raylib.h>
#include <iostream>
#include <vector>
using namespace std;

int main(){
	// Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - drop files");

    FilePathList droppedFiles;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsFileDropped())
        {
            // Is some files have been previously loaded, unload them
            //if (droppedFiles.count > 0) UnloadDroppedFiles(droppedFiles);
            
            // Load new dropped files
            droppedFiles = LoadDroppedFiles();
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            if (droppedFiles.count == 0) DrawText("Drop your files to this window!", 100, 40, 20, DARKGRAY);
            else
            {
                DrawText("Dropped files:", 100, 40, 20, DARKGRAY);

                for (unsigned int i = 0; i < droppedFiles.count; i++)
                {
                    if (i%2 == 0) DrawRectangle(0, 85 + 40*i, screenWidth, 40, Fade(LIGHTGRAY, 0.5f));
                    else DrawRectangle(0, 85 + 40*i, screenWidth, 40, Fade(LIGHTGRAY, 0.3f));

                    DrawText(droppedFiles.paths[i], 120, 100 + 40*i, 10, GRAY);
                }

                DrawText("Drop new files...", 100, 110 + 40*droppedFiles.count, 20, DARKGRAY);
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadDroppedFiles(droppedFiles); // Unload files memory

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

	return 0;
}