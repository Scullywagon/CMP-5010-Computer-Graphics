#ifndef FLOOR_H
#define FLOOR_H

#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "stb_image.h"

class Floor
{
  public:
    unsigned int textureID;
    Mesh *mesh;

    Texture tex;

    float textureScale = 40.0f;
    float floorSize = 100.0f;
    // Large square floor extending to the horizon

    std::vector<Texture> textures;

    std::vector<Vertex> vertices = {
        {{-floorSize, 0.0f, -floorSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{floorSize, 0.0f, -floorSize},
         {0.0f, 1.0f, 0.0f},
         {textureScale, 0.0f}},
        {{floorSize, 0.0f, floorSize},
         {0.0f, 1.0f, 0.0f},
         {textureScale, textureScale}},
        {{-floorSize, 0.0f, floorSize},
         {0.0f, 1.0f, 0.0f},
         {0.0f, textureScale}}};

    // Create a vector of indices
    std::vector<unsigned int> indices = {
        0, 1, 2, // First triangle
        2, 3, 0  // Second triangle
    };

    Floor()
    {
        // textureID =
        //     loadTexture("assets/terrain/FloorTile/Terreno_TileMat_2.png");
        textureID = TextureFromFile("Terreno_TileMat_2.png",
                                    "assets/terrain/FloorTile/", false);
        tex.id = textureID;
        tex.type = "texture_diffuse";
        tex.path = "assets/terrain/FloorTile/Terreno_TileMat_2.png";

        textures.push_back(tex);

        mesh = new Mesh(vertices, indices, textures);
    }

    void use(Shader &shader, bool depthOnly = false)
    {
        mesh->Draw(shader, depthOnly);
    }

  private:
    unsigned int loadTexture(std::string texture)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        int with, height, nrChannels;
        unsigned char *data =
            stbi_load(texture.c_str(), &with, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, with, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);

        return textureID;
    }
};

#endif
