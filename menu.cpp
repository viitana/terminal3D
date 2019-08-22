#include <array>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <ncurses.h>

struct settings
{
	std::array<std::string, 6> settingLabels;
	std::array<bool, 6> settingValues;
};

struct menu
{
	int cursor;
	settings setting;
};

void drawMenu(menu m, int x, int y) {
	std::ostringstream ss;
	int spacing = 4;
	int w = 0;
	
	for (const auto& label : m.setting.settingLabels) {
		w = std::max(w, (int)label.length());
	}

	ss << "MENU - " << m.cursor << "\n";
	mvprintw(y, x, ss.str().c_str());
	ss.clear();

	for (int n = 0; n < (int)m.setting.settingLabels.size(); n++) {
		std::string s = (m.cursor == n) ? "> " : "  ";
		std::string e;
		e.append(w - (int)m.setting.settingLabels[n].length() + spacing, '.');
		e.append(m.setting.settingValues[n] ? "ON" : "OFF");
		mvprintw(y + n + 1, x, (s + m.setting.settingLabels[n] + e).c_str());
	}
}

