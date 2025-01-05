#include "Game.h"
#include "GameLogic.h"
#include "GameCallbacks.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

// Include the logic so we can call them here:
extern void setupGameObjects(GameObject &ship,
                             std::vector<GameObject> &diamonds,
                             std::shared_ptr<GameGeometry> shipGeometry,
                             std::shared_ptr<GameGeometry> diamondGeometry);

extern void updateShipGeom(GameObject &ship,
                           const glm::vec3 &mousePos,
                           bool isWPressed,
                           bool isSPressed);

extern void gameLogic(GameObject &ship,
                      std::vector<GameObject> &diamonds,
                      int &score);

Game::Game(std::shared_ptr<GameCallbacks> inputManager,
           std::shared_ptr<GameGeometry> shipGeometry,
           std::shared_ptr<GameGeometry> diamondGeometry,
           ShaderProgram &shaderRef,
           int windowW,
           int windowH)
    : inputManager(inputManager), shipGeom(shipGeometry), diamondGeom(diamondGeometry), shader(shaderRef), ship(shipGeometry) // <-- Initialize 'ship' by calling GameObject(std::shared_ptr<GameGeometry>)
      ,
      score(0), windowWidth(windowW), windowHeight(windowH), transformLoc(-1) // Optionally set a default
{
    // Grab the transformation uniform from your shader
    transformLoc = glGetUniformLocation(shader.getProgram(), "transformationMatrix");

    // Set up initial objects (calls your free function(s))
    reset();
}

void Game::reset()
{
    score = 0;
    setupGameObjects(ship, diamonds, shipGeom, diamondGeom);
}

void Game::update()
{
    // If user pressed ENTER => reset
    if (inputManager->isKeyPressed(GLFW_KEY_ENTER))
    {
        reset();
    }

    // Get mouse pos, W key, S key from input manager
    glm::vec3 mousePos = inputManager->mouseGL();
    bool isWPressed = inputManager->isKeyPressed(GLFW_KEY_W);
    bool isSPressed = inputManager->isKeyPressed(GLFW_KEY_S);

    // Move the ship based on mouse pos + W/S
    updateShipGeom(ship, mousePos, isWPressed, isSPressed);

    // Collisions, scoring, etc.
    gameLogic(ship, diamonds, score);
}

void Game::render(Window &window)
{
    // Use the shader
    shader.use();

    // sRGB & clear
    glEnable(GL_FRAMEBUFFER_SRGB);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the ship
    ship.geometry->ggeom.bind();
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &ship.transformationMatrix[0][0]);

    ship.geometry->texture.bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    ship.geometry->texture.unbind();

    // Draw diamonds
    for (const auto &diamond : diamonds)
    {
        diamond.geometry->ggeom.bind();
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &diamond.transformationMatrix[0][0]);

        diamond.geometry->texture.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        diamond.geometry->texture.unbind();
    }

    glDisable(GL_FRAMEBUFFER_SRGB);

    // Now do your ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(5, 5));
    ImGuiWindowFlags textWindowFlags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("scoreText", (bool *)0, textWindowFlags);
    ImGui::SetWindowFontScale(1.5f);

    // If diamonds are gone => user won
    if (diamonds.empty())
    {
        ImGui::Text("Congratulations you won!!! \nPress Enter to play again");
    }
    else
    {
        ImGui::Text("Score: %d", score);
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Finally, swap buffers
    window.swapBuffers();
}
