#include "Gamelogic/ILevel.h"
#include "Gamelogic/Cube.h"
#include <vector>

class Level : public ILevel
{
public:
	Level();
	~Level();

	bool RotateX(const int iFactor);
	bool RotateY(const int iFactor);
	bool RotateZ(const int iFactor);

private:
	std::vector<Cube> m_Cubes;
};