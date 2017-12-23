#include "MessageBus.h"

MessageBus* MessageBus::m_pInstance = NULL;

MessageBus* MessageBus::Instance() {
	if (!m_pInstance)
		m_pInstance = new MessageBus();
	return m_pInstance;
}