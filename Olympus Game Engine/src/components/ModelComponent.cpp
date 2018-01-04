#include "ModelComponent.h"

ModelComponent::ModelComponent() {

	std::string inputfile = "models/sponza/sponza.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err; 
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	//loop over shapes 
	for (size_t i = 0; i < shapes.size(); i++) {
		//loop faces (polygons)
		size_t index_offset = 0;

		tinyobj::shape_t& s = shapes[i];
		tinyobj::mesh_t m = s.mesh;

		for (size_t j = 0; j < m.indices.size(); j += 3) {
			tinyobj::index_t i0 = m.indices[j + 0];
			tinyobj::index_t i1 = m.indices[j + 1];
			tinyobj::index_t i2 = m.indices[j + 2];

			m_vertices.push_back(attrib.vertices[i0.vertex_index * 3 + 0]);
			m_vertices.push_back(attrib.vertices[i0.vertex_index * 3 + 1]);
			m_vertices.push_back(attrib.vertices[i0.vertex_index * 3 + 2]);
		}

		/*
		tinyobj::shape_t& s = shapes[i]
		tinyobj::mesh_t m = s.mesh;

		for(size_t j = 0; j < m.)

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];
			//loop over vertices in the face 

			for (size_t v = 0; v < fv; v++) {
				//access to vertex 
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx = attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty = attrib.texcoords[2 * idx.texcoord_index + 1];

				m_vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
				m_vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
				m_vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

				normals.push_back(nx);
				normals.push_back(ny);
				normals.push_back(nz);

				textureCoordinates.push_back(tx);
				textureCoordinates.push_back(ty);
		}

		*/
	}

	m_shader = ResourceManager::Instance()->loadShader("src/shaders/cube.vs", "src/shaders/cube.fs");

	//Create the appropriate buffers for the cube
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices) + sizeof(textureCoordinates) + sizeof(normals), NULL, GL_STATIC_DRAW);
	//attributes for shader
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices), &m_vertices[0]);
	//offset for texture coordinates (location = 1), normalData location = 2
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices), sizeof(textureCoordinates), &textureCoordinates[0]);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices) + sizeof(textureCoordinates), sizeof(normals), &normals[0]);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(m_vertices));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(m_vertices) + sizeof(textureCoordinates)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);	
}

void ModelComponent::update(Entity& entity){
	m_shader->use();
	m_shader->setMat4("view", Camera::Instance()->getViewMatrix());
	m_shader->setMat4("projection", Settings::Instance()->projection);
	glm::mat4 model;
	model = glm::translate(model, entity.getPosition());
	m_shader->setMat4("model", model);
	glBindVertexArray(VAO);
	sendToRenderer(GL_TRIANGLES, 0, m_vertices.size());

}
void ModelComponent::postInit(Entity& entity){}