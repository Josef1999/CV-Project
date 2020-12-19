#include "Font.h"
#include "utils.h"

void Text::Configure(Shader& text_shader, std::string font)
{
	// Craete text shader

	// Orthographic projection 
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));

	text_shader.use();
	glUniformMatrix4fv(glGetUniformLocation(text_shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	FT_Library ft;
	// Initialize freetype 
	if (FT_Init_FreeType(&ft))
		printf("ERROR: Failed to initialize FreeType Library.\n");
	
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face)) {
		printf("ERROR: Failed to load font.\n");
		return;
	}
	else {
		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Loop through all 128 ASCII characters and retrieve their character glyphs
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load each character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				printf("ERROR: Failed to load glyph.\n");
				continue;
			}

			// Generate and configure texture parameters for each character glyph
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED, 
				face->glyph->bitmap.width, 
				face->glyph->bitmap.rows, 
				0,
				GL_RED, 
				GL_UNSIGNED_BYTE, 
				face->glyph->bitmap.buffer
			);

			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// now store characters for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};

			this->characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Set text properties 
	this->color = glm::vec3(1.0f, 1.0f, 1.0f);
	this->scale = 1.0f;
	this->x = this->y = 0.0f;
}

void Text::Render(Shader& text_shader)
{
	// activate corresponding render state
	text_shader.use();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform3f(glGetUniformLocation(text_shader.ID, "textColor"), this->color.x, this->color.y, this->color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	for (std::string::const_iterator c = this->text.begin(); c != this->text.end(); c++)
	{
		Character ch = this->characters[*c];

		// Calculate origin position of the quad, size of quad, and generate 6 vertices to form 2D quad (glDrawArrays)
		float xpos = this->x + ch.Bearing.x * this->scale;
		float ypos = this->y - (ch.Size.y - ch.Bearing.y) * this->scale;

		float w = ch.Size.x * this->scale;
		float h = ch.Size.y * this->scale;

		// Update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};

		// Render glyph texture on the quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		// Update content of VBO memory and draw quad
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_ZERO, GL_ZERO);
	glDisable(GL_BLEND);
}