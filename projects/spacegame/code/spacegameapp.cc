//------------------------------------------------------------------------------
// spacegameapp.cc
// (C) 2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "spacegameapp.h"
#include <cstring>
#include "imgui.h"
#include "render/renderdevice.h"
#include "render/shaderresource.h"
#include <vector>
#include "render/textureresource.h"
#include "render/model.h"
#include "render/cameramanager.h"
#include "render/lightserver.h"
#include "render/debugrender.h"
#include "core/random.h"
#include "render/input/inputserver.h"
#include "core/cvar.h"
#include "render/physics.h"
#include <chrono>
#include "loader.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace Display;
using namespace Render;

namespace Game
{

//------------------------------------------------------------------------------
/**
*/
SpaceGameApp::SpaceGameApp()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
SpaceGameApp::~SpaceGameApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
SpaceGameApp::Open()
{
	App::Open();
	this->window = new Display::Window;
    this->window->SetSize(1280, 720);

    if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        RenderDevice::Init();

		// set ui rendering function
		this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});
        this->window->SetNanoFunc([this](NVGcontext* vg)
        {
            this->RenderNanoVG(vg);
        });
        
        return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
SpaceGameApp::Run()
{
    int w;
    int h;
    this->window->GetSize(w, h);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(w) / float(h), 0.01f, 1000.f);
    Camera* cam = CameraManager::GetCamera(CAMERA_MAIN);
    cam->projection = projection;

    ModelId golfModels[14] = {
        LoadModel("assets/golf/open.glb"),
        LoadModel("assets/golf/side.glb"),
        LoadModel("assets/golf/corner.glb"),
        LoadModel("assets/golf/square-corner-a.glb"),
        LoadModel("assets/golf/round-corner-c.glb"),
        LoadModel("assets/golf/round-corner-a.glb"),
        LoadModel("assets/golf/round-corner-b.glb"),
        LoadModel("assets/golf/inner-corner.glb"),
        LoadModel("assets/golf/straight.glb"),
        LoadModel("assets/golf/walls-to-open.glb"),
        LoadModel("assets/golf/end.glb"),
        LoadModel("assets/golf/hole-open.glb"),
        LoadModel("assets/golf/hole-square.glb"),
        LoadModel("assets/golf/flag-red.glb")
    };

    Physics::ColliderMeshId golfColliderMeshes[14] = {
        Physics::LoadColliderMesh("assets/golf/open.glb"),
        Physics::LoadColliderMesh("assets/golf/side.glb"),
        Physics::LoadColliderMesh("assets/golf/corner.glb"),
        Physics::LoadColliderMesh("assets/golf/square-corner-a.glb"),
        Physics::LoadColliderMesh("assets/golf/round-corner-c.glb"),
        Physics::LoadColliderMesh("assets/golf/round-corner-a.glb"),
        Physics::LoadColliderMesh("assets/golf/round-corner-b.glb"),
        Physics::LoadColliderMesh("assets/golf/inner-corner.glb"),
        Physics::LoadColliderMesh("assets/golf/straight.glb"),
        Physics::LoadColliderMesh("assets/golf/walls-to-open.glb"),
        Physics::LoadColliderMesh("assets/golf/end.glb"),
        Physics::LoadColliderMesh("assets/golf/hole-open.glb"),
        Physics::LoadColliderMesh("assets/golf/hole-square.glb"),
        Physics::LoadColliderMesh("assets/golf/flag-red.glb")
    };
    
    //Set up level array and load level 1
    std::vector<std::string> levelPath = { "levels/level-1.txt", "levels/level-2.txt", "levels/level-3.txt" };
    int cl = 0; //curren Level
    Physics::ColliderId flagColliderId = levelLoader::loadLevel(levelPath[cl], level, golfModels, golfColliderMeshes);

    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i) {
        //std::cout << i << (glfwJoystickPresent(i) ? " true" : " false") << '\n';
        if (glfwJoystickPresent(i)) {
            Input::CreateGamepad(i);
        }
    }
    
    // Setup skybox
    std::vector<const char*> skybox
    {
        "textures/vikkitor.png",
        "textures/vikkitor.png",
        "textures/vikkitor.png",
        "textures/vikkitor.png",
        "textures/vikkitor.png",
        "textures/vikkitor.png"
    };

    TextureResourceId skyboxId = TextureResource::LoadCubemap("skybox", skybox, true);
    RenderDevice::SetSkybox(skyboxId);
   

    const int numLights = 4;
    Render::PointLightId lights[numLights];
    // Setup lights
    for (int i = 0; i < numLights; i++)
    {
        glm::vec3 translation = glm::vec3(
            Core::RandomFloatNTP() * 20.0f,
            Core::RandomFloatNTP() * 20.0f,
            Core::RandomFloatNTP() * 20.0f
        );
        glm::vec3 color = glm::vec3(
            Core::RandomFloat(),
            Core::RandomFloat(),
            Core::RandomFloat()
        );
        lights[i] = Render::LightServer::CreatePointLight(translation, color, Core::RandomFloat() * 4.0f, 1.0f + (15 + Core::RandomFloat() * 10.0f));
    }

    ship.model = LoadModel("assets/golf/ball-blue.glb");
    ship.position.y = 1.0f;
    Physics::RaycastPayload hit = Physics::Raycast(level, glm::vec3(ship.position.x, ship.position.y - 0.03f, ship.position.z), glm::vec3(0, -1.0f, 0), 10.0f);
    ship.position.y -= hit.hitDistance;

    std::clock_t c_start = std::clock();
    double dt = 0.01667f;

    // game loop
    while (this->window->IsOpen())
    {
        auto timeStart = std::chrono::steady_clock::now();
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        this->window->Update();
        if (auto cgp = Input::GetCurrentGamepad(); cgp != nullptr)
            cgp->Update();

        Input::Keyboard* kbd = Input::GetDefaultKeyboard();
        Input::Gamepad* gpd = Input::GetCurrentGamepad();

        if (kbd->pressed[Input::Key::Code::End])
        {
            ShaderResource::ReloadShaders();
        }

        ship.Update(dt);

        //Highscore system
        if (ship.CheckCollisions(level, flagColliderId)) {
            std::string line;
            std::ifstream inputFile("levels/highscores.txt");
            if (!inputFile) {
                this->Close();
                break;
            }

            std::vector<std::string> scores;
            for (int i = 0; std::getline(inputFile, line); i++) {
                if (i == cl && stoi(line) > ship.strokes) {
                    scores.push_back(std::to_string(ship.strokes));
                    continue;
                }
                scores.push_back(line);
            }
            inputFile.close();

            std::ofstream outputFile("levels/highscores.txt");
            if (!outputFile) {
                this->Close();
                break;
            }

            for (std::string line : scores) {
                outputFile << line << "\n";
            }
            outputFile.close();


            ship.position = { 0, 0, 0 };
            ship.transform = glm::mat4(1);
            ship.linearVelocity = { 0, 0, 0 };
            ship.camRot = { 0, 0, 0, };
            ship.position.y = 1.0f;
            Physics::RaycastPayload hit = Physics::Raycast(level, glm::vec3(ship.position.x, ship.position.y - 0.03f, ship.position.z), glm::vec3(0, -1.0f, 0), 10.0f);
            ship.position.y -= hit.hitDistance;
            ship.strokes = 0;
            if (++cl == 3) won = true;
            else {
                level.tiles.clear();
                flagColliderId = levelLoader::loadLevel(levelPath[cl], level, golfModels, golfColliderMeshes);
            }
        }

        if (!won) {
            // Store all drawcalls in the render device
            for (auto const& tile : level.tiles) {
                RenderDevice::Draw(tile.model, tile.transform);
            }

            RenderDevice::Draw(ship.model, ship.transform);

            // Execute the entire rendering pipeline
            RenderDevice::Render(this->window, dt);
            //Physics::DebugDrawColliders();
        }

		// transfer new frame to window
        this->window->SwapBuffers();

        auto timeEnd = std::chrono::steady_clock::now();
        dt = std::min(0.04, std::chrono::duration<double>(timeEnd - timeStart).count());

        if (auto cgp = Input::GetCurrentGamepad(); cgp && cgp->pressed[Input::GamepadButton::Code::BACK])
            this->Exit();
        if (kbd->pressed[Input::Key::Code::Escape])
            this->Exit();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
SpaceGameApp::Exit()
{
    this->window->Close();
}

//------------------------------------------------------------------------------
/**
*/
void
SpaceGameApp::RenderUI()
{
	if (this->window->IsOpen())
	{
	}
}

//------------------------------------------------------------------------------
/**
*/
void
SpaceGameApp::RenderNanoVG(NVGcontext* vg)
{
    nvgSave(vg);
    if (!won) {
        nvgBeginPath(vg);
        nvgStrokeColor(vg, nvgRGBA(0, 0, 0, 32));
        nvgStroke(vg);

        nvgFontSize(vg, 18.0f);
        nvgFontFace(vg, "sans");
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 180));
        nvgText(vg, 0, 30, ("Shooting power: " + std::to_string(ship.shootPower)).c_str(), NULL);
    }
    else {
        nvgBeginPath(vg);
        nvgRect(vg, 0, 0, 1280, 720);
        nvgFillColor(vg, nvgRGBA(50, 200, 150, 255));
        nvgFill(vg);

        nvgFontSize(vg, 64.0f);
        nvgFontFace(vg, "sans");
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
        nvgText(vg, 1280.0f / 2 - 32.0f * 3.5f, 720.0f / 2 - 32.0f, "You Won", NULL);
    }

    nvgRestore(vg);
}

} // namespace Game