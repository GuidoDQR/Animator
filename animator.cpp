#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <filesystem>
#include <fstream>
namespace fs = std::filesystem;

#include "raymath.h"
using namespace std;

void ChangeTexturePosition(int actualPos, int newPos, std::vector<Texture2D>& textureManager, std::vector<Vector2>& texturePos, 
    std::vector<string>& filePathManager, std::vector<string>& fileNameManager, std::vector<Rectangle>& showTextureManager, std::vector<Color>& showTextureColorManager){

    Texture2D aux = textureManager[actualPos];
    textureManager[actualPos] = textureManager[newPos];
    textureManager[newPos] = aux;

    Vector2 auxVec = texturePos[actualPos];
    texturePos[actualPos] = texturePos[newPos];
    texturePos[newPos] = auxVec;

    string auxFilePath = filePathManager[actualPos];
    filePathManager[actualPos] = filePathManager[newPos];
    filePathManager[newPos] = auxFilePath;

    string auxFileName = fileNameManager[actualPos];
    fileNameManager[actualPos] = fileNameManager[newPos];
    fileNameManager[newPos] = auxFileName;

    Rectangle auxRec = showTextureManager[actualPos];
    showTextureManager[actualPos] = showTextureManager[newPos];
    showTextureManager[newPos] = auxRec;

    Color auxColor = showTextureColorManager[actualPos];
    showTextureColorManager[actualPos] = showTextureColorManager[newPos];
    showTextureColorManager[newPos] = auxColor;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Animator");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    FilePathList droppedFiles = { 0 };

    std::vector<Texture2D> textureManager;
    std::vector<Vector2> texturePos;
    std::vector<string> filePathManager;
    std::vector<string> fileNameManager;
    std::vector<Rectangle> showTextureManager;
    std::vector<Color> showTextureColorManager;
    bool canPressKey = true;

    Vector2 targetCamera = {0.0f - 20,0.0f};
    Camera2D camera = { 0 };
    camera.target = targetCamera;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.zoom = 1.0f;

    unsigned int actualTexture = 0;
    int framesCounter = 0;          // Variable used to count frames

    int textAlpha = 0;
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        // Load Images
        if (IsFileDropped()){          

            // Load new dropped files
            droppedFiles = LoadDroppedFiles();

            //printf("count: %d\n",droppedFiles.count);

            string filePath;
            string fileFormat;
            int filePathSize;
            string fileName;
            string fileNameReverse;
            Texture2D texture;
            
            for(unsigned int i=0; i < droppedFiles.count;i++){

                filePath = droppedFiles.paths[i];
                printf("filepath: %s\n",filePath.c_str());

                filePathManager.push_back(filePath);
                
                filePathSize = filePath.size();
                        
                fileFormat.clear();

                for(int i=3; i> 0;i--){
                    fileFormat += filePath[filePathSize-i];
                }
              
                if(fileFormat == "png" || fileFormat == "jpg"){
                    
                    texture = LoadTexture(filePath.c_str());
                    textureManager.push_back(texture);
                    texturePos.push_back(Vector2{0,0}); // Set position of the texture      

                    for(int i=filePath.size()-1; i >= 0;i--){
                        if(filePath[i] != '\\')
                            fileNameReverse += filePath[i];
                        else i = 0;
                    }

                    for(unsigned int i=1; i <= fileNameReverse.size();i++){
                        
                        fileName += fileNameReverse[fileNameReverse.size()-i];                 
                    }
                    printf("%s\n",fileName.c_str());

                    fileNameManager.push_back(fileName);

                    showTextureManager.push_back(Rectangle{screenWidth -240,(float)(screenHeight-190 + (34*showTextureColorManager.size())),32,32});
                    showTextureColorManager.push_back(GREEN);
                }      

                fileName.clear();   
                fileNameReverse.clear();      
            }

            UnloadDroppedFiles(droppedFiles);
        }

        framesCounter++;
        // Every two seconds (120 frames) 
        if (((framesCounter/10)%2) == 1 || canPressKey == true){
            canPressKey = true;

            if (IsKeyDown(KEY_RIGHT) || (!IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_D))){
                texturePos[actualTexture].x += 1;
                //printf("(%f,%f)\n", texturePos[actualTexture].x,texturePos[actualTexture].y);
                framesCounter = 0; canPressKey = false;
            } 
            else if (IsKeyDown(KEY_LEFT) || (!IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_A))){
               texturePos[actualTexture].x -= 1; 
                //printf("(%f,%f)\n", texturePos[actualTexture].x,texturePos[actualTexture].y);
                framesCounter = 0; canPressKey = false;
            } 

            if (IsKeyDown(KEY_DOWN) || (!IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_S))){
                texturePos[actualTexture].y += 1;
                //printf("(%f,%f)\n", texturePos[actualTexture].x,texturePos[actualTexture].y);
                framesCounter = 0; canPressKey = false;
            } 
            else if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)){
                texturePos[actualTexture].y -= 1; 
                //printf("(%f,%f)\n", texturePos[actualTexture].x,texturePos[actualTexture].y);
                framesCounter = 0; canPressKey = false;
            } 

            // Change edit of the actual texture

            if(IsKeyDown(KEY_LEFT_CONTROL)){
                if(textureManager.size()>1){
                    if(IsKeyPressed(KEY_A)){
                        //printf("pase por control + a\n");

                        canPressKey = false;

                        unsigned int lastPos;

                        if(actualTexture != 0){ lastPos = actualTexture-1; }
                        else{         lastPos = textureManager.size()-1; }

                        ChangeTexturePosition(actualTexture,lastPos,textureManager,texturePos, filePathManager, fileNameManager, showTextureManager, showTextureColorManager);

                        if(actualTexture != 0){ actualTexture--; }
                        else{ actualTexture = textureManager.size()-1; }
                        

     
                        
                    }else if(IsKeyPressed(KEY_D)){
                        
                        canPressKey = false;
                        
                        unsigned int lastPos;

                        if(actualTexture != textureManager.size()-1){ lastPos = actualTexture+1; }
                        else{ lastPos = 0; }
                        

                       ChangeTexturePosition(actualTexture,lastPos,textureManager,texturePos, filePathManager, fileNameManager, showTextureManager, showTextureColorManager);

                        if(actualTexture != textureManager.size()-1){ actualTexture++; }
                        else{ actualTexture = 0; }
                    }
                }
            }

            if(IsKeyDown(KEY_Q)){
                canPressKey = false;
                
                if(actualTexture!=0){
                    actualTexture--;                 
                }else { actualTexture = textureManager.size()-1; }
                framesCounter = 0;
            }else if(IsKeyDown(KEY_E)){
                canPressKey = false;
                actualTexture++;
                if(actualTexture >= textureManager.size()){
                    actualTexture = 0;
                }
                framesCounter = 0;
            }

            // zoom camera
            if(IsKeyDown(KEY_KP_ADD)){
                camera.zoom += 0.2;
            }else if(IsKeyDown(KEY_KP_SUBTRACT)){
                camera.zoom -= 0.2;
                if(camera.zoom < 0.2){ camera.zoom = 0.2; }
            }
        
        }

        // Change Visible image

        for(unsigned int i = 0; i<showTextureManager.size();i++){
            if(CheckCollisionPointRec(GetMousePosition(), showTextureManager[i]) && IsMouseButtonPressed(0)){
                if(showTextureColorManager[i].r == 0){ showTextureColorManager[i] = RED;
                                                }else{ showTextureColorManager[i] = GREEN; }
            }             
        }

        if(IsKeyPressed(KEY_F)){ 
            if(showTextureColorManager[actualTexture].r == 0){ showTextureColorManager[actualTexture] = RED;
                                                        }else{ showTextureColorManager[actualTexture] = GREEN; }
        }

        if(IsMouseButtonDown(0)){
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f/camera.zoom);

            camera.target = Vector2Add(camera.target, delta);
        }

        
        if(IsKeyDown(KEY_LEFT_CONTROL) ){
            if(textureManager.size()>0){

                // Save Positions of the images
                if(IsKeyPressed(KEY_S)){
                    textAlpha = 128; // 2 seconds

                    fs::create_directories("./Positions"); 
                    string fileName = "Positions/position.txt";
                    ofstream file;

                    if(FileExists(fileName.c_str())){ 
                        int i=0;
                        
                        do{
                            i++;
                            fileName = "Positions/position(" + to_string(i) + ").txt";            
                        }while(FileExists(fileName.c_str()) == true);

                        file.open(fileName.c_str());

                    }else{ file.open(fileName.c_str()); }

                    
                    for(unsigned int i=0; i< texturePos.size();i++){
                        file << texturePos[i].x << " " << texturePos[i].y;
                    }

                    file.close();  
                }
                // Delete the image
                else if(IsKeyPressed(KEY_X)){ 
                    textureManager.erase(textureManager.begin() + actualTexture);
                    texturePos.erase(texturePos.begin() + actualTexture);
                    filePathManager.erase(filePathManager.begin() + actualTexture);
                    fileNameManager.erase(fileNameManager.begin() + actualTexture);
                    showTextureManager.erase(showTextureManager.begin() + actualTexture);
                    showTextureColorManager.erase(showTextureColorManager.begin() + actualTexture);

                    if(actualTexture > 0 && actualTexture >= textureManager.size()){
                        actualTexture--;
                    }

                    for(unsigned int i=actualTexture; i < textureManager.size(); i++){
                        showTextureManager[i].y = (float)(screenHeight-190 + (34*i));  
                    }
                    
                }
            }      
        }

        
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode2D(camera);

            // Draw all the textures
            for(unsigned int i=0;i<textureManager.size();i++){
                if(showTextureColorManager[i].g == 228)
                    DrawTextureV(textureManager[i],texturePos[i],WHITE);
            }

            // Draw the actual Texture manipulated
            /*if(textureManager.size() > 0){
                if(showTextureColorManager[actualTexture].g == 228)
                    DrawTextureV(textureManager[actualTexture],texturePos[actualTexture],WHITE);
            }*/

            DrawLine((int)camera.target.x, -screenHeight*10, (int)camera.target.x, screenHeight*10, GREEN);
            DrawLine(-screenWidth*10, (int)camera.target.y, screenWidth*10, (int)camera.target.y, GREEN);

            EndMode2D();


            // Parameters Image
            DrawRectangleRec(Rectangle{screenWidth - 250,screenHeight-200,screenWidth,screenHeight},DARKGRAY);
            DrawText(TextFormat("zoom: %f",camera.zoom),10,screenHeight-40,20,DARKGRAY);

            if(textureManager.size() > 0){

                DrawText(TextFormat("position: %0.f, %0.f",texturePos[actualTexture].x,texturePos[actualTexture].y), 10, screenHeight-100, 20, DARKGRAY);
                DrawText(TextFormat("image: %d",actualTexture), 10, screenHeight-80, 20, DARKGRAY);
                DrawText(fileNameManager[actualTexture].c_str(),10,screenHeight-60,20,DARKGRAY);
                

                for(unsigned int i = 0; i<textureManager.size();i++){
                    DrawRectangleRec(showTextureManager[i],showTextureColorManager[i]);
                    DrawTextureEx(textureManager[i],Vector2{screenWidth - 210,(float)(screenHeight-190 + (34*i)) },0,0.5,WHITE);
                    DrawText(fileNameManager[i].c_str(),screenWidth - 160,screenHeight-190+ (34*i),2,WHITE);
                }

                DrawText(fileNameManager[actualTexture].c_str(),screenWidth - 160,screenHeight-190+ (34*actualTexture),2,ORANGE);
                
            }

            if(textAlpha>0){
                DrawText("Saved positions",0,0,20,Color{0,0,0,(unsigned char)textAlpha});
                textAlpha--;
            }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadDroppedFiles(droppedFiles); // Unload files memory

    if(textureManager.size()>0){
        for(unsigned int i=0;i<textureManager.size();i++) UnloadTexture(textureManager[i]);
    }

    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
