#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <vector>

class Quads {
public:
	Quads(float _height, float _width ,float _posx, float _posy);
	~Quads();

	void draw();
	void loadTextures(std::string tex);
	void activateTexture() const;

	const float* getVertices() const;
	const unsigned int* getIndices() const;
	const unsigned int getVerticesSize() const;
	const unsigned int getIndicesSize() const;
	unsigned int getIndexCnt() const;

private:
	float width, height;
	float posx, posy;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int q_vao, q_vbo, q_ebo;

	unsigned int quadTexture;
	
	void buildVertices();
	void buildIndices();
	void CreateQuadsObject();
};