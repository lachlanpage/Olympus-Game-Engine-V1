#include "ScriptManager.h"

//WORK IN PROGRESS 
//CURRENTLY NOT NEEDED 
ScriptManager* ScriptManager::m_pInstance = 0;

ScriptManager* ScriptManager::Instance() {
	if (!m_pInstance)
		m_pInstance = new ScriptManager();
	return m_pInstance;
}

ScriptManager::ScriptManager() {
	//init script manager

	//Test lua
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	if (luaL_loadfile(L, "src/scripts/test.lua")) {
		std::cout << "ERROR LOADING SCRIPT" << std::endl;
	}


	//luabridge::LuaRef sumNumbers = luabridge::getGlobal(L, "sumNumbers");
	//int result = sumNumbers(5, 4);

	//std::cout << result << std::endl;
}



