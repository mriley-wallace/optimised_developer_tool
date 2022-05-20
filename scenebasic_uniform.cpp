#include "scenebasic_uniform.h"

#include <iostream>
#include <sstream>
using std::cerr;
using std::endl;

#include <glm/glm/gtc/matrix_transform.hpp>

#include "helper/glutils.h"

using glm::vec3;
using glm::mat4;


SceneBasic_Uniform::SceneBasic_Uniform() : rotation(0.0f), plane(20.0f, 20.0f, 100, 100){


    pigmesh = ObjMesh::load("../c1-prototype/media/pig_triangulated.obj",
        true);

    farmer = ObjMesh::load("../c1-prototype/media/Casual_Male.obj",
        true);

    bluebarn = ObjMesh::load("../c1-prototype/media/barn-low.obj",
        true);
}

void SceneBasic_Uniform::initScene()
{

    compile();
    
    glEnable(GL_DEPTH_TEST);

    projection = mat4(1.0f);

    prog.setUniform("Lights[0].L", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Lights[1].L", 0.8f, 0.8f, 0.8f);
    prog.setUniform("Lights[2].L", 0.8f, 0.8f, 0.8f);

    prog.setUniform("Lights[0].La", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Lights[1].La", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Lights[2].La", 0.5f, 0.5f, 0.5f);

   
    
    prog.setUniform("Lights[0].Position", 1.0f, 2.0f, 1.0f);
    prog.setUniform("Lights[1].Position", 3.0f, 1.0f, 1.0f);
    prog.setUniform("Lights[2].Position", 4.0f, 1.0f, 1.0f);


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



}

void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}


void SceneBasic_Uniform::setMatrices() {


    mat4 mv = view * model;

    prog.setUniform("ModelViewMatrix", mv);

    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    prog.setUniform("MVP", projection * mv);


}



void SceneBasic_Uniform::update( float t )
{
    rotation = t;
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view = glm::lookAt(vec3(-8.0f, 6.0f, 0.0f), vec3(-2.0f, -1.0f, 1.0f), vec3(0.0f, 2.0f, 0.0f));
    view = glm::rotate(view, glm::radians(15.0f * rotation), vec3(0.0f, 1.0f, 0.0f));

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

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
    projection = glm::perspective(glm::radians(70.0f), (float)w / h,
        0.3f, 100.0f);

}


