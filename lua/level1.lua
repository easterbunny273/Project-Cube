io.write("LUA: Loading level1...\n")

level1 = Level()
level1:LoadLevelFromXMLFile('config/levels/level1.xml')

io.write("LUA: Creating light \n")
light1 = Light()
io.write("LUA: Created light \n")
io.write("LUA: ", light1:GetObjectType(), "\n")
--io.write("LUA: getting numcubes \n")
--numcubes = level1:GetNumCubes()
--io.write("LUA: returned numcubes \n")
--io.write("LUA: ",numcubes,"\n")

--io.write("LUA: adding light to the level \n")
--level1:AddObject(light1)


function GetLevel()
	return level1
end