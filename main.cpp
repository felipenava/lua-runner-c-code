#include <string>
#include <fstream>
#include "lua_manager.h"
#include "file_size.h"

typedef enum {
	kFileSize = 1
} ExportedCFunction;

int main() {
	LuaManager lua_manager;
	std::ifstream if_str("path_lua_script.lua");
	std::string lua_code((std::istreambuf_iterator<char>(if_str)), std::istreambuf_iterator<char>());
	lua_manager.AddLuaCode(lua_code);
	lua_manager.AddCFunction(std::to_string(kFileSize), std::shared_ptr<FileSize>(new FileSize()));
	std::string ret;
	lua_manager.CallLuaFunction("Process", "C:\\path\\teste.exe", &ret);
	return 0;
}