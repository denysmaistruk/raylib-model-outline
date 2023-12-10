#include "raylib.h"
#include "raymath.h"

int main(int argc, char const** argv)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib model outline");

    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3{ 3.0f, 3.0f, 3.0f }; // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                              // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;           // Camera mode type

    // Outline settings
    //--------------------------------------------------------------------------------------
    int outlineSize = 2;
    const float outlineColor[4] = {1, 0.63, 0, 1}; // Orange

    Shader outlineShader = LoadShader(0, TextFormat("%s\\shaders\\outline.fs", GetPrevDirectoryPath(GetWorkingDirectory())));

    SetShaderValue(outlineShader, GetShaderLocation(outlineShader, "width"), &screenWidth, SHADER_UNIFORM_INT);
    SetShaderValue(outlineShader, GetShaderLocation(outlineShader, "height"), &screenHeight, SHADER_UNIFORM_INT);
    SetShaderValue(outlineShader, GetShaderLocation(outlineShader, "color"), outlineColor, SHADER_UNIFORM_VEC4);

    int sizeLoc = GetShaderLocation(outlineShader, "size");
    SetShaderValue(outlineShader, sizeLoc, &outlineSize, SHADER_UNIFORM_INT);
    
    RenderTexture renderTexture = LoadRenderTexture(screenWidth, screenHeight);

    Image checked = GenImageChecked(2, 2, 1, 1, RED, BLUE);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);

    Model cube = LoadModelFromMesh(GenMeshCube(1, 1, 1));
    cube.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    Vector3 cubePosition = Vector3Zero();

    Model sphere = LoadModelFromMesh(GenMeshSphere(0.5f, 10, 20));
    sphere.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    Vector3 spherePosition = Vector3{0.5f, 0.5f, 0.5f};

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_ORBITAL);          // Update camera

        if (IsKeyPressed(KEY_UP)) ++outlineSize;        // Update input
        if (IsKeyPressed(KEY_DOWN)) --outlineSize;
        
        outlineSize = Clamp(outlineSize, 1, 8);
        SetShaderValue(outlineShader, sizeLoc, &outlineSize, SHADER_UNIFORM_INT);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);

            // Draw geometry
            //----------------------------------------------------------------------------------
            BeginMode3D(camera);
                
                DrawGrid(10, 1.0f);
                
                DrawModel(cube, cubePosition, 1, WHITE);
                DrawModel(sphere, spherePosition, 1, WHITE);

            EndMode3D();

            // Draw black silhouettes to texture
            //----------------------------------------------------------------------------------
            BeginTextureMode(renderTexture);

                ClearBackground(WHITE);

                BeginMode3D(camera);
                
                    DrawModel(cube, cubePosition, 1, BLACK);
                    DrawModel(sphere, spherePosition, 1, BLACK);

                EndMode3D();

            EndTextureMode();

            // Draw outline
            //----------------------------------------------------------------------------------
            BeginShaderMode(outlineShader);
            
                DrawTextureRec(renderTexture.texture, 
                    Rectangle{ 0, 0, (float)renderTexture.texture.width, (float)-renderTexture.texture.height }, Vector2{ 0, 0 }, WHITE);

            EndShaderMode();

            // Draw text
            //----------------------------------------------------------------------------------
            DrawText("Model outline", 10, 10, 20, WHITE);
            DrawText("Arrow Up/Down to\nchange outline size", 10, 42, 20, WHITE);
            DrawText(TextFormat("Outline size: %i px", (int)outlineSize), 10, 90, 20, YELLOW);

            DrawFPS(710, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(cube);
    UnloadModel(sphere);

    UnloadShader(outlineShader);

    UnloadRenderTexture(renderTexture);

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}