#include "EntityModule.hpp"
#include "../fight/Fight.hpp"
#include "WeaponModule.hpp"
#include "ChipModule.hpp"
#include "../fight/Simulator.hpp"
#include "../item/Weapon.hpp"
#include "../entity/Entity.hpp"
#include "../entity/Team.hpp"
#include "../item/Chip.hpp"

const ls::LSClass* EntityModule::entity_clazz;
const EntityType* EntityModule::raw_type(new EntityType());
const ls::Type EntityModule::type(raw_type, ls::Nature::POINTER, true);
const ls::Type EntityModule::const_type(raw_type, ls::Nature::POINTER, true, false, true);
const ls::Type EntityModule::array_type(ls::RawType::ARRAY, ls::Nature::POINTER, EntityModule::type);

EntityModule::EntityModule() : Module("Entity") {

	EntityModule::entity_clazz = this->clazz;

	field("absoluteShield", ls::Type::INTEGER, (void*) &Entity::getAbsoluteShield);
	field("agility", ls::Type::INTEGER, (void*) &Entity::getAgility);
	field("cell", CellModule::type, (void*) &Entity::getCell);
	field("chips", ChipModule::array_type, (void*) &Entity::getChips);
	field("damageReturn", ls::Type::INTEGER, (void*) &Entity::getDamageReturn);
	field("frequency", ls::Type::INTEGER, (void*) &Entity::getFrequency);
	field("level", ls::Type::INTEGER, (void*) &Entity::getLevel);
	field("life", ls::Type::INTEGER, (void*) &Entity::getLife);
	field("name", ls::Type::STRING, (void*) &Entity::getName);
	field("magic", ls::Type::INTEGER, (void*) &Entity::getMagic);
	field("mp", ls::Type::INTEGER, (void*) &Entity::getMP);
	field("relativeShield", ls::Type::INTEGER, (void*) &Entity::getRelativeShield);
	field("resistance", ls::Type::INTEGER, (void*) &Entity::getResistance);
	field("science", ls::Type::INTEGER, (void*) &Entity::getScience);
	field("strength", ls::Type::INTEGER, (void*) &Entity::getStrength);
	field("talent", ls::Type::INTEGER, (void*) &Entity::getTalent);
	field("totalMP", ls::Type::INTEGER, (void*) &Entity::getTotalMP);
	field("totalTP", ls::Type::INTEGER, (void*) &Entity::getTotalTP);
	field("tp", ls::Type::INTEGER, (void*) &Entity::getTP);
	field("weapon", WeaponModule::type, (void*) &Entity::getWeapon);
	field("weapons", WeaponModule::array_type, (void*) &Entity::getWeapons);
	field("wisdom", ls::Type::INTEGER, (void*) &Entity::getWisdom);

	method("getAliveAllies", {{EntityModule::array_type, {EntityModule::const_type}, (void*) &Entity::get_alive_allies, ls::Method::NATIVE}});
	method("getAliveEnemies", {{EntityModule::array_type, {EntityModule::const_type}, (void*) &Entity::get_alive_enemies, ls::Method::NATIVE}});
	method("getAllies", {{EntityModule::array_type, {EntityModule::const_type}, (void*) &Entity::get_allies, ls::Method::NATIVE}});
	method("getClosestAlly", {{EntityModule::type, {EntityModule::const_type}, (void*) &Entity::get_closest_ally, ls::Method::NATIVE}});
	method("getClosestEnemy", {{EntityModule::type, {EntityModule::const_type}, (void*) &Entity::get_closest_enemy, ls::Method::NATIVE}});
	method("getDeadAllies", {{EntityModule::array_type, {EntityModule::const_type}, (void*) &Entity::get_alive_allies, ls::Method::NATIVE}});
	method("getDeadEnemies", {{EntityModule::array_type, {EntityModule::const_type}, (void*) &Entity::get_alive_enemies, ls::Method::NATIVE}});
	method("getEnemies", {{EntityModule::array_type, {EntityModule::const_type}, (void*) &Entity::get_enemies, ls::Method::NATIVE}});
	method("getFarthestAlly", {{EntityModule::type, {EntityModule::const_type}, (void*) &Entity::get_farthest_ally, ls::Method::NATIVE}});
	method("getFarthestEnemy", {{EntityModule::type, {EntityModule::const_type}, (void*) &Entity::get_farthest_enemy, ls::Method::NATIVE}});
	method("isAlly", {{ls::Type::BOOLEAN, {EntityModule::const_type}, (void*) &Entity::isAlly, ls::Method::NATIVE}});
	method("isEnemy", {{ls::Type::BOOLEAN, {EntityModule::const_type}, (void*) &Entity::isEnemy, ls::Method::NATIVE}});
	method("moveToward", {
		{ls::Type::INTEGER, {EntityModule::const_type, EntityModule::const_type, ls::Type::CONST_INTEGER}, (void*) &Entity::moveTowardMP, ls::Method::NATIVE},
		{ls::Type::INTEGER, {EntityModule::const_type, EntityModule::const_type}, (void*) &Entity::moveToward, ls::Method::NATIVE},
		{ls::Type::INTEGER, {EntityModule::const_type, CellModule::const_type, ls::Type::CONST_INTEGER}, (void*) &Entity::moveTowardCellMP, ls::Method::NATIVE},
		{ls::Type::INTEGER, {EntityModule::const_type, CellModule::const_type}, (void*) &Entity::moveTowardCell, ls::Method::NATIVE},
	});
	method("useChip", {{ls::Type::INTEGER, {EntityModule::const_type, ChipModule::const_type, EntityModule::const_type}, (void*) &Entity::useChip, ls::Method::NATIVE}});
	method("useWeapon", {{ls::Type::INTEGER, {EntityModule::const_type, EntityModule::const_type}, (void*) &Entity::useWeapon, ls::Method::NATIVE}});
	method("say", {{ls::Type::BOOLEAN, {EntityModule::const_type, ls::Type::CONST_POINTER}, (void*) &Entity::say, ls::Method::NATIVE}});
	method("setWeapon", {
		{ls::Type::NULLL, {EntityModule::const_type, WeaponModule::const_type}, (void*) &entity_setWeapon, ls::Method::NATIVE},
		{ls::Type::NULLL, {EntityModule::const_type, ls::Type::CONST_INTEGER}, (void*) &Entity::setWeaponInteger, ls::Method::NATIVE}
	});

	/*
	 * V1 functions
	 */
	method("_getLeek", ls::Method::Static, {{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getLeek, ls::Method::NATIVE}});

	method("_getAbsoluteShield", ls::Method::Static, {
		{ls::Type::POINTER, {}, (void*) &entity__getAbsoluteShield, ls::Method::NATIVE},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getAbsoluteShieldEntity, ls::Method::NATIVE}
	});
	method("_getAgility", ls::Method::Static, {
		{ls::Type::POINTER, {}, (void*) &entity__getAgility, ls::Method::NATIVE},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getAgilityEntity, ls::Method::NATIVE}
	});
	method("_getBirthTurn", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getBirthTurn},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getBirthTurnEntity}
	});
	method("_getCell", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getCell},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getCellEntity}
	});
	method("_getChips", ls::Method::Static, {
		{ls::Type::INT_ARRAY, {}, (void*) &entity__getChips},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getChipsEntity},
	});
	method("_getCores", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getCores},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getCoresEntity},
	});
	method("_getDamageReturn", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getDamageReturn},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getDamageReturnEntity}
	});
	method("_getEffects", ls::Method::Static, {
		{ls::Type::PTR_ARRAY, {}, (void*) &entity__getEffects},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getEffectsEntity}
	});
	method("_getEntityTurnOrder", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getEntityTurnOrder},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getEntityTurnOrderEntity}
	});
	method("_getFrequency", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getFrequency},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getFrequencyEntity}
	});
	method("_getLaunchedEffects", ls::Method::Static, {
		{ls::Type::PTR_ARRAY, {}, (void*) &entity__getLaunchedEffects},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getLaunchedEffectsEntity}
	});
	method("_getLeekID", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getLeekID},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getLeekIDEntity}
	});
	method("_getLevel", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getLevel},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getLevelEntity}
	});
	method("_getLife", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getLife},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getLifeEntity}
	});
	method("_getMagic", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getMagic},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getMagicEntity}
	});
	method("_getMP", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getMP},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getMPEntity}
	});
	method("_getName", ls::Method::Static, {
		{ls::Type::STRING, {}, (void*) &entity__getName},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getNameEntity}
	});
	method("_getNearestEnemy", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getNearestEnemy}
	});
	method("_getNearestAlly", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getNearestAlly}
	});
	method("_getRelativeShield", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getRelativeShield},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getRelativeShieldEntity}
	});
	method("_getResistance", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getResistance},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getResistanceEntity}
	});
	method("_getScience", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getScience},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getScienceEntity}
	});
	method("_getStrength", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getStrength},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getStrengthEntity}
	});
	method("_getSummoner", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getSummoner},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getSummonerEntity}
	});
	method("_getTeamID", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getTeamID},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getTeamIDEntity}
	});
	method("_getTeamName", ls::Method::Static, {
		{ls::Type::STRING, {}, (void*) &entity__getTeamName},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getTeamNameEntity}
	});
	method("_getTotalLife", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getTotalLife},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getTotalLifeEntity}
	});
	method("_getTotalMP", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getTotalMP},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getTotalMPEntity}
	});
	method("_getTotalTP", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getTotalTP},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getTotalTPEntity}
	});
	method("_getTP", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getTP},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getTPEntity}
	});
	method("_getType", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getType},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getTypeEntity}
	});
	method("_getWeapon", ls::Method::Static, {
		{ls::Type::POINTER, {}, (void*) &entity__getWeapon},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getWeaponEntity}
	});
	method("_getWeapons", ls::Method::Static, {
		{ls::Type::INT_ARRAY, {}, (void*) &entity__getWeapons},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getWeaponsEntity}
	});
	method("_getWisdom", ls::Method::Static, {
		{ls::Type::INTEGER, {}, (void*) &entity__getWisdom},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getWisdomEntity}
	});
	method("_isAlive", ls::Method::Static, {{ls::Type::BOOLEAN, {ls::Type::POINTER}, (void*) &entity__isAlive}});
	method("_isAlly", ls::Method::Static, {{ls::Type::BOOLEAN, {ls::Type::POINTER}, (void*) &entity__isAlly}});
	method("_isDead", ls::Method::Static, {{ls::Type::BOOLEAN, {ls::Type::POINTER}, (void*) &entity__isDead}});
	method("_isEnemy", ls::Method::Static, {{ls::Type::BOOLEAN, {ls::Type::POINTER}, (void*) &entity__isEnemy}});
	method("_isSummon", ls::Method::Static, {
		{ls::Type::BOOLEAN, {}, (void*) &entity__isSummon},
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__isSummonEntity},
	});

	method("_getCooldown", ls::Method::Static, {
		{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__getCooldown},
		{ls::Type::POINTER, {ls::Type::POINTER, ls::Type::POINTER}, (void*) &entity__getCooldownEntity},
	});

	method("_listen", ls::Method::Static, {{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__listen}});
	method("_moveToward", ls::Method::Static, {{ls::Type::INTEGER, {ls::Type::POINTER}, (void*) &entity__moveToward}});
	method("_say", ls::Method::Static, {{ls::Type::POINTER, {ls::Type::POINTER}, (void*) &entity__say}});
	method("_setWeapon", ls::Method::Static, {{ls::Type::BOOLEAN, {ls::Type::NUMBER_P}, (void*) &entity__setWeapon}});
	method("_useWeapon", ls::Method::Static, {{ls::Type::INTEGER, {ls::Type::NUMBER_P}, (void*) &entity__useWeapon}});
}

EntityModule::~EntityModule() {}

const Weapon* entity_getWeapon(Entity* entity) {
	return (Weapon*) entity->getWeapon();
}

const ls::LSArray<Weapon*>* entity_getWeapons(Entity* entity) {
	return (const ls::LSArray<Weapon*>*) entity->getField("weapons");
}

const ls::LSNull* entity_setWeapon(Entity* entity, const Weapon* weapon) {
	entity->setWeapon(weapon);
	return (ls::LSNull*) ls::LSNull::get();
}

/*
 * LS v1 support functions (deprecated in v2)
 */

/*
 * Internal function to get a entity from the function argument
 * Null -> my entity
 * Number -> an entity id, try to get it
 */
inline Entity* entity__getEntity(const ls::LSValue* entity) {
	if (dynamic_cast<const ls::LSNull*>(entity)) {
		return Simulator::entity;
	}
	if (const ls::LSNumber* n = dynamic_cast<const ls::LSNumber*>(entity)) {
		return Simulator::fight->getEntity(n->value);
	}
	return nullptr;
}

/*
 * Internal function to get an entity characteristic
 */
inline ls::LSValue* entity__getCharacteristic(const ls::LSValue* entity, Characteristic charac) {

	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();

	return ls::LSNumber::get(e->getCharacteristic(charac));
}

/*
 * Characteristics methods, all the same
 * Null if invalid entity, int otherwise
 */
int entity__getAbsoluteShield() {
	return Simulator::entity->getAbsoluteShield();
}
ls::LSValue* entity__getAbsoluteShieldEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::ABSOLUTE_SHIELD);
}
int entity__getAgility() {
	return Simulator::entity->getAgility();
}
ls::LSValue* entity__getAgilityEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::AGILITY);
}
int entity__getFrequency() {
	return Simulator::entity->getFrequency();
}
ls::LSValue* entity__getFrequencyEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::FREQUENCY);
}
int entity__getCores() {
	return 0; // was already deprecated in v1
}
ls::LSValue* entity__getCoresEntity(const ls::LSValue*) {
	return ls::LSNumber::get(0); // was already deprecated in v1
}
int entity__getDamageReturn() {
	return Simulator::entity->getDamageReturn();
}
ls::LSValue* entity__getDamageReturnEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::DAMAGE_RETURN);
}
int entity__getMagic() {
	return Simulator::entity->getMagic();
}
ls::LSValue* entity__getMagicEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::MAGIC);
}
int entity__getRelativeShield() {
	return Simulator::entity->getRelativeShield();
}
ls::LSValue* entity__getRelativeShieldEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::RELATIVE_SHIELD);
}
int entity__getResistance() {
	return Simulator::entity->getResistance();
}
ls::LSValue* entity__getResistanceEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::RESISTANCE);
}
int entity__getScience() {
	return Simulator::entity->getScience();
}
ls::LSValue* entity__getScienceEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::SCIENCE);
}
int entity__getStrength() {
	return Simulator::entity->getStrength();
}
ls::LSValue* entity__getStrengthEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::STRENGTH);
}
int entity__getTotalLife() {
	return Simulator::entity->getTotalLife();
}
ls::LSValue* entity__getTotalLifeEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::LIFE);
}
int entity__getTotalMP() {
	return Simulator::entity->getTotalMP();
}
ls::LSValue* entity__getTotalMPEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::MP);
}
int entity__getTotalTP() {
	return Simulator::entity->getTotalTP();
}
ls::LSValue* entity__getTotalTPEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::TP);
}
int entity__getWisdom() {
	return Simulator::entity->getWisdom();
}
ls::LSValue* entity__getWisdomEntity(const ls::LSValue* entity) {
	return entity__getCharacteristic(entity, Characteristic::WISDOM);
}

int entity__getLife() {
	return Simulator::entity->getLife();
}
ls::LSValue* entity__getLifeEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNumber::get(e->life);
}

int entity__getTP() {
	return Simulator::entity->getTP();
}
ls::LSValue* entity__getTPEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNumber::get(e->getTP());
}

int entity__getMP() {
	return Simulator::entity->getMP();
}
ls::LSValue* entity__getMPEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNumber::get(e->getMP());
}

int entity__getBirthTurn() {
	return 0;
}
ls::LSValue* entity__getBirthTurnEntity(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNumber::get(0);
}

int entity__getCell() {
	return Simulator::entity->cell->id;
}
ls::LSValue* entity__getCellEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNumber::get(e->cell->id);
}

ls::LSArray<int>* entity__getChips() {
	// TODO Int array
//	return Simulator::entity->chips.clone();
}
ls::LSValue* entity__getChipsEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return e->chips.clone();
}

ls::LSArray<ls::LSValue*>* entity__getEffects() {
	// TODO
//	return LSNull::null_var;
}
ls::LSValue* entity__getEffectsEntity(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNull::get();
}

int entity__getEntityTurnOrder() {
	// TODO
}
ls::LSValue* entity__getEntityTurnOrderEntity(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNull::get();
}

ls::LSArray<ls::LSValue*>* entity__getLaunchedEffects() {
	// TODO
//	return LSNull::null_var;
}
ls::LSValue* entity__getLaunchedEffectsEntity(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNull::get();
}

int entity__getLeekID() {
	return Simulator::entity->id;
}
ls::LSValue* entity__getLeekIDEntity(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNull::get();
}

int entity__getLevel() {
	return Simulator::entity->level;
}
ls::LSValue* entity__getLevelEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNumber::get(e->level);
}

ls::LSString* entity__getName() {
	return new ls::LSString(Simulator::entity->name);
}
ls::LSValue* entity__getNameEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return new ls::LSString(e->name);
}

int entity__getNearestEnemy() {
	return Simulator::entity->get_closest_enemy()->id;
}
int entity__getNearestAlly() {
	return Simulator::entity->get_closest_ally()->id;
}

ls::LSValue* entity__getSummoner() {
	// TODO
}
ls::LSValue* entity__getSummonerEntity(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNull::get();
}

ls::LSValue* entity__getTeamID() {
	// TODO
//	return LSNumber::get(e->team_id);
}
ls::LSValue* entity__getTeamIDEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNumber::get(e->team_id);
}

/*
 * Null or string
 */
ls::LSValue* entity__getTeamName() {
//	return new LSString(e->team_name);
}
ls::LSValue* entity__getTeamNameEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return new ls::LSString(e->team_name);
}

ls::LSValue* entity__getType() {
	// TODO
}
ls::LSValue* entity__getTypeEntity(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNull::get();
}

ls::LSValue* entity__getWeapon() {
	// TODO
}
ls::LSValue* entity__getWeaponEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return e->weapon->clone();
}

ls::LSArray<int>* entity__getWeapons() {
	// TODO
}
ls::LSValue* entity__getWeaponsEntity(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return e->weapons.clone();
}

bool entity__isAlive(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return e->isAlive();
}

bool entity__isDead(const ls::LSValue* entity) {
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return e->isDead();
}

bool entity__isAlly(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNull::get();
}

bool entity__isEnemy(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return ls::LSNull::get();
	return ls::LSNull::get();
}

bool entity__isSummon() {
	// TODO
	return false;
}
bool entity__isSummonEntity(const ls::LSValue* entity) {
	// TODO
	Entity* e = entity__getEntity(entity);
	if (e == nullptr) return false;
	return false;
}

int entity__getLeek() {
	return Simulator::entity->id;
}

ls::LSArray<ls::LSValue*>* entity__listen() {
	// TODO
	return new ls::LSArray<ls::LSValue*>();
}

int entity__moveToward(const ls::LSValue* entity) {
	auto e = entity__getEntity(entity);
	if (e == nullptr) {
		return 0;
	}
	return Simulator::entity->moveToward(e);
}

bool entity__say(const ls::LSValue* message) {
	return Simulator::entity->say(message);
}

bool entity__setWeapon(const ls::LSNumber* weapon) {
	if (weapon == nullptr) return false;
	return Simulator::entity->setWeaponInteger(weapon->value);
}

/*
 * Null or int
 */
ls::LSValue* entity__getCooldown(const ls::LSValue* chip) {
	// TODO
}
ls::LSValue* entity__getCooldownEntity(const ls::LSValue* chip, const ls::LSValue* entity) {
	// TODO
}

ls::LSValue* entity__resurrect() {
	// TODO
}
ls::LSValue* entity__summon() {
	// TODO
}
ls::LSValue* entity__useChip() {
	// TODO
}
ls::LSValue* entity__useChipOnCell() {
	// TODO
}

int entity__useWeapon(ls::LSValue* target) {
	auto e = entity__getEntity(target);
	return Simulator::entity->useWeapon(e);
}
