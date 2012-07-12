io.write("LUA: Loading level1...\n")

level1 = Level()
level1:LoadLevelFromXMLFile('config/levels/level1.xml')

io.write("LUA: Creating light 1 \n")
light1 = level1:CreateLight("light1")
light1:SetPosition(-0.2, 0.1, 0.14)
light1:SetLookDirection(1.0, -0.4, -1.0)
light1:SetColor(1.0, 0.0, 0.0)

io.write("LUA: Creating light 2 \n")
light2 = level1:CreateLight("light2")
light2:SetPosition(-0.2, 0.2, -0.14)
light2:SetLookDirection(1.0, -1.1, 0.62)
light2:SetColor(0.0, 1.0, 0.0)

treppe = level1:CreateObject("treppe", "models/bunte-treppe3.dae")
treppe:Scale(0.01, 0.01, 0.01)

sphere = level1:CreateObject("sphere", "models/pool_sphere.dae")
sphere:Translate(0.0, 0.0, 0.05)
sphere:Scale(0.02, 0.02, 0.02)
sphere:ActivateEnvironmentMapping()


function GetLevel()
	return level1
end