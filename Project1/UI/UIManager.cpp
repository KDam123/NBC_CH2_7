#include "UIManager.h"
#include "UI/GameUI.h"
#include "Core/RenderSystem.h"

UIManager::UIManager()
{
    // 콘솔 사이즈 구하기
    int w = RenderSystem::GetInstance().GetScreenWidth();
    int h = RenderSystem::GetInstance().GetScreenHeight();

    // 메뉴 좌하단 시작 길이 0.7, 높이 0.2
    int menu_width = static_cast<int>(w * 0.7f);
    int menu_height = static_cast<int>(h * 0.2f);
    int menu_x = 0;
    int menu_y = h - menu_height;
    uis.push_back(std::make_unique<BorderUI>(menu_x, menu_y, menu_width, menu_height));

    // 우측 영역 변수
    int right_area_width = w - menu_width - 1;
    int right_area_x = menu_width;


    // 인포
    int info_width = right_area_width / 2;
    int info_height = static_cast<int>(h * 0.2f);
    int info_y = 0;
    uis.push_back(std::make_unique<CharacterInfoUI>(right_area_x, info_y, right_area_width, info_height));
    

    // 로그  높이 0.45
    int log_height = static_cast<int>(h * 0.45f);
    int log_y = h - log_height;
    uis.push_back(std::make_unique<LogUI>(right_area_x, log_y, right_area_width, log_height));


    // 아이템
    int item_height = h - info_height - log_height;
    uis.push_back(std::make_unique<ItemUI>(right_area_x, info_height, right_area_width, item_height));

    // 킬보드 UI 추가
    int kill_width = static_cast<int>(w * 0.15f);
    int kill_height = static_cast<int>(h * 0.5f);
    int kill_x = menu_width - kill_width;
    int kill_y = 0;
    uis.push_back(std::make_unique<KillBoardUI>(kill_x, kill_y, kill_width, kill_height));
}

UIManager::~UIManager()
{
}

void UIManager::AddMessage(UIType type, std::string_view msg)
{
    size_t idx = static_cast<size_t>(type);

    if (idx < static_cast<size_t>(UIType::COUNT)) {
        uis[idx]->AddContents(msg);
    }
}

void UIManager::ClearMessage(UIType type)
{
    size_t idx = static_cast<size_t>(type);

    if (idx < static_cast<size_t>(UIType::COUNT)) {
        uis[idx]->Clear();
    }
}

void UIManager::ClearAll(const std::vector<UIType>& ignores)
{
    for (size_t i = 0; i < static_cast<size_t>(UIType::COUNT); ++i) {
        UIType current_type = static_cast<UIType>(i);

        auto it = std::find(ignores.begin(), ignores.end(), current_type);
        if (it != ignores.end()) {
            continue;
        }

        uis[i]->Clear();
    }
}

void UIManager::Render()
{
    // 로그ui, 메세지ui 등 전역 ui 그리기
    for (const auto& ui : uis) {
        if (ui->IsVisible()) {
            ui->Render();
        }
    }
}

void UIManager::SetVisible(UIType type, bool value)
{
    size_t idx = static_cast<size_t>(type);
    if (idx < static_cast<size_t>(UIType::COUNT)) {
        uis[idx]->SetVisible(value);
    }
}

void UIManager::SetAllVisible(bool value)
{
    for (auto& ui : uis) {
        ui->SetVisible(value);
    }
}

void UIManager::NextPageItemUI()
{
    auto* item_ui = static_cast<ItemUI*>(uis[static_cast<int>(UIType::Item)].get());
    item_ui->NextPage();
}

void UIManager::PrevPageItemUI()
{
    auto* item_ui = static_cast<ItemUI*>(uis[static_cast<int>(UIType::Item)].get());
    item_ui->PrevPage();
}

void UIManager::OnMonsterKilled(const std::string& monster_name)    // 킬보드에 킬 추가
{
    static_cast<KillBoardUI*>(uis[static_cast<int>(UIType::KillLog)].get())->AddKill(monster_name);
}

void UIManager::SaveLogToFile(const std::string& filename)  //로그 저장
{
    std::ofstream file(filename);

    // 킬보드 저장
    file << "\n=== 킬 보드 ===\n";
    auto* kill_ui = static_cast<KillBoardUI*>(uis[static_cast<int>(UIType::KillLog)].get());
    for (const auto& k : kill_ui->GetKillCount())
    {
        file << k.first << " x" << k.second << "\n";
    }

    // 전투 로그 저장
    file << "=== 전투 로그 ===\n";
    auto* log_ui = uis[static_cast<int>(UIType::Log)].get();
    for (const auto& line : log_ui->GetAllContents())
    {
        file << line << "\n";
    }

    file.close();
}