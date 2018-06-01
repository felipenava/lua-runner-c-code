# lua-runner-c-code
A manager for the LUA call functions in C / C ++ inside a lua script.

This example shows the interaction between LUA calls to C ++ and C ++ to LUA

This manager we add lua script and then we have a function in C that takes the size of the file (implementation for windows), via lua script we ask the code in C/C ++ to execute a certain task .. in this case get the size of the file , the code in C/C ++ does the work and returns the code in LUA.
