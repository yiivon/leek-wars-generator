#ifndef FIGHT_HPP_
#define FIGHT_HPP_

#include <leekscript.h>
#include <vector>
class Entity;
class Team;
#include "Report.hpp"
#include "Order.hpp"
class Field;
class Chip;
class Cell;
#include "../action/Actions.hpp"
#include "Statistics.hpp"
class FightManager;
#include "TrophyManager.hpp"

class Fight {
public:

	static const int MAX_TURNS = 64;

	std::unique_ptr<Field> field;
	std::vector<Team*> teams;
	std::map<int, Entity*> entities;
	Order order;
	Actions actions;
	Statistics statistics;
	int type;
	int context;
	FightManager* manager;
	TrophyManager trophy_manager;

	Fight();
	virtual ~Fight();

	Report* start(ls::VM& vm, ls::VM& vm_v1);
	Report* crash();

	Entity* getEntity(int id);
	int turn();

	bool is_finished() const;
	void entity_died(Entity* entity, Entity* killer);
	int moveEntity(Entity* entity, const std::vector<const Cell*> path);
	bool generateCritical(Entity* entity) const;
	bool hasCooldown(const Entity* entity, const Chip* chip) const;
	int useWeapon(Entity* launcher, Cell* target);
	int useChip(Entity* caster, Cell* cell, Chip* chip);
	bool mark(std::vector<const Cell*> cells, int color, int duration);

	Json entities_json() const;
};

#endif
