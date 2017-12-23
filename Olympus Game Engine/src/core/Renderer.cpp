#include "Renderer.h"

Renderer* Renderer::m_Instance = nullptr;

Renderer* Renderer::Instance() {
	if (m_Instance == nullptr)
		m_Instance = new Renderer();
	return m_Instance;
}

void Renderer::render(GLenum mode, GLint first, GLsizei count) {
	glDrawArrays(mode, first, count);
}

Renderer::Renderer() {

}