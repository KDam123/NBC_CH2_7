#pragma once
#include "Item.h"

#include <string>

class Character;

class HealthPotion : public IItem 
{
private:
	std::string name;
	ItemID id;
	int health_restore;
	std::string description;

public:
	HealthPotion();

	std::string GetName() const override;
	ItemID GetID() const override;
	std::string GetDesc() const override;

	void Use(Character& target) override;
};