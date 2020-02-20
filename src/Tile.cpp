#include "Tile.h"
#include "Game.h"
#include "Util.h"
#include <iomanip>
#include <sstream>

Tile::Tile(glm::vec2 worldPosition, glm::vec2 gridPosition):
	m_gridPosition(gridPosition)
{
	TheTextureManager::Instance()->load("../Assets/textures/tile.png",
		"tile", TheGame::Instance()->getRenderer());

	glm::vec2 size = TheTextureManager::Instance()->getTextureSize("tile");
	setWidth(size.x);
	setHeight(size.y);
	setPosition(worldPosition);

	std::ostringstream tempLabel;
	tempLabel << std::fixed << std::setprecision(1) <<  m_tileValue;
	std::string labelstring = tempLabel.str();

	SDL_Color black{ 0, 0, 0, 255 };
	glm::vec2 closedOpenLabelPosition = glm::vec2(getPosition().x, getPosition().y - 5);
	m_pClosedOpenLabel = new Label("-", "Consolas", 12, black, closedOpenLabelPosition);

	glm::vec2 valueLabelPosition = glm::vec2(getPosition().x, getPosition().y + 10);
	m_pValueLabel = new Label(labelstring, "Consolas", 14, black, valueLabelPosition, true);

	m_pNeighbours = { nullptr, nullptr, nullptr, nullptr };
}

Tile::~Tile()
{
	delete m_pValueLabel;
	delete m_pClosedOpenLabel;
	m_pNeighbours.clear();
	m_pNeighbours.resize(0);
	m_pNeighbours.shrink_to_fit();
}

void Tile::draw()
{
	int xComponent = getPosition().x;
	int yComponent = getPosition().y;

	TheTextureManager::Instance()->draw("tile", xComponent, yComponent,
		TheGame::Instance()->getRenderer(), 0, 255, true);

	m_pClosedOpenLabel->draw();
	m_pValueLabel->draw();
}

void Tile::update()
{
}

void Tile::clean()
{
	
}

Tile * Tile::up()
{
	return m_pNeighbours[UP];
}

Tile * Tile::down()
{
	return m_pNeighbours[DOWN];
}

Tile * Tile::right()
{
	return m_pNeighbours[RIGHT];
}

Tile * Tile::left()
{
	return m_pNeighbours[LEFT];
}

void Tile::setUp(Tile * tile)
{
	m_pNeighbours[UP] = tile;
}

void Tile::setDown(Tile * tile)
{
	m_pNeighbours[DOWN] = tile;
}

void Tile::setRight(Tile * tile)
{
	m_pNeighbours[RIGHT] = tile;
}

void Tile::setLeft(Tile * tile)
{
	m_pNeighbours[LEFT] = tile;
}

void Tile::setTileState(TileState state)
{
	m_tileState = state;

	switch(state)
	{
	case OPEN:
		setTileStateLabel("O");
		break;
	case CLOSED:
		setTileStateLabel("C");
		break;
	case START:
		setTileStateLabel("S");
		break;
	case GOAL:
		setTileStateLabel("G");
		break;
	case UNDEFINED:
		setTileStateLabel("-");
		break;
	}

	if (state == GOAL)
	{
		m_tileValue = 0;
	}
}

TileState Tile::getTileState()
{
	return m_tileState;
}

void Tile::setTargetDistance(glm::vec2 goalLocation)
{
	m_goalLocation = goalLocation;

	// use euclidean distance heuristic
	//float h = Util::distance(getGridPosition(), goalLocation);

	// manhattan distance heuristic
	float h = abs(getGridPosition().x - goalLocation.x) +
		      abs(getGridPosition().y - goalLocation.y);

	float g = Config::TILE_COST;
	
	m_tileValue = g + h;

	std::ostringstream tempLabel;
	tempLabel << std::fixed << std::setprecision(1) << m_tileValue;
	std::string labelstring = tempLabel.str();
	m_pValueLabel->setText(labelstring);
	
}

glm::vec2 Tile::getGridPosition()
{
	return m_gridPosition;
}

float Tile::getTileValue()
{
	return m_tileValue;
}

void Tile::setTileStateLabel(std::string closedOpen)
{
	m_pClosedOpenLabel->setText(closedOpen);

	SDL_Color blue = { 0, 0, 255, 255 };
	m_pClosedOpenLabel->setColour(blue);
}

std::vector<Tile*> Tile::getNeighbours()
{
	return m_pNeighbours;
}
