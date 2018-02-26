#ifndef MESH_H 
#define MESH_H 
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "../core/Shader.h"

struct Vertex {
	//used to store data about vertex in mesh
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	//used to organize texture data 
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	//Mesh Data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	int id;
	int isSelected;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, int id) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->id = id;
		isSelected = 0;
		setupMesh();
	}
	void Draw(Shader* shader) {
		// bind appropriate textures
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		//only have diffuse texture at the moment. 		
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			//glActiveTexture(GL_TEXTURE0 + i);
			//need to look into ordering of textures because they may not be in order of diffuse, spec etc. 
			if (i == 0) {
				//diffuse texture
				glActiveTexture(GL_TEXTURE0);
				shader->setInt("texture_diffuse1", 0);
				glBindTexture(GL_TEXTURE_2D, textures[0].id);
			}
			else if (i == 1) {
				//spec texture
				glActiveTexture(GL_TEXTURE1);
				shader->setInt("specular_texture", 1);
				glBindTexture(GL_TEXTURE_2D, textures[1].id);
			}
			//std::string number;
			//std::string name = textures[i].type;
			//if (name == "texture_diffuse")
			//	number = std::to_string(diffuseNr++);
			//else if (name == "texture_specular")
			//	number = std::to_string(specularNr++); // transfer unsigned int to stream
			//else if (name == "texture_normal")
			//	number = std::to_string(normalNr++); // transfer unsigned int to stream
			//else if (name == "texture_height")
			//	number = std::to_string(heightNr++); // transfer unsigned int to stream

													 // now set the sampler to the correct texture unit
			//shader->setInt(name+number, i);
			//glUniform1i(glGetUniformLocation(shader->ID, (name + number).c_str()), i);
			// and finally bind the texture
			//glBindTexture(GL_TEXTURE_2D, textures[i].id);
			//need to look into ordering of textures because they may not be in order of diffuse, spec etc. 
		}

		shader->setInt("isSelected", isSelected);
		
		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// always good practice to set everything back to defaults once configured.
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// vertex Positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// vertex texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		glBindVertexArray(0);
	}
};

#endif