#ifndef SCRIPTMANAGER_H 
#define SCRIPTMANAGER_H
#include <Lua\lua.hpp>
#include <LuaBridge\LuaBridge.h>
#include <iostream>
class ScriptManager {
public: 
	static ScriptManager* Instance();
private:
	ScriptManager();
	static ScriptManager* m_pInstance;
};
#endif