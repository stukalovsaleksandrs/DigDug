// Project
#include "Application.hpp"
// Third-party
#include "SDL3/SDL_main.h"// Required for the windows build not to give errors
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

int main(int, char*[]) {
    Game::Application game{};
    game.Run();
}

// #include <SDL3/SDL.h>
// #include <vector>
// #include <cmath>
// #include <fstream>
// #include <cstring>
// #include <ranges>
// #define GLM_ENABLE_EXPERIMENTAL// Required for norm.hpp
// #include <glm/gtx/norm.hpp>
//
// const int LOGICAL_WIDTH = 224;
// const int LOGICAL_HEIGHT = 288;
// const int WINDOW_WIDTH = 448;   // 2x scale
// const int WINDOW_HEIGHT = 576;  // 2x scale
//
// struct Player {
//     float x, y;
//     float speed = 75.0f;
//     float dirX = 0, dirY = 0;
//     float radius = 8.0f;
// };
//
// // Simple PNG loader implementation
// class PNGBitmap {
// public:
//     int width, height;
//     std::vector<Uint8> pixels; // RGBA format
//
//     bool LoadFromFile(const char* filename) {
//         std::ifstream file(filename, std::ios::binary);
//         if (!file) {
//             SDL_Log("Failed to open file: %s", filename);
//             return false;
//         }
//
//         // Read PNG signature
//         Uint8 signature[8];
//         file.read(reinterpret_cast<char*>(signature), 8);
//         if (file.gcount() != 8) return false;
//
//         // Verify PNG signature
//         const Uint8 pngSignature[8] = {137, 80, 78, 71, 13, 10, 26, 10};
//         if (memcmp(signature, pngSignature, 8) != 0) {
//             SDL_Log("Not a valid PNG file: %s", filename);
//             return false;
//         }
//
//         // Quick and dirty PNG reader - just enough for simple PNGs
//         // This skips proper zlib decompression and filtering
//         // For production code, use a proper PNG library or SDL3_image
//
//         width = 0;
//         height = 0;
//
//         while (file) {
//             Uint32 length;
//             file.read(reinterpret_cast<char*>(&length), 4);
//             length = SDL_Swap32BE(length); // PNG is big-endian
//
//             char type[5] = {0};
//             file.read(type, 4);
//
//             if (strcmp(type, "IHDR") == 0) {
//                 // Read image header
//                 Uint8 ihdrData[13];
//                 file.read(reinterpret_cast<char*>(ihdrData), 13);
//
//                 width = (ihdrData[0] << 24) | (ihdrData[1] << 16) |
//                         (ihdrData[2] << 8) | ihdrData[3];
//                 height = (ihdrData[4] << 24) | (ihdrData[5] << 16) |
//                          (ihdrData[6] << 8) | ihdrData[7];
//
//                 // Skip CRC
//                 file.seekg(4, std::ios::cur);
//
//             } else if (strcmp(type, "IDAT") == 0) {
//                 // For simplicity, we'll just note that we found image data
//                 // A real implementation would need zlib decompression
//                 SDL_Log("Found IDAT chunk (skipping - simplified loader)");
//                 file.seekg(length + 4, std::ios::cur);
//
//             } else if (strcmp(type, "IEND") == 0) {
//                 break;
//             } else {
//                 // Skip unknown chunks
//                 file.seekg(length + 4, std::ios::cur);
//             }
//         }
//
//         if (width == 0 || height == 0) {
//             SDL_Log("Failed to extract PNG dimensions from: %s", filename);
//             return false;
//         }
//
//         SDL_Log("PNG dimensions: %dx%d (note: pixel data not loaded without proper decoder)",
//                 width, height);
//         return true;
//     }
// };
//
// // Texture creation from raw RGBA data
// SDL_Texture* CreateTextureFromRGBA(SDL_Renderer* renderer,
//                                    int width, int height,
//                                    const Uint8* rgbaData) {
//     SDL_Texture* texture = SDL_CreateTexture(renderer,
//                                              SDL_PIXELFORMAT_RGBA8888,
//                                              SDL_TEXTUREACCESS_STATIC,
//                                              width, height);
//     if (texture) {
//         SDL_UpdateTexture(texture, NULL, rgbaData, width * 4);
//     }
//     return texture;
// }
//
// class DigDugGame {
// private:
//     SDL_Renderer* renderer;
//     SDL_Texture* backgroundTexture;
//     SDL_Texture* maskTexture;
//     SDL_Texture* maskedBackground;
//     SDL_Texture* playerTexture;
//
//     float digRadius = 14.0f;
//     Player player;
//
//     // Manual PNG loading without SDL3_image
//     SDL_Texture* LoadPNGTexture(const char* filename) {
//         PNGBitmap bitmap;
//         if (!bitmap.LoadFromFile(filename)) {
//             return CreateFallbackTexture(filename);
//         }
//
//         // Since we can't actually decode PNG without a proper library,
//         // we'll use fallback textures but note the attempt
//         SDL_Log("Attempting to use PNG: %s (using fallback - need proper PNG decoder)",
//                 filename);
//         return CreateFallbackTexture(filename);
//     }
//
//     SDL_Texture* CreateFallbackTexture(const char* filename) {
//         SDL_Log("Creating fallback texture for: %s", filename);
//
//         SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
//                                                  SDL_TEXTUREACCESS_TARGET,
//                                                  LOGICAL_WIDTH, LOGICAL_HEIGHT);
//         if (strstr(filename, "Background")) {
//             // Create dirt-style background
//             SDL_SetRenderTarget(renderer, texture);
//             SDL_SetRenderDrawColor(renderer, 139, 90, 43, 255);
//             SDL_RenderClear(renderer);
//
//             // Add dirt texture pattern
//             for (int i = 0; i < 2000; i++) {
//                 int x = rand() % LOGICAL_WIDTH;
//                 int y = rand() % LOGICAL_HEIGHT;
//                 Uint8 shade = 100 + (rand() % 80);
//                 SDL_SetRenderDrawColor(renderer, shade, shade * 0.7, shade * 0.4, 255);
//                 SDL_RenderPoint(renderer, x, y);
//             }
//             SDL_SetRenderTarget(renderer, NULL);
//         } else if (strstr(filename, "player")) {
//             // Create simple player texture (smaller size)
//             SDL_DestroyTexture(texture);
//             texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
//                                        SDL_TEXTUREACCESS_TARGET, 32, 32);
//             SDL_SetRenderTarget(renderer, texture);
//             SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
//             SDL_RenderClear(renderer);
//
//             // Draw simple character
//             for (int y = 0; y < 32; y++) {
//                 for (int x = 0; x < 32; x++) {
//                     float dx = x - 16;
//                     float dy = y - 16;
//                     float dist = sqrt(dx*dx + dy*dy);
//                     if (dist < 14) {
//                         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//                         SDL_RenderPoint(renderer, x, y);
//                     }
//                 }
//             }
//             SDL_SetRenderTarget(renderer, NULL);
//         }
//         return texture;
//     }
//
// public:
//     DigDugGame(SDL_Renderer* rend) : renderer(rend) {
//         // Load textures from PNG files
//         backgroundTexture = LoadPNGTexture("Resources/Sprites/DigDugBackground.png");
//
//         // Create mask texture
//         maskTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
//                                        SDL_TEXTUREACCESS_TARGET,
//                                        LOGICAL_WIDTH, LOGICAL_HEIGHT);
//         SDL_SetTextureBlendMode(maskTexture, SDL_BLENDMODE_BLEND);
//
//         // Initialize mask to white (fully visible)
//         SDL_SetRenderTarget(renderer, maskTexture);
//         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//         SDL_RenderClear(renderer);
//         SDL_SetRenderTarget(renderer, NULL);
//
//         // Create result texture
//         maskedBackground = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
//                                         SDL_TEXTUREACCESS_TARGET,
//                                         LOGICAL_WIDTH, LOGICAL_HEIGHT);
//
//         // Create initial hole at player position
//         player.x = LOGICAL_WIDTH / 2;
//         player.y = 30;
//         DigCircle(player.x, player.y, 15.0f);
//
//         // Load player texture
//         playerTexture = LoadPNGTexture("Resources/Sprites/player.png");
//     }
//
//     ~DigDugGame() {
//         SDL_DestroyTexture(backgroundTexture);
//         SDL_DestroyTexture(maskTexture);
//         SDL_DestroyTexture(maskedBackground);
//         SDL_DestroyTexture(playerTexture);
//     }
//
//     void DigCircle(float cx, float cy, float radius) {
//         SDL_SetRenderTarget(renderer, maskTexture);
//
//         // Fill the center completely black
//         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//         for (int y = (int)(cy - radius); y <= (int)(cy + radius); y++) {
//             if (y < 0 || y >= LOGICAL_HEIGHT) continue;
//
//             for (int x = (int)(cx - radius); x <= (int)(cx + radius); x++) {
//                 if (x < 0 || x >= LOGICAL_WIDTH) continue;
//
//                 float dx = x - cx;
//                 float dy = y - cy;
//                 float dist = sqrt(dx*dx + dy*dy);
//
//                 if (dist <= radius) {
//                     SDL_RenderPoint(renderer, x, y);
//                 }
//             }
//         }
//
//         SDL_SetRenderTarget(renderer, NULL);
//     }
//
//     void ApplyMask() {
//         SDL_SetRenderTarget(renderer, maskedBackground);
//         SDL_RenderTexture(renderer, backgroundTexture, NULL, NULL);
//
//         SDL_SetTextureBlendMode(maskTexture, SDL_BLENDMODE_MOD);
//         SDL_RenderTexture(renderer, maskTexture, NULL, NULL);
//
//         SDL_SetRenderTarget(renderer, NULL);
//     }
//
//     void HandleInput(const bool* keyState, float deltaTime) {
//         float targetDirX = 0, targetDirY = 0;
//
//         if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]) targetDirX = -1;
//         if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]) targetDirX = 1;
//         if (keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]) targetDirY = -1;
//         if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]) targetDirY = 1;
//
//         float smoothing = 10.0f;
//         player.dirX += (targetDirX - player.dirX) * smoothing * deltaTime;
//         player.dirY += (targetDirY - player.dirY) * smoothing * deltaTime;
//
//         float length = sqrt(player.dirX*player.dirX + player.dirY*player.dirY);
//         if (length > 1.0f) {
//             player.dirX /= length;
//             player.dirY /= length;
//         }
//
//         player.x += player.dirX * player.speed * deltaTime;
//         player.y += player.dirY * player.speed * deltaTime;
//
//         player.x = fmax(player.radius, fmin(LOGICAL_WIDTH - player.radius, player.x));
//         player.y = fmax(player.radius, fmin(LOGICAL_HEIGHT - player.radius, player.y));
//
//         if (length > 0.1f) {
//             DigCircle(player.x, player.y, digRadius);
//         }
//     }
//
//     void Render() {
//         ApplyMask();
//
//         // Draw the masked background
//         SDL_RenderTexture(renderer, maskedBackground, NULL, NULL);
//
//         // Render player
//         SDL_FRect playerRect = {
//             player.x - player.radius * 2,
//             player.y - player.radius * 2,
//             player.radius * 4,
//             player.radius * 4
//         };
//
//         double angle = 0;
//         if (fabs(player.dirX) > 0.1f || fabs(player.dirY) > 0.1f) {
//             angle = atan2(player.dirY, player.dirX) * 180.0 / M_PI;
//         }
//
//         SDL_RenderTextureRotated(renderer, playerTexture, NULL, &playerRect,
//                                 angle, NULL, SDL_FLIP_NONE);
//
//         // Particle effects
//         if (fabs(player.dirX) > 0.1f || fabs(player.dirY) > 0.1f) {
//             SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
//
//             for (int i = 0; i < 8; i++) {
//                 float angle_offset = (rand() % 360) * M_PI / 180.0f;
//                 float particle_dist = 10.0f + (rand() % 15);
//                 float px = player.x + cos(angle_offset) * particle_dist;
//                 float py = player.y + sin(angle_offset) * particle_dist;
//                 float particle_alpha = 100 + (rand() % 100);
//
//                 SDL_SetRenderDrawColor(renderer, 139, 69, 19, (Uint8)particle_alpha);
//                 SDL_FRect particleRect = {px - 1, py - 1, 2, 2};
//                 SDL_RenderFillRect(renderer, &particleRect);
//             }
//         }
//     }
// };
//
// int main(int argc, char* argv[]) {
//     SDL_Init(SDL_INIT_VIDEO);
//
//     SDL_Window* window = SDL_CreateWindow("Texture Masking - Dig Dug Style",
//                                           WINDOW_WIDTH, WINDOW_HEIGHT,
//                                           SDL_WINDOW_RESIZABLE);
//
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
//
//     SDL_SetRenderLogicalPresentation(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT,
//                                      SDL_LOGICAL_PRESENTATION_LETTERBOX);
//
//     DigDugGame game(renderer);
//
//     bool quit = false;
//     SDL_Event event;
//     Uint32 lastTime = SDL_GetTicks();
//
//     while (!quit) {
//         Uint32 currentTime = SDL_GetTicks();
//         float deltaTime = (currentTime - lastTime) / 1000.0f;
//         lastTime = currentTime;
//
//         if (deltaTime > 0.05f) deltaTime = 0.05f;
//
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_EVENT_QUIT) {
//                 quit = true;
//             }
//         }
//
//         const bool* keyState = SDL_GetKeyboardState(NULL);
//
//         game.HandleInput(keyState, deltaTime);
//         game.Render();
//
//         SDL_RenderPresent(renderer);
//     }
//
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();
//
//     return 0;
// }
