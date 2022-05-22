#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"


#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/cube.h"

#include "helper/texture.h"
#include <helper/random.h>
#include <helper/grid.h>
#include <helper/skybox.h>


class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog, flatProg, partProg, skyProg;

    Random rand;

    Cube cube;
    Plane plane; // plane surface
    std::unique_ptr<ObjMesh> pigmesh; // pig mesh

    std::unique_ptr<ObjMesh> farmer;

    std::unique_ptr<ObjMesh> bluebarn;

    float rotation;

    float angleSky, tPrevSky, rotSpeedSky;

    SkyBox sky;

    GLuint pigskin1 = Texture::loadTexture("./media/texture/pinkskin.png"); // pig texture
    GLuint brick1 = Texture::loadTexture("./media/texture/cartoonWall.png"); // cube texture
    GLuint grass = Texture::loadTexture("./media/texture/cartoonGrass.png"); // plane texture
    GLuint farmerskin = Texture::loadTexture("./media/texture/Casual_male_Albedo.png"); // farmers skin texture
    GLuint barn = Texture::loadTexture("./media/texture/barn.png"); // farmers skin texture
    GLuint smoke = Texture::loadTexture("./media/texture/smoke.png");

    GLuint fsQuad, fboHandle, renderTex;

    GLuint initVel, startTime, particles, nParticles;

    Grid grid;

    glm::vec3 emitterPos, emitterDir;

    float angle, time, particleLifetime;
    float tPrev, rotSpeed;
    void setupFBO();
    void pass1();
    void pass2();

    void setGLSLMatrices(GLSLProgram&);
    void setMatrices();
    void compile();

    void initBuffers();
    float randFloat();



public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t);
    void render();
    void resize(int, int);
    void initEdge();
    void initNormal();
    void renderNormal();
    void renderEdge();
    void initParticles();
};

#endif // SCENEBASIC_UNIFORM_H
