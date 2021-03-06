#ifndef MODEL_H
#define MODEL_H
#include "../core/Shader.h"
#include "Mesh.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../utilities/stb_image.h"

#include "../core/ResourceManager.h"


unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model
{
public:
	std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Mesh> meshes;
	std::string directory;
	int mesh_id;

	bool gammaCorrection;
	Model();
	void setFilepath(char *path) {
		loadModel(path);
	}
	Model(char *path)
	{
		loadModel(path);
	}
	void Draw(Shader *shader) {
		int vertCount = 0;
		for (auto mesh : meshes) {
			mesh.Draw(shader);
			vertCount += mesh.vertices.size();
		}
	}

	std::vector<Mesh> getMesh() {
		return meshes;
	}
private:
	void loadModel(std::string path) {
		mesh_id = 0;
		// read file via ASSIMP
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace |aiProcess_GenNormals);
		//error checking {if root node and scene are null then we got problems}
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			std::cout << "ASSIMP:: ERROR" << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		// process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode *node, const aiScene *scene) {
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
			processNode(node->mChildren[i], scene);
	}

	Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		mesh_id++;

		//Iterate each of mesh vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
							  // positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;

				vertex.TexCoords = vec;
			}
			//else
			//	vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}
	
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		//std::vector<unsigned int> diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		//std::vector<unsigned int> specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		//std::vector<unsigned int> normalTextures = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		//std::vector<unsigned int> displacementTextures = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_displacement");
		//std::vector<unsigned int> textu = loadMaterialTextures(material, aiTextureType_EMISSIVE, "map_ab");

		std::vector<unsigned int> baseColour = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		std::vector<unsigned int> displacementMap = loadMaterialTextures(material, aiTextureType_OPACITY, "texture_displacement");
		std::vector<unsigned int> aoMap = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ao");
		std::vector<unsigned int> metallicMap = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_metallic");
		std::vector<unsigned int> normalMap = loadMaterialTextures(material, aiTextureType_HEIGHT,  "texture_normal");
		std::vector<unsigned int> roughnessMap = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_roughness");

		//diffuse texture = 
		//specular texture = 
		//normal texture = normal map 
		
		//opacity: map_d 
		//diffuse: map_Kd
		//ambient: map_Ka
		//specular: map_Ks
		//height (normal): map_bump
		//shininess: map_Ns 


		//opacity: map_d -> heightmap/displacement map 
		//diffuse: map_Kd -> basecolor
		//ambient: map_ka -> ao 
		//specular: map_ks -> metallic 
		//height: map_bump -> normal 
		//shininess: map_ns -> roughness 

		//albedo 
		//metallicness
		//roughness
		//ao 
		//normal 
		return Mesh(vertices, indices, baseColour, displacementMap, aoMap, metallicMap, normalMap, roughnessMap);
		//return Mesh(vertices, indices, diffuseTextures, specularTextures, normalTextures, displacementTextures, mesh_id);
	}


	std::vector<unsigned int> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
		std::vector<unsigned int> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			std::string filename = std::string(str.C_Str());
			filename = this->directory + '/' + filename;

			textures.push_back(ResourceManager::Instance()->loadTexture(filename));
		}	
		return textures;
	}
	
	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)
	{
		std::string filename = std::string(path);
		filename = directory + '/' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
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