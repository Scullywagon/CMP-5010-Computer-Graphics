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
    vector<Mesh> meshes;

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

        meshes.push_back(Mesh(vertices, indices, textures));
    }

  private:
    unsigned int TextureFromFile(const char *path, const string &directory,
                                 bool gamma = false)
    {
        cout << "Loading texture: " << path << endl;
        string filename = string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data =
            stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                         GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                            GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
            cout << "Texture loaded at path: " << path << endl;
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};

#endif
