#include "ShopScene.h"
#include "UI/UIManager.h"
#include "UI/GameUI.h"

void ShopScene::Init()
{
	current_state = ShopState::Buy;

	// -----------
	//배경
	auto bg = std::make_unique<AsciiUI>(0, 0);
	bg->LoadAsciiArt("Resource/Shop_BackGround.txt");
	scene_uis.push_back(std::move(bg));


	// 상점 리스트
	int start_x = 3;
	int start_y = 3;
	int width = 50;
	int height = 20;
	auto items = std::make_unique<BorderUI>(start_x, start_y, width, height);

	// 내용
	items->AddContents("Shop");
	scene_uis.push_back(std::move(items));

	SetMenu();
}

void ShopScene::SetMenu()
{
	UIManager::GetInstance().ClearContent(UIType::Menu);

	switch (current_state) {
	case ShopState::Buy:
		UIManager::GetInstance().AddContent(UIType::Menu, "1~9. 아이템 구매   0. 마을로 돌아가기  Tab : 판매/구매 전환");
		UIManager::GetInstance().AddContent(UIType::Menu, "구매할 아이템의 번호를 입력하세요: ");
		break;

	case ShopState::Sell:
		UIManager::GetInstance().AddContent(UIType::Menu, "1~9. 아이템 구매   0. 마을로 돌아가기  Tab : 판매/구매 전환");
		UIManager::GetInstance().AddContent(UIType::Menu, "판매할 아이템의 번호를 입력하세요 : ");
		break;
	}
}

void ShopScene::ProcessEvent(const Event& e)
{
	if (e.type != EventType::KeyDown) {
		return;
	}

	// 탭키 = 9
	if (e.key_code == 9) {	
		int next = (static_cast<int>(current_state) + 1) % 2;
		current_state = static_cast<ShopState>(next);
	}
	else if (e.key_code == '0') {
		PopScene();
		return;
	}
	else {
		switch (current_state) {
		case ShopState::Buy:
			BuyItem(e.key_code);
			break;

		case ShopState::Sell:
			SellItem(e.key_code);
			break;
		}
	}
	SetMenu();
}

void ShopScene::Update(float delta_time)
{
}

void ShopScene::BuyItem(int key_code)
{
	int num = key_code - '0';

	// 1~9 아이템 구매
	if (num >= 1 && num < 9) {
		// 상점 아이템 1~9 구매

	}
	else {
		switch (key_code) {
			// 상점 페이지 넘김 <-
		case 'q':
		case 'Q':

			break;

			// 상점 페이지 넘김 ->
		case 'e':
		case 'E':

			break;

		default:
			UIManager::GetInstance().AddContent(UIType::Menu, "잘못된 입력입니다.");
			break;

		}
	}
}

void ShopScene::SellItem(int key_code)
{
	int num = key_code - '0';

	// 1~9 아이템 판매
	if (num >= 1 && num < 9) {
		// 인벤토리 페이지 * ITEMS_PER_PAGE + num 번째 아이템 판매
	}
	else {
		switch (key_code) {
			// 인벤토리 페이지 넘김 <- 
		case 'q':
		case 'Q':

			break;

			// 인벤토리 페이지 넘김 ->
		case 'e':
		case 'E':

			break;

		default:
			UIManager::GetInstance().AddContent(UIType::Menu, "잘못된 입력입니다.");
			break;

		}
	}
}
