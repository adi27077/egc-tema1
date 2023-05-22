#include "lab_m1/tema1/tema1.h"

#include <random>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
= default;


Tema1::~Tema1()
= default;

void Tema1::Init()
{
	const glm::ivec2 resolution = window->GetResolution();
	const auto camera = GetSceneCamera();
    camera->SetOrthographic(0, static_cast<float>(resolution.x), 0, static_cast<float>(resolution.y), 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    windowSizeX = resolution.x;
    windowSizeY = resolution.y;

    constexpr auto wingsLayer = glm::vec3(0, 0, 0);
    constexpr auto bodyLayer = glm::vec3(0, 0, 1);
    constexpr auto headLayer = glm::vec3(0, 0, 2);
    constexpr auto grassLayer = glm::vec3(0, 0, 3);
    constexpr auto UILayer = glm::vec3(0, 0, 4);

    duckLength = 80;
    hitboxRadius = duckLength * 1.25f;

    bulletLength = 20;
    bulletHeight = 40;
    liveRadius = 20;
    bullets = maxBullets;
    lives = maxLives;
    score = 0;
    ducksShot = 0;
    speedModifier = 1.0;

    timePassed = 0;
    maxTimeLevel = 7.5;
    duckEvade = false;
    duckShot = false;
    duckDead = false;

    duckPositionX = duckSpawnX;
    duckPositionY = duckSpawnY;
    duckCenterX = duckPositionX + duckLength + 20;
    duckCenterY = duckPositionY + duckLength / 2.0f;

    duckPositionX = duckCenterX;
    duckPositionY = duckCenterY;

    duckHeadingDirection = getRandomAngle();

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    wingsAngularStep = 0;
    rotationDirection = 1;

    //duck body
    Mesh* triangle1 = object2D::CreateTriangle("triangle1", bodyLayer, duckLength, glm::vec3(0.26, 0.16, 0.05));
    AddMeshToList(triangle1);

    //duck wings
    Mesh* triangle2 = object2D::CreateTriangle("triangle2", wingsLayer, duckLength / 2, glm::vec3(0.26, 0.16, 0.05));
    AddMeshToList(triangle2);

    //duck beak
    Mesh* triangle3 = object2D::CreateTriangle("triangle3", bodyLayer, duckLength / 4, glm::vec3(1, 1, 0));
    AddMeshToList(triangle3);

    //duck head
    Mesh* circle1 = object2D::CreateCircle("circle1", headLayer, duckLength / 3, glm::vec3(0.06, 0.27, 0.05));
    AddMeshToList(circle1);

    //bullets
    Mesh* rectangle1 = object2D::CreateRectangle("rectangle1", UILayer, bulletLength, bulletHeight, glm::vec3(0, 0.7, 0));
    AddMeshToList(rectangle1);

    //lives
    Mesh* circle2 = object2D::CreateCircle("circle2", UILayer, liveRadius, glm::vec3(1, 0, 0));
    AddMeshToList(circle2);

	//score
    Mesh* rectangle2 = object2D::CreateRectangle("rectangle2", UILayer, static_cast<float>(maxScore) + 2, bulletHeight + 2, glm::vec3(1, 1, 1));
    AddMeshToList(rectangle2);

    Mesh* rectangle3 = object2D::CreateRectangle("rectangle3", UILayer, static_cast<float>(maxScore), bulletHeight, glm::vec3(0, 0, 1), true);
    AddMeshToList(rectangle3);

    //hitbox
    Mesh* circle3 = object2D::CreateCircle("circle3", glm::vec3(0, 0, -1), hitboxRadius, glm::vec3(0, 1, 0));
    AddMeshToList(circle3);

    //grass
    Mesh* rectangle4 = object2D::CreateRectangle("rectangle4", grassLayer, static_cast<float>(windowSizeX), static_cast<float>(windowSizeY) / 5, glm::vec3(0, 1, 0), true);
    AddMeshToList(rectangle4);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.74f, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}




void Tema1::Update(const float deltaTimeSeconds)
{

	// duck rotation parameters
	speedX = cos(duckHeadingDirection);
    speedY = sin(duckHeadingDirection);

    translateX += deltaTimeSeconds * speedX * initialSpeed * speedModifier;
    translateY += deltaTimeSeconds * speedY * initialSpeed * speedModifier;

    duckPositionX += deltaTimeSeconds * initialSpeed * speedX * speedModifier;
    duckPositionY += deltaTimeSeconds * initialSpeed * speedY * speedModifier;


    // duck wings animation parameters
    wingsAngularStep += deltaTimeSeconds * 0.75f * static_cast<float>(rotationDirection);
    if (wingsAngularStep >= PI / 8.0f)
        rotationDirection = -1;
    if (wingsAngularStep <= - PI / 8.0f)
        rotationDirection = 1;

    // duck collision with screen edges
    if (duckPositionY >= static_cast<float>(windowSizeY)) {

        // check if duck has evaded
        if (duckEvade) {
            respawnDuck();

            duckEvade = false;
            lives--;
        } else {
	        duckHeadingDirection = -duckHeadingDirection;
        }
        
    }

    if (duckPositionY <= 0) {

        // check if duck was shot
        if (duckDead) {
            respawnDuck();
            duckDead = false;
        } else {
	        duckHeadingDirection = -duckHeadingDirection;
        }
        
    }

    if (duckPositionX >= static_cast<float>(windowSizeX)) {
        duckHeadingDirection = glm::radians(180.0f) - duckHeadingDirection;
    }

    if (duckPositionX <= 0) {
        duckHeadingDirection = glm::radians(180.0f) - duckHeadingDirection;
    }

    // game restart when lives are 0
    if (lives == 0) {
        gameReset();
    }

    //score, lives and bullets update
    if (!duckEvade && !duckDead) {
		timePassed += deltaTimeSeconds;
    	if (timePassed >= maxTimeLevel) {
	        duckEvade = true;
	        duckHeadingDirection = glm::radians(90.0f);
	        timePassed = 0;
		}
    }

    if (duckShot) {
        timePassed = 0;
    	duckHeadingDirection = glm::radians(270.0f);
        score += 6;
        ducksShot++;
        duckShot = false;
        duckDead = true;

        if (ducksShot % 5 == 0) {
            speedModifier *= 1.2f;
            maxTimeLevel = max(maxTimeLevel - 0.5f, 3.0f);
        }
    }

    if (bullets == 0 && !duckDead) {
        duckEvade = true;
        duckHeadingDirection = glm::radians(90.0f);
        bullets = -1;
    }

    //duck movements and animations
    computeModelMatricesDuck();

	// render UI
    renderUI();

    // render duck
    renderDuck();
}


void Tema1::FrameEnd()
{
}

float Tema1::getRandomAngle() const {
    random_device rd;
    default_random_engine dre(rd());
    uniform_real_distribution<float> distribution(20.0f, 160.0f);
    return glm::radians(distribution(dre));
}

void Tema1::respawnDuck() {
    duckPositionX = duckSpawnX;
    duckPositionY = duckSpawnY;
    duckCenterX = duckPositionX + duckLength + 20;
    duckCenterY = duckPositionY + duckLength / 2.0f;

    duckPositionX = duckCenterX;
    duckPositionY = duckCenterY;
    duckHeadingDirection = getRandomAngle();

    translateX = 0;
    translateY = 0;

    bullets = maxBullets;
}

void Tema1::gameReset () {
    bullets = maxBullets;
    lives = maxLives;
    score = 0;
    ducksShot = 0;

    timePassed = 0;
    maxTimeLevel = 7.5;
    speedModifier = 1.0;
    duckEvade = false;
    duckShot = false;
    duckDead = false;

    duckHeadingDirection = getRandomAngle();
}

void Tema1::computeModelMatricesDuck() {
    modelMatrixBody = glm::mat3(1.0);
    modelMatrixWing1 = glm::mat3(1.0);
    modelMatrixWing2 = glm::mat3(1.0);
    modelMatrixBeak = glm::mat3(1.0);
    modelMatrixHead = glm::mat3(1.0);
    modelMatrixHitbox = glm::mat3(1.0);

    modelMatrixBody *= transform2D::Translate(translateX, translateY);
    modelMatrixWing1 *= transform2D::Translate(translateX, translateY);
    modelMatrixWing2 *= transform2D::Translate(translateX, translateY);
    modelMatrixBeak *= transform2D::Translate(translateX, translateY);
    modelMatrixHead *= transform2D::Translate(translateX, translateY);
    modelMatrixHitbox *= transform2D::Translate(translateX, translateY);

    modelMatrixBody *= transform2D::Translate(duckCenterX, duckCenterY);
    modelMatrixWing1 *= transform2D::Translate(duckCenterX, duckCenterY);
    modelMatrixWing2 *= transform2D::Translate(duckCenterX, duckCenterY);
    modelMatrixBeak *= transform2D::Translate(duckCenterX, duckCenterY);
    modelMatrixHead *= transform2D::Translate(duckCenterX, duckCenterY);
    modelMatrixHitbox *= transform2D::Translate(duckCenterX, duckCenterY);

    modelMatrixBody *= transform2D::Rotate(duckHeadingDirection);
    modelMatrixWing1 *= transform2D::Rotate(duckHeadingDirection);
    modelMatrixWing2 *= transform2D::Rotate(duckHeadingDirection);
    modelMatrixBeak *= transform2D::Rotate(duckHeadingDirection);
    modelMatrixHead *= transform2D::Rotate(duckHeadingDirection);
    modelMatrixHitbox *= transform2D::Rotate(duckHeadingDirection);

    modelMatrixBody *= transform2D::Translate(-duckCenterX, -duckCenterY);
    modelMatrixWing1 *= transform2D::Translate(-duckCenterX, -duckCenterY);
    modelMatrixWing2 *= transform2D::Translate(-duckCenterX, -duckCenterY);
    modelMatrixBeak *= transform2D::Translate(-duckCenterX, -duckCenterY);
    modelMatrixHead *= transform2D::Translate(-duckCenterX, -duckCenterY);
    modelMatrixHitbox *= transform2D::Translate(-duckCenterX, -duckCenterY);


    modelMatrixBody *= transform2D::Translate(duckSpawnX, duckSpawnY);

    modelMatrixWing1 *= transform2D::Translate(duckSpawnX + duckLength, duckSpawnY + duckLength / 2);
    modelMatrixWing1 *= transform2D::Rotate(wingsAngularStep);
    modelMatrixWing1 *= transform2D::Rotate(PI / 2.0f);
    modelMatrixWing1 *= transform2D::Translate(0, -duckLength / 4);

    modelMatrixWing2 *= transform2D::Translate(duckSpawnX + duckLength, duckSpawnY + duckLength / 2);
    modelMatrixWing2 *= transform2D::Rotate(-wingsAngularStep);
    modelMatrixWing2 *= transform2D::Rotate(-PI / 2.0f);
    modelMatrixWing2 *= transform2D::Translate(0, -duckLength / 4);

    modelMatrixBeak *= transform2D::Translate(duckSpawnX + duckLength * 2, duckSpawnY + duckLength / 4);

    modelMatrixHead *= transform2D::Translate(duckSpawnX + duckLength * 2, duckSpawnY + duckLength / 2);

    //hitbox
    modelMatrixHitbox *= transform2D::Translate(duckCenterX, duckCenterY);
}

void Tema1::renderUI() {
    auto modelMatrixBullet = glm::mat3(1.0);
    modelMatrixBullet *= transform2D::Translate(static_cast<float>(windowSizeX) - 10 - bulletLength, static_cast<float>(windowSizeY) - 10 - bulletHeight);

    for (int i = 0; i < bullets; i++) {
        RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrixBullet);
        modelMatrixBullet *= transform2D::Translate(-2.0f * bulletLength, 0);
    }

    auto modelMatrixLive = glm::mat3(1.0);
    modelMatrixLive *= transform2D::Translate(static_cast<float>(windowSizeX) - 10 - (bulletLength + 10) * static_cast<float>(maxBullets) - 2 * liveRadius, static_cast<float>(windowSizeY) - 10 - liveRadius);

    for (int i = 0; i < lives; i++) {
        RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrixLive);
        modelMatrixLive *= transform2D::Translate(-2 * liveRadius - 10, 0);
    }

    auto modelMatrixScoreWireframe = glm::mat3(1.0);
    modelMatrixScoreWireframe *= transform2D::Translate(static_cast<float>(windowSizeX) - 10 - static_cast<float>(maxScore) - 1, static_cast<float>(windowSizeY) - 20 - 2 * bulletHeight - 1);

    RenderMesh2D(meshes["rectangle2"], shaders["VertexColor"], modelMatrixScoreWireframe);

    auto modelMatrixScoreInner = glm::mat3(1.0);

    modelMatrixScoreInner *= transform2D::Translate(static_cast<float>(windowSizeX) - 10 - static_cast<float>(maxScore), static_cast<float>(windowSizeY) - 20 - 2 * bulletHeight);
    modelMatrixScoreInner *= transform2D::Scale(min(static_cast<float>(score) / static_cast<float>(maxScore), 1.0f), 1);

    RenderMesh2D(meshes["rectangle3"], shaders["VertexColor"], modelMatrixScoreInner);
}

void Tema1::renderDuck() {
    RenderMesh2D(meshes["triangle1"], shaders["VertexColor"], modelMatrixBody);
    RenderMesh2D(meshes["triangle2"], shaders["VertexColor"], modelMatrixWing1);
    RenderMesh2D(meshes["triangle2"], shaders["VertexColor"], modelMatrixWing2);
    RenderMesh2D(meshes["triangle3"], shaders["VertexColor"], modelMatrixBeak);
    RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrixHead);
    if (showHitbox)
        RenderMesh2D(meshes["circle3"], shaders["VertexColor"], modelMatrixHitbox);

    RenderMesh2D(meshes["rectangle4"], shaders["VertexColor"], glm::mat3(1));
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(const int mouseX, const int mouseY, int button, int mods)
{
    // Add mouse button press event
    const auto worldMouseX = static_cast<float>(mouseX);
    const auto worldMouseY = static_cast<float>(windowSizeY - mouseY);
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && !duckEvade && !duckDead) {
        bullets--;
        const float dist = (worldMouseX - duckPositionX) * (worldMouseX - duckPositionX) + (worldMouseY - duckPositionY) * (worldMouseY - duckPositionY);
        if (dist <= hitboxRadius * hitboxRadius) {
            duckShot = true;
        }
    	
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
