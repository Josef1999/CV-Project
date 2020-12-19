#include "Shape.h"
#include "utils.h"

template <typename T>
void addElements(std::vector<T>& ve, const T x, const T y, const T z)
{
    ve.push_back(x);
    ve.push_back(y);
    ve.push_back(z);
}

template <typename T>
void addElements(std::vector<T>& ve, const T x, const T y)
{
    ve.push_back(x);
    ve.push_back(y);
}

Quads::Quads(float _height, float _width, float _posx, float _posy):height(_height), width(_width), posx(_posx), posy(_posy)
{
    /*
     * ÊäÈë²ÎÊý·¶Î§
     *      posx  : 0~SCR_WIDTH
     *      posy  : 0~SCR_HEIGHT
     *      width : 0~1
     *      height: 0~1
     */
    this->posx = (this->posx + 0.5 * width * SCR_WIDTH) / (0.5 * SCR_WIDTH) - 1;
    this->posy = (this->posy + 0.5 * height * SCR_HEIGHT) / (0.5 * SCR_HEIGHT) - 1;

    this->buildVertices();
    this->buildIndices();
}

Quads::~Quads()
{
}

void Quads::draw()
{
    unsigned int vbo, vao, ebo;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, this->getVerticesSize(), this->getVertices(), GL_STATIC_DRAW);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndicesSize(), this->getIndices(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    //  this->CreateQuadsObject();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Quads::loadTextures(std::string tex)
{
    // load and create a texture 
    // -------------------------
    //  unsigned int texture;
    glGenTextures(1, &this->quadTexture);
    glBindTexture(GL_TEXTURE_2D, this->quadTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(tex.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void Quads::activateTexture() const
{
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->quadTexture);
}

void Quads::buildVertices()
{
    //  positions                                                                           colors                                                      textures
    addElements<float>(this->vertices, posx + width, posy + height, 0.0f);      addElements<float>(this->vertices, 1.0f, 0.0f, 0.0f);       addElements<float>(this->vertices, 1.0f, 1.0f);
    addElements<float>(this->vertices, posx + width, posy - height, 0.0f);      addElements<float>(this->vertices, 1.0f, 0.0f, 0.0f);       addElements<float>(this->vertices, 1.0f, 0.0f);
    addElements<float>(this->vertices, posx - width, posy - height, 0.0f);      addElements<float>(this->vertices, 1.0f, 0.0f, 0.0f);       addElements<float>(this->vertices, 0.0f, 0.0f);
    addElements<float>(this->vertices, posx - width, posy + height, 0.0f);      addElements<float>(this->vertices, 1.0f, 0.0f, 0.0f);       addElements<float>(this->vertices, 0.0f, 1.0f);
}

void Quads::buildIndices()
{
    addElements<unsigned int>(this->indices, 0, 1, 3);
    addElements<unsigned int>(this->indices, 1, 2, 3);
}

void Quads::CreateQuadsObject()
{
    glGenVertexArrays(1, (GLuint*)&q_vao);
    glBindVertexArray(q_vao);

    glGenBuffers(1, (GLuint*)&q_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, q_vbo);
    glBufferData(GL_ARRAY_BUFFER, this->getVerticesSize(), this->getVertices(), GL_STATIC_DRAW);

    glGenBuffers(1, (GLuint*)&q_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, q_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndicesSize(), this->getIndices(), GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

const float* Quads::getVertices() const
{
    return this->vertices.data();
}

const unsigned int* Quads::getIndices() const
{
    return this->indices.data();
}

const unsigned int Quads::getVerticesSize() const
{
    return (unsigned int)this->vertices.size() * sizeof(float);
}

const unsigned int Quads::getIndicesSize() const
{
    return (unsigned int)this->indices.size() * sizeof(unsigned int);
}

unsigned int Quads::getIndexCnt() const
{
    return (unsigned int)this->indices.size();
}
