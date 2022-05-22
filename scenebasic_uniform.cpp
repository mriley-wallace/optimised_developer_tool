#include "scenebasic_uniform.h"

#include <iostream>
#include <sstream>
using std::cerr;
using std::endl;

#include <glm/glm/gtc/matrix_transform.hpp>

#include "helper/glutils.h"
#include <helper/particleutils.h>

using glm::vec3;
using glm::mat4;

int sceneChanger = 0;


SceneBasic_Uniform::SceneBasic_Uniform() : time(0), particleLifetime(50.5f), nParticles(500), emitterPos(1,0,0), emitterDir(-1,2,0),rotation(5.0f), plane(20.0f, 20.0f, 100, 100), angleSky(0.0f), tPrevSky(0.0f), rotSpeedSky(glm::pi<float>()/8.0f), sky(100.0f){

    pigmesh = ObjMesh::load("../optimised_developer_tool/media/pig_triangulated.obj",
        true);

    farmer = ObjMesh::load("../optimised_developer_tool/media/Casual_Male.obj",
        true);

    bluebarn = ObjMesh::load("../optimised_developer_tool/media/barn-low.obj",
        true);
}

void SceneBasic_Uniform::initScene()
{

    compile();
  
    if (sceneChanger == 0) {
        initNormal();
       
    }
    else if (sceneChanger == 1) {
        initEdge();
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brick1);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pigskin1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, grass);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, farmerskin);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, barn);


    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, smoke);
  
}

void SceneBasic_Uniform::compile()
{

    if (sceneChanger == 0) {
        try {
            prog.compileShader("shader/basic_uniform.vert");
            prog.compileShader("shader/basic_uniform.frag");
            prog.link();
            prog.use();
            partProg.compileShader("shader/particle.vert");
            partProg.compileShader("shader/particle.frag");
            partProg.link();
            partProg.use();
            flatProg.compileShader("shader/flat_frag.glsl");
            flatProg.compileShader("shader/flat_vert.glsl");
            flatProg.link();
        }
        catch (GLSLProgramException& e) {
            cerr << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }
    else if (sceneChanger == 1) {
        try {
            prog.compileShader("shader/edge.vert");
            prog.compileShader("shader/edge.frag");
            prog.link();
            prog.use();
        }
        catch (GLSLProgramException& e) {
            cerr << e.what() << endl;
            exit(EXIT_FAILURE);
        }
    }
}


void SceneBasic_Uniform::setMatrices() {

    mat4 mv = view * model;
    prog.use();
    prog.setUniform("ModelViewMatrix", mv);

    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    prog.setUniform("MVP", projection * mv);
}



void SceneBasic_Uniform::update( float t)
{
    rotation = t;
    time = t;

    angle = std::fmod(angle + 0.01f, glm::two_pi<float>());
}

void SceneBasic_Uniform::render()
{
    if (sceneChanger == 0){
        renderNormal();
    }
    else if (sceneChanger == 1) {
        renderEdge();
    }
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    projection = glm::perspective(glm::radians(70.0f), (float)w / h,
        0.3f, 100.0f);

}

void SceneBasic_Uniform::setupFBO()
{
    // Generate and bind the framebuffer
    glGenFramebuffers(1, &fboHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    // Create the texture object
    glGenTextures(1, &renderTex);
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
    // Bind the texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        renderTex, 0);
    // Create the depth buffer
    GLuint depthBuf;
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    // Bind the depth buffer to the FBO
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, depthBuf);
    // Set the targets for the fragment output variables
    GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);
    // Unbind the framebuffer, and revert to default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SceneBasic_Uniform::pass1()
{
    prog.use();
    prog.setUniform("Pass", 1);
    glBindFramebuffer(GL_FRAMEBUFFER, fboHandle);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    view = glm::lookAt(vec3(-10.0f, 10.0f, 0.0f), vec3(-2.0f, -1.0f, 1.0f), vec3(0.0f, 2.0f, 0.0f));
    view = glm::rotate(view, glm::radians(30.0f * rotation), vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(60.0f), (float)width / height,
        0.3f, 100.0f);

        //This is the pig
     prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 1);
    model = mat4(1.0f);
    /*model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 0.5f, 0.0f));*/
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::translate(model, vec3(4.0f, 4.3f, 0.0f));
    setMatrices();
    pigmesh->render();


    //This is the farmer
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 3);
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 0.5f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.5f, 0.0f));
    setMatrices();
    farmer->render();

    //This is the barn
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 4);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-6.0f, 3.0f, 0.0f));
    setMatrices();
    bluebarn->render();



    //This is the floor
    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 2);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    plane.render();



    //These are the cubes
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 0);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, 0.5f, -2.0f));
    model = glm::rotate(model, glm::radians(15.0f * rotation), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    cube.render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, 1.45f, 0.0f));
    model = glm::rotate(model, glm::radians(15.0f * rotation), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    cube.render();


    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, 2.0f, 2.0f));
    model = glm::rotate(model, glm::radians(15.0f * rotation), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    cube.render();
}


void SceneBasic_Uniform::pass2()
{
    prog.setUniform("Pass", 2);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderTex);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    model = mat4(1.0f);
    view = mat4(1.0f);
    projection = mat4(1.0f);
    setMatrices();
    // Render the full-screen quad
    glBindVertexArray(fsQuad);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void SceneBasic_Uniform::initEdge() {

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    projection = mat4(1.0f);
    angle = glm::pi<float>() / 4.0f;
    setupFBO();
    // Array for full-screen quad
    GLfloat verts[] = {
    -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f
    };
    GLfloat tc[] = {
    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    // Set up the buffers
    unsigned int handle[2];
    glGenBuffers(2, handle);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(float), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), tc, GL_STATIC_DRAW);
    // Set up the vertex array object
    glGenVertexArrays(1, &fsQuad);
    glBindVertexArray(fsQuad);
    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0); // Vertex position
    glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2); // Texture coordinates
    glBindVertexArray(0);
    prog.setUniform("EdgeThreshold", 0.01f);
    prog.setUniform("Lights.L", vec3(1.0f));
    prog.setUniform("Lights.La", vec3(0.2f));
}

void SceneBasic_Uniform::initNormal() {



    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);
    prog.use();
    prog.setUniform("Lights[0].L", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Lights[1].L", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Lights[2].L", 0.8f, 0.8f, 0.8f);

    prog.setUniform("Lights[0].La", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Lights[1].La", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Lights[2].La", 0.5f, 0.5f, 0.5f);



    prog.setUniform("Lights[0].Position", 1.0f, 2.0f, 1.0f);
    prog.setUniform("Lights[1].Position", 3.0f, 1.0f, 1.0f);
    prog.setUniform("Lights[2].Position", 4.0f, 1.0f, 1.0f);


    initParticles();


}


void SceneBasic_Uniform::renderNormal() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(vec3(-16.0f, 12.0f, 0.0f), vec3(-2.0f, -1.0f, 1.0f), vec3(0.0f, 2.0f, 0.0f));
    view = glm::rotate(view, glm::radians(15.0f * rotation), vec3(0.0f, 1.0f, 0.0f));

    //This is the pig
    prog.use();
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 1);
    model = mat4(1.0f);
    /*model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 0.5f, 0.0f));*/
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::translate(model, vec3(4.0f, 4.3f, 0.0f));
    setMatrices();
    pigmesh->render();


    //This is the farmer
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 3);
    model = mat4(1.0f);
    model = glm::rotate(model, glm::radians(90.0f), vec3(0.0f, 0.5f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.5f, 0.0f));
    setMatrices();
    farmer->render();

    //This is the barn
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 4);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-6.0f, 3.0f, 0.0f));
    setMatrices();
    bluebarn->render();



    //This is the floor
    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Shininess", 1000.0f);
    prog.setUniform("Tex1", 2);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    plane.render();



    //These are the cubes
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Tex1", 0);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, 0.5f, -2.0f));
    model = glm::rotate(model, glm::radians(15.0f * rotation), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    cube.render();

    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, 1.45f, 0.0f));
    model = glm::rotate(model, glm::radians(15.0f * rotation), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    cube.render();


    model = mat4(1.0f);
    model = glm::translate(model, vec3(2.0f, 2.0f, 2.0f));
    model = glm::rotate(model, glm::radians(15.0f * rotation), vec3(0.0f, 1.0f, 0.0f));
    setMatrices();
    cube.render();


    //This is the smoke trail
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-7.5f, 6.5f, 1.0f));
    flatProg.use();
    setGLSLMatrices(flatProg);
    grid.render();
    glDepthMask(GL_FALSE);
    partProg.use();
    setGLSLMatrices(partProg);
    partProg.setUniform("Time", time);
    partProg.setUniform("ParticleTex", 5);
    glBindVertexArray(particles);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticles);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);



    


}


void SceneBasic_Uniform::renderEdge() {
    pass1();
    glFlush();
    pass2();
}

void SceneBasic_Uniform::initParticles() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    angle = glm::half_pi<float>();

    initBuffers();


    partProg.use();
    partProg.setUniform("ParticleTex", 0);
    partProg.setUniform("ParticleLifetime", particleLifetime);
    partProg.setUniform("ParticleSize", 0.05f);
    partProg.setUniform("Gravity", vec3(0.0f, -0.2f, 0.0f));
    partProg.setUniform("EmitterPos", emitterPos);

    flatProg.use();
    flatProg.setUniform("Color", glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
   
}

void SceneBasic_Uniform::initBuffers() {

    glGenBuffers(1, &initVel);
    glGenBuffers(1, &startTime);

    int size = nParticles * sizeof(float);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferData(GL_ARRAY_BUFFER, size * 3, 0, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);


    glm::mat3 emitterBasis = ParticleUtils::makeArbitraryBasis(emitterDir);
    vec3 v(0.0f);
    float velocity, theta, phi;
    std::vector<GLfloat>data(nParticles * 3);
    for (uint32_t i = 0; i < nParticles; i++) {
        theta = glm::mix(0.0f, glm::pi<float>() / 20.0f, randFloat());
        phi = glm::mix(0.0f, glm::two_pi<float>(), randFloat());

        v.x = sinf(theta) * cosf(phi);
        v.y = cosf(theta);
        v.z = sinf(theta) * sinf(phi);


        velocity = glm::mix(1.25f, 1.5f, randFloat());
        v = glm::normalize(emitterBasis * v) * velocity;

        data[3 * i] = v.x;
        data[3 * i + 1] = v.y;
        data[3 * i + 2] = v.z;
    }

    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size * 3, data.data());

    float rate = particleLifetime / nParticles;

    for (int i = 0; i < nParticles; i++) {
        data[i] = rate * i;
    }
    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data.data());

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &particles);
    glBindVertexArray(particles);
    glBindBuffer(GL_ARRAY_BUFFER, initVel);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, startTime);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 1);

    glBindVertexArray(0);
}

float SceneBasic_Uniform::randFloat() {
    return rand.nextFloat();
}

void SceneBasic_Uniform::setGLSLMatrices(GLSLProgram& p) {
    mat4 mv = view * model;
    p.use();
    p.setUniform("MV", mv);
    p.setUniform("Proj", projection);
}

