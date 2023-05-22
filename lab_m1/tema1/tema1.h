#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1() override;
        

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        float getRandomAngle() const;
    	void respawnDuck();
    	void gameReset();
		void computeModelMatricesDuck();
        void renderUI();
		void renderDuck();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        

     protected:
        const float initialSpeed = 150;
        const float duckSpawnX = 300, duckSpawnY = 50;

        glm::mat3 modelMatrixBody{}, modelMatrixHead{}, modelMatrixWing1{}, modelMatrixWing2{}, modelMatrixBeak{};
        glm::mat3 modelMatrixHitbox{};
        float translateX{}, translateY{};

        float timePassed{};
        bool duckEvade{}, duckShot{}, duckDead{};

        float maxTimeLevel{};

        float duckPositionX{}, duckPositionY{};
        float duckCenterX{}, duckCenterY{};
        const float PI = M_PI;
        float duckLength{};
        float bulletLength{}, bulletHeight{};
        float liveRadius{};
        float wingsAngularStep{};
        int rotationDirection{};
        float duckHeadingDirection{};
        float speedX{}, speedY{};
        float speedModifier{};
        float hitboxRadius{};

        int windowSizeX{}, windowSizeY{};

        int bullets{}, lives{}, score{}, ducksShot{};
        const int maxBullets = 3, maxLives = 3, maxScore = 250;

        bool showHitbox = false;

    };
}   // namespace m1
