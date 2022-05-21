#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"


#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/cube.h"

#include "helper/texture.h"


class SceneBasic_Uniform : public Scene
{
private:
    GLSLProgram prog;

    Cube cube;
    Plane plane; // plane surface
    std::unique_ptr<ObjMesh> pigmesh; // pig mesh

    std::unique_ptr<ObjMesh> farmer;

    std::unique_ptr<ObjMesh> bluebarn;

    float rotation;

    GLuint pigskin1 = Texture::loadTexture("./media/texture/pinkskin.png"); // pig texture
    GLuint brick1 = Texture::loadTexture("./media/texture/cartoonWall.png"); // cube texture
    GLuint grass = Texture::loadTexture("./media/texture/cartoonGrass.png"); // plane texture
    GLuint farmerskin = Texture::loadTexture("./media/texture/Casual_male_Albedo.png"); // farmers skin texture
    GLuint barn = Texture::loadTexture("./media/texture/barn.png"); // farmers skin texture

    GLuint fsQuad, fboHandle, renderTex;

    float angle;
    float tPrev, rotSpeed;
    void setupFBO();
    void pass1();
    void pass2();


    void setMatrices();
    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
