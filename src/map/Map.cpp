#include <iostream>
#include <math.h>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <queue>
#include <set>
#include "Map.hpp"
#include "../util/Util.hpp"
#include "ConnexeMap.hpp"
#include "Pathfinding.hpp"
#include "../effect/Attack.hpp"
#include "../entity/Team.hpp"

Map::Map(int width, int height, int obstacles_count, const std::vector<Team*>& teams) {

	this->width = width;
	this->height = height;

	nb_cells = (width * 2 - 1) * height - (width - 1);

	for (int i = 0; i < nb_cells; i++) {
		auto c = new Cell(this, i);
		cells.push_back(c);
		if (min_x == -1 || c->x < min_x) min_x = c->x;
		if (max_x == -1 || c->x > max_x) max_x = c->x;
		if (min_y == -1 || c->y < min_y) min_y = c->y;
		if (max_y == -1 || c->y > max_y) max_y = c->y;
	}

	sx = max_x - min_x + 1;
	sy = max_y - min_y + 1;

	coord.resize(sx);
	for (int i = 0; i < sx; ++i) {
		coord[i].resize(sy);
	}

	for (int i = 0; i < nb_cells; i++) {
		auto c = cells[i];
		coord[c->x - min_x][c->y - min_y] = c;
	}

//	cout << "map cells: " << cells.size() << endl;

	generate(obstacles_count, teams);
}

Map::~Map() {
	for (auto& c : cells) {
		delete c;
	}
}

void Map::generate(int obstacles_count, const std::vector<Team*>& teams) {

	int nb = 0;
	bool valid = false;

	while (!valid && nb < 64) {

		valid = true;
		type = Util::rand_int(0, 4);

		for (int i = 0; i < obstacles_count; i++) {
			auto c = getCell(Util::rand_int(nb_cells));
			if (c != nullptr && c->available()) {
				int size = Util::rand_int(1, 2);
				int type = Util::rand_int(0, 2);
				if (size == 2) {
					Cell* c2 = Pathfinding::getCellByDir(c, Direction::EAST);
					Cell* c3 = Pathfinding::getCellByDir(c, Direction::SOUTH);
					Cell* c4 = Pathfinding::getCellByDir(c3, Direction::EAST);
					if (c2 == nullptr || c3 == nullptr || c4 == nullptr
						|| !c2->available() || !c3->available() || !c4->available())
						size = 1;
					else {
						c2->setObstacle(0, -1);
						c3->setObstacle(0, -1);
						c4->setObstacle(0, -1);
						obstacles.push_back(c2);
						obstacles.push_back(c3);
						obstacles.push_back(c4);
					}
				}
				c->setObstacle(type, size);
				obstacles.push_back(c);
			}
		}
		ConnexeMap cm(this);
		std::vector<Entity*> entities;

		// Set entities positions
		for (unsigned t = 0; t < teams.size(); ++t) {
			for (Entity* e : teams[t]->entities) {
				Cell* c;
				if (teams.size() == 2) { // 2 teams : 2 sides
					c = getRandomCell(t == 0 ? 1 : 4);
				} else { // 2+ teams : random
					c = getRandomCell();
				}
				if (c == nullptr) {
					// Enable to find a cell, rebuild the map
					valid = false;
					break;
				}
				c->setEntity(e);
				entities.push_back(e);
			}
		}
		if (!valid) break;

		// Check paths
		if (entities.size() > 0) {
			int component = cm.getComponent(entities[0]->cell);
			for (unsigned i = 1; i < entities.size(); i++) {
				if (component != cm.getComponent(entities[i]->cell)) {
					valid = false;
					break;
				}
			}
		}
		nb++;
	}
	assert(valid || "Invalid map! Too much obstacles?");
}

Cell* Map::getCell(int id) {
	if (id < 0 or id >= (int) cells.size()) {
		return nullptr;
	}
	return cells[id];
}

Cell* Map::getCell(int x, int y) {
	return coord[x - min_x][y - min_y];
}

std::vector<Cell*> Map::getObstacles() {
	return obstacles;
}

Cell* Map::getRandomCell() {
	Cell* c = nullptr;
	int security = 0;
	do {
		c = getCell(Util::rand_int(nb_cells));
	} while (!c->available() && security++ < 512);
	return c;
}

Cell* Map::getRandomCell(int part) {
	Cell* c = nullptr;
	int security = 0;
	do {
		int y = Util::rand_int(height - 1);
		int x = Util::rand_int(width / 4);
		int cellid = y * (width * 2 - 1);
		cellid += (part - 1) * width / 4 + x;
		c = getCell(cellid);
	} while (!c->available() && security++ < 512);
	return c;
}

bool Map::canUseAttack(const Cell* caster, const Cell* target, const Attack* attack) const {

	if (target == nullptr || caster == nullptr) {
		return false;
	}
	if (attack->launch_type == LaunchType::CIRCLE) {

		int dist = getCellDistance(caster, target);
		if (attack->min_range > dist || attack->max_range < dist) {
			return false;
		}
		return line_of_sight_attack(caster, target, attack, {caster});

	} else if (attack->launch_type == LaunchType::LINE) {

		if (caster->x != target->x and caster->y != target->y) {
			return false;
		}
		int dist = getCellDistance(caster, target);
		if (attack->min_range > dist || attack->max_range < dist) {
			return false;
		}
		return line_of_sight_attack(caster, target, attack, {caster});
	}
	return false;
}

int Map::getDistance2(const Cell* c1, const Cell* c2) const {
	return (c1->x - c2->x) * (c1->x - c2->x) + (c1->y - c2->y) * (c1->y - c2->y);
}

float Map::getDistance2_float(const Cell* c1, const Cell* c2) const {
	return (c1->x - c2->x) * (c1->x - c2->x) + (c1->y - c2->y) * (c1->y - c2->y);
}

float Map::getDistance2_float(const Cell* c1, const std::vector<const Cell*> cells) const {
	float dist = -1;
	for (const auto& c2 : cells) {
		auto d = getDistance2_float(c1, c2);
		if (dist == -1 or d < dist) {
			dist = d;
		}
	}
	return dist;
}

double Map::getDistance(const Cell* c1, const Cell* c2) const {
	return sqrt(getDistance2(c1, c2));
}

float Map::getDistance_float(const Cell* c1, const Cell* c2) const {
	return sqrt(getDistance2_float(c1, c2));
}

float Map::getDistance_float(const Cell* c1, const std::vector<const Cell*> cells) const {
	float dist = -1;
	for (const auto& c2 : cells) {
		auto d = getDistance(c1, c2);
		if (dist == -1 or d < dist) {
			dist = d;
		}
	}
	return dist;
}

int Map::getCellDistance(const Cell* c1, const Cell* c2) const {
	return abs(c1->x - c2->y) + abs(c1->y - c2->y);
}

int Map::getCellDistance(const Cell* c1, const std::vector<const Cell*> cells) const {
	int dist = -1;
	for (const Cell* c2 : cells) {
		int d = getCellDistance(c1, c2);
		if (dist == -1 || d < dist) {
			dist = d;
		}
	}
	return dist;
}

bool Map::line_of_sight(const Cell* start, const Cell* end) const {

	if (!end->walkable) return false;

	return line_of_sight_ignored(start, end, {});
}

bool Map::line_of_sight_attack(const Cell* start, const Cell* end, const Attack* attack, std::vector<const Cell*> ignored) const {

	if (!end->walkable) return false;

	bool needLos = attack == nullptr ? true : attack->need_los;
	if (!needLos) return true;

	return line_of_sight_ignored(start, end, ignored);
}

bool Map::line_of_sight_ignored(const Cell* start, const Cell* end, std::vector<const Cell*> ignored) const {

	auto check_cell = [&](Cell* cell) {
		if (!cell->walkable) return false;
		if (!cell->available()) {
			if (cell == end) return true;
			return find(ignored.begin(), ignored.end(), cell) != ignored.end();
		}
		return true;
	};
	int x1 = start->x - min_x;
	int y1 = start->y - min_y;
	int a = abs(start->y - end->y);
	int b = abs(start->x - end->x);
	int dx = copysign(1, end->x - start->x);
	int dy = copysign(1, end->y - start->y);
	float h = 0;
	float y = 0;
	if (b > 0) {
		float d = a / b / 2;
		for (int x = 0; x < b; ++x) {
			y = 0.5 + (x * 2 + 1) * d;
			for (int i = 0; i < ceil(y) - h; i++) {
				if (!check_cell(coord[y1 + (h + i) * dy][x1])) return false;
			}
			h = floor(y + 2 * std::numeric_limits<float>::epsilon());
			x1 += dx;
		}
	}
	for (int i = 0; i < a + 1 - h; i++) {
		if (!check_cell(coord[y1 + (h + i) * dy][x1])) return false;
	}
	return true;
}

inline const std::vector<Cell*> Map::get_cells_around(const Cell* const c) const {

	std::vector<Cell*> cells;
	if (c->y > min_y) {
		auto v = coord[c->x - min_x][c->y - min_y - 1];
		if (v != nullptr and v->walkable) {
			cells.push_back(v);
		}
	}
	if (c->x < max_x) {
		auto v = coord[c->x - min_x + 1][c->y - min_y];
		if (v != nullptr and v->walkable) {
			cells.push_back(v);
		}
	}
	if (c->x > min_x) {
		auto v = coord[c->x - min_x - 1][c->y - min_y];
		if (v != nullptr and v->walkable) {
			cells.push_back(v);
		}
	}
	if (c->y < max_y) {
		auto v = coord[c->x - min_x][c->y - min_y + 1];
		if (v != nullptr and v->walkable) {
			cells.push_back(v);
		}
	}
	return cells;
}

std::vector<const Cell*> Map::get_path_between(Cell* start, const Cell* end, std::vector<const Cell*> ignored_cells) const {
	if (start == nullptr || end == nullptr) {
		return {};
	}
	return get_path(start, {end}, ignored_cells);
}

std::vector<const Cell*> Map::get_path(Cell* c1, std::vector<const Cell*> end_cells, std::vector<const Cell*> ignored) const {

	if (c1 == nullptr or end_cells.size() == 0) {
		// cout << "[path] invalid cells" << endl;
		return {};
	}
	if (find(end_cells.begin(), end_cells.end(), c1) != end_cells.end()) {
		// cout << "[path] already arrived" << endl;
		return {};
	}

	for (auto& c : cells) {
		c->visited = false;
		c->closed = false;
		c->cost = std::numeric_limits<unsigned short>::max();
	}

	std::vector<Cell*> open;
	NodeComparator comp;

	c1->cost = 0;
	c1->weight = 0;
	c1->visited = true;
	open.push_back(c1);
	std::push_heap(open.begin(), open.end(), comp);

	while (open.size() > 0) {

		std::pop_heap(open.begin(), open.end(), comp);
		auto u = open.back();
		open.pop_back();
		u->closed = true;

		if (find(end_cells.begin(), end_cells.end(), u) != end_cells.end()) {
			std::vector<const Cell*> result;
			int s = u->cost;
			while (s-- >= 0) {
				result.push_back(u);
				u = u->parent;
			}
			return result;
		}

		for (auto& c : get_cells_around(u)) {

			if (c->closed) continue;
			if (c->entity != nullptr and find(ignored.begin(), ignored.end(), c) == ignored.end()) continue;

			if (!c->visited or u->cost + 1 < c->cost) {
				c->cost = u->cost + 1;
				c->weight = c->cost + getDistance_float(c, end_cells);
				c->parent = u;
				if (!c->visited) {
					open.push_back(c);
					std::push_heap(open.begin(), open.end(), comp);
					c->visited = true;
				}
			}
		}
	}
	// cout << "[path] no path found!" << endl;
	return {};
}

const Cell* Map::int_to_cell(int cell) {
	if (cell < 0 or (size_t) cell >= cells.size()) return nullptr;
	return cells[cell];
}

void Map::print() const {
	std::ostringstream oss;
	oss << "Map: " << std::endl;
	for (int x = 0; x < sx; ++x) {
		for (int y = 0; y < sy; ++y) {
			auto c = coord[y][x];
			if (c == nullptr) {
				oss << "  ";
			} else if (c->walkable) {
				oss << "░░";
			} else {
				oss << "▓▓";
			}
		}
		oss << std::endl;
	}
	LOG << oss.str();
}

void Map::draw_path(const std::vector<const Cell*> path, const Cell* cell1, const Cell* cell2) const {

	std::ostringstream oss;
	oss << "Draw path: [";
	for (const auto& c : path) oss << c->id << " -> ";
	oss << "]" << std::endl;

	for (int x = 0; x < sx; ++x) {
		for (int y = 0; y < sy; ++y) {
			auto c = coord[y][x];
			auto it = find(path.begin(), path.end(), c);
			if (c == nullptr) {
				oss << "  ";
			} else if (c == cell1) {
				oss << "S ";
			} else if (c == cell2) {
				oss << "E ";
			} else if (it != path.end()) {
				oss << (char)('a' + (distance(path.begin(), it) % 26) ) << ' ';
			} else if (c->walkable) {
				oss << "░░";
			} else {
				oss << "▓▓";
			}
		}
		oss << std::endl;
	}
	LOG << oss.str();
}

Json Map::json() const {
	Json obstacles_json;
	for (auto c : obstacles) {
		obstacles_json[std::to_string(c->id)] = std::vector<int>{ c->obstacle, c->obstacle_size };
	}
	return {
		{"type", 1},
		{"obstacles", obstacles_json},
		{"width", width},
		{"height", height}
	};
}

std::ostream& operator << (std::ostream& os, const Map& map) {
	os << "[" << map.width << " × " << map.height << "]";
	return os;
}
