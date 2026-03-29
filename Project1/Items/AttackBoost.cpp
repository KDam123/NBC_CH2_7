#include "AttackBoost.h"

#include <iostream>

#include "Character.h"


AttackBoost::AttackBoost() : name("Attack Boost"), id(ItemID::AttackBoost), attack_increase(10) 
{
	description = "다음 전투에서 공격력이 10 증가합니다.";
}

std::string AttackBoost::GetName() const 
{
	return name;
}

ItemID AttackBoost::GetID() const 
{
	return id;
}

std::string AttackBoost::GetDesc() const
{
	return description;
}

void AttackBoost::Use(Character& target) 
{
	target.IncreaseAttack(attack_increase);
}