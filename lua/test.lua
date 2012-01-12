io.write("Running ", _VERSION, "\n")

PrintDebugMessageLuaManager("Calling PrintDebugMessageLuaManager from LUA")
PrintDebugMessageMain("Calling PrintDebugMessageMain from LUA")

a = Grid()
a:AddDoor(1,1)
a:PrintGrid()

function retGrid()
	
	return a
end