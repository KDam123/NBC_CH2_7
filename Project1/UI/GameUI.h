#pragma once
#include "BaseUI.h"

class Character;
class Monster;

// 테두리 있는 UI
class BorderUI : public BaseUI
{
public:
	BorderUI(int x, int y, int w, int h);
	virtual ~BorderUI() = default;

	virtual void Render() override;

private:
	std::string top_border;
	std::string bottom_border;
};


class LogUI : public BorderUI
{
public:
	LogUI(int x, int y, int w, int h);
	~LogUI() = default;

	void AddContents(std::string_view msg) override;
};


class CharacterInfoUI : public BorderUI
{
public:
	CharacterInfoUI(int x, int y, int w, int h);
	~CharacterInfoUI() = default;

	void Render() override;
};


class ItemUI : public BorderUI
{
public:
	ItemUI(int x, int y, int w, int h);
	~ItemUI() = default;

	void Render() override;
	void NextPage();
	void PrevPage();

private:
	int current_page = 0;
	const int ITEMS_PER_PAGE = 10;
};


// 아스키아트만 출력하는 UI
class AsciiUI : public BaseUI
{
public:
	AsciiUI(int x, int y);
	~AsciiUI() = default;

	virtual void Render() override;
};


// 아스키아트 + info 출력하는 UI
// Character와 Monster의 상위 클래스가 있다면 아래 두 클래스 합치기 가능
class CharacterUI : public AsciiUI
{
public:
	CharacterUI(int x, int y);
	~CharacterUI() = default;

	void Render() override;
	void SetTarget(const Character* target);

private:
	const Character* target = nullptr;
};


class MonsterUI : public AsciiUI
{
public:
	MonsterUI(int x, int y);
	~MonsterUI() = default;

	void Render() override;
	void SetTarget(const Monster* target);

private:
	const Monster* target = nullptr;
};

