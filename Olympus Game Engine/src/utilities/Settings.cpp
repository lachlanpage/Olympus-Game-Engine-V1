#include "Settings.h"

Settings* Settings::m_Instance = 0;

Settings* Settings::Instance() {
	if (m_Instance == 0)
		m_Instance = new Settings();
	return m_Instance;
}

Settings::Settings() {
	projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 10000.0f);
}
