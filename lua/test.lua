io.write("Running ", _VERSION, "\n")

level1 = Level()
level1:LoadLevelFromXMLFile('config/levels/level1.xml')

cube1 = level1:GetCubeByPosition(0,0,0)

cube1:GetGrid(3):PrintGrid()