// Copyright (C) 2022 Chiahong Hong. <https://github.com/ChiahongHong/>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "wx/wx.h"

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();
	void generateMines(int &x, int &y);
	void onButtonClicked(wxCommandEvent &event);
	void onButtonRightClicked(wxMouseEvent &event);
	void expand(int x, int y);
	void showMines();
	void resetGame();
	bool isWin();

private:
	bool *mines = nullptr;
	bool firstClick = true;
	const int nums = 15;
	const int width = 10;
	const int height = 10;
	wxButton **btn;
};
