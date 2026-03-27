#pragma once
#include"DropTable.h"
#include<string>
#include<vector>
#include<random>

using namespace std;


class Monster {
public:
	Monster(int level) : level(level), health(1), attack(1), maxhealth(1),Isvisible(true){}
    virtual ~Monster(){}
    virtual void TakeDamage(int damage) = 0;
    virtual vector<ItemID> getDropItems() {
        return droptable.drop();
    }

    

    bool IsDead() const { return health <= 0; };

    //getter 함수

    string GetName() const {
        return name;
    }

    int GetHealth() const {
        return health;
    }

    int GetAttack() const {
        return attack;
    }

    int GetLevel() const {
        return level;
    }

    int GetMaxHealth() const {
        return maxhealth;
    }

    // setter 함수

    void SetVisible(bool visible) {
        this->Isvisible = visible;
    };

    void SetName(const string &name) {
        this->name = name;
    }

    void SetHealth(int health) {
        this->health = health;
    }

    void SetAttack(int attack) {
        this->attack = attack;
    }

    void SetLevel(int level) {
        this->level = level;
    }



protected:
    string name;
    int health;
    int attack;
    int level;
    DropTable droptable;
    bool Isvisible;
    int maxhealth;
};

