#include "GameManager.h"
#include "Characters/Character.h"
#include "GamePlay/Battle/BattleManager.h"
#include "Scene/BaseScene.h"
#include "Scene/ExitPopUpScene.h"
#include "UI/UIManager.h"
#include "Core/RenderSystem.h"
#include "Scene/SceneFactory.h"
#include "Items/ItemFactory.h"
#include <thread>
#include <conio.h>

GameManager::GameManager() : scene_op(SceneOp::None), next_scene(SceneType::None),
	is_running(true)
{
}

GameManager::~GameManager() = default;

void GameManager::Init()
{
	UIManager::GetInstance().SetAllVisible(true);

	ItemFactory::Initialize();
	player = &Character::GetInstance();
	battle_manager = std::make_unique<BattleManager>(player);
	
	// 초기 씬 = 타이틀
	scene_stack.push_back(SceneFactory::CreateScene(SceneType::Title));
	scene_stack.back()->Init();
}

// 게임 루프 
void GameManager::Run()
{
	// 60fps -> 1프레임당 약16.66ms
	constexpr float delta_time = 1.f / 60.f;

	auto last = std::chrono::steady_clock::now();
	float accumulator = 0.f;	// 경과한 시간 누적

	while (is_running) {
		auto current = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsed = current - last;
		last = current;

		// 이번 루프에 걸린 시간
		accumulator += elapsed.count();

		// KeyDown 이벤트 감지
		ProcessInput();

		// 이벤트 처리
		while (!event_queue.empty()) {
			Event ev = event_queue.front();
			event_queue.pop();

			switch (ev.type) {
			case EventType::KeyDown:
				if (!scene_stack.empty()) {
					scene_stack.back()->ProcessEvent(ev);
				}
				break;

			case EventType::KeyUp:
				if (!scene_stack.empty()) {
					scene_stack.back()->ProcessEvent(ev);
				}
				break;

			case EventType::ChangeScene:
				scene_op = SceneOp::Change;
				next_scene = ev.next_scene;
				break;

			case EventType::PushScene:
				scene_op = SceneOp::Push;
				next_scene = ev.next_scene;
				break;

			case EventType::PopScene:
				scene_op = SceneOp::Pop;
				break;

			case EventType::Quit:
				is_running = false;
				break;
			}

			// 씬 전환 예약되었으면 이후에 다른 event가 있어도 무시
			if (scene_op != SceneOp::None) {
				break;
			}
		}


		// 16.66ms가 지났다면 update, render
		if (accumulator >= delta_time) {

			// 만약 렉으로 처리가 늦어 시간이 밀렸다면 반복해서 처리
			while (accumulator >= delta_time) {
				if (!scene_stack.empty()) {
					scene_stack.back()->Update(delta_time);
				}
				accumulator -= delta_time;
			}

			// 씬 Change, Push, Pop 처리
			if (scene_op != SceneOp::None) {
				ProcessScene();
			}

			// 로직 업데이트가 끝났다면 최신 상태 그리기
			// 화면 비우기 -> Scene Render -> 전역 UI Render -> 화면 출력 순
			RenderSystem::GetInstance().ClearBuffer();

			// 최상단 스택부터 검사해서 불투명한 씬을 찾고 찾았다면 break
			int idx = 0;
			for (int i = static_cast<int>(scene_stack.size()) - 1; i >= 0; --i) {
				if (scene_stack[i]->IsOpaque()) {
					idx = i;
					break;
				}
			}

			UIManager::GetInstance().Render();

			// 불투명한 씬부터 렌더링, 이 아래는 어차피 안보이므로 그릴 필요 X
			for (size_t i = static_cast<size_t>(idx); i < scene_stack.size(); ++i) {
				scene_stack[i]->Render();
			}

			RenderSystem::GetInstance().Draw();
		}

		// 짧게 휴식
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void GameManager::Release()
{
	while (!scene_stack.empty()) {
		scene_stack.back()->Release();
		scene_stack.pop_back();
	}
}

void GameManager::PushEvent(const Event& ev)
{
	event_queue.push(ev);
}

Character* GameManager::GetPlayer() const
{
	return player;
}

BattleManager* GameManager::GetBattleManager() const
{
	return battle_manager.get();
}


// private 함수
void GameManager::ProcessInput()
{
	if (_kbhit()) {
		int key = _getch();

		// 27 = ESC 키, 누를 경우 게임종료 팝업 뜨도록
		if (key == 27) {
			// 이미 종료팝업 떠있다면 넘기기
			if (!scene_stack.empty() && dynamic_cast<ExitPopUpScene*>(scene_stack.back().get())) {
			}
			else {	// 종료팝업 없으면 팝업씬 push, 함수 빠져나가기
				Event ev{};
				ev.type = EventType::PushScene;
				ev.next_scene = SceneType::Exit;
				event_queue.push(ev);
				return;
			}
		}

		Event ev{};
		ev.type = EventType::KeyDown;
		ev.key_code = key;
		event_queue.push(ev);
	}
}

void GameManager::ProcessScene()
{
	UIManager::GetInstance().SetAllVisible(true);

	switch (scene_op) {
	case SceneOp::Change:
		// 기존 씬 전부 제거
		Release();
		[[fallthrough]];

	case SceneOp::Push:
		scene_stack.push_back(SceneFactory::CreateScene(next_scene));
		scene_stack.back()->Init();
		break;

	case SceneOp::Pop:
		if (!scene_stack.empty()) {
			scene_stack.back()->Release();
			scene_stack.pop_back();

			// 씬이 없다면 종료
			if (!scene_stack.empty()) {
				scene_stack.back()->SetMenu();
			}
			else {
				is_running = false;
			}
		}
		break;
	}


	// 처리가 끝났으면 다음씬 None, 씬 동작 None
	next_scene = SceneType::None;
	scene_op = SceneOp::None;

	// 혹시 남아있다면 기존 씬의 이벤트 전부 날리기
	while (!event_queue.empty()) {
		event_queue.pop();
	}
}
