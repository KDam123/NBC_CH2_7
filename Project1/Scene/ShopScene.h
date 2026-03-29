#pragma once
#include "BaseScene.h"
#include "Items/Item.h"

enum class ShopState { Buy, Sell };

class ShopScene : public BaseScene
{
public:
	ShopScene() = default;

	void Init() override;
	void SetMenu() override;
	void ProcessEvent(const Event& e) override;
	void Update(float delta_time) override;

private:
	ShopState current_state = ShopState::Buy;
	std::vector<ItemID> shop_items;

	void BuyItem(int key_code);
	void SellItem(int key_code);
};

