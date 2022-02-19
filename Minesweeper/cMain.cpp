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

#include "cMain.h"

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Minesweeper", wxDefaultPosition, wxSize(800, 600))
{
	btn = new wxButton * [width * height];
	mines = new bool [width * height];
	wxGridSizer *grid = new wxGridSizer(width, height, 0, 0);

	wxFont font(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			btn[y * width + x] = new wxButton(this, 10000 + (y * width + x));
			btn[y * width + x]->SetFont(font);
			grid->Add(btn[y * width + x], 1, wxEXPAND | wxALL);

			btn[y * width + x]->Bind(wxEVT_BUTTON, &cMain::onButtonClicked, this);
			btn[y * width + x]->Bind(wxEVT_RIGHT_DOWN, &cMain::onButtonRightClicked, this);
			btn[y * width + x]->Bind(wxEVT_RIGHT_DCLICK, &cMain::onButtonRightClicked, this);
			mines[y * width + x] = false;
		}
	}
	this->Center();
	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain()
{
	delete [] btn;
}

void cMain::onButtonClicked(wxCommandEvent& event)
{
	int x = (event.GetId() - 10000) % width;
	int y = (event.GetId() - 10000) / width;

	if (btn[y * width + x]->GetLabel() != wxT("\u25B2")) {
		if (firstClick) {
			generateMines(x, y);
			firstClick = false;
		}

		btn[y * width + x]->Enable(false);

		if (mines[y * width + x]) {
			showMines();
			wxMessageBox(wxT("Game Over!"), wxT("Minesweeper"));
			resetGame();
		} else {
			expand(x, y);

			if (isWin()) {
				wxMessageBox(wxT("You Win!"), wxT("Minesweeper"));
				resetGame();
			}
		}
	}

	event.Skip();
}

void cMain::onButtonRightClicked(wxMouseEvent& event)
{
	int x = (event.GetId() - 10000) % width;
	int y = (event.GetId() - 10000) / width;

	if (btn[y * width + x]->GetLabel() == wxT("\u25B2")) {
		btn[y * width + x]->SetLabel("");
	} else {
		btn[y * width + x]->SetLabel(wxT("\u25B2"));
	}
	
	event.Skip();
}

void cMain::expand(int x, int y) {
	int mine_count = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height) {
				if (mines[(y + j) * width + (x + i)]) {
					mine_count++;
				}
			}
		}
	}

	btn[y * width + x]->Enable(false);
	btn[y * width + x]->SetLabel("");
	
	if (mine_count) {
		btn[y * width + x]->SetLabel(std::to_string(mine_count));
		return;
	}
	
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height) {
				if (btn[(y + j) * width + (x + i)]->IsEnabled()) {
					expand(x + i, y + j);
				}
			}
		}
	}
}

void cMain::generateMines(int &x, int &y) {
	int nums = this->nums;
	srand(time(0));
	while (nums) {
		int rx = rand() % width;
		int ry = rand() % height;

		if (!mines[ry * width + rx] && rx != x && ry != y) {
			mines[ry * width + rx] = true;
			nums--;
		}
	}
}

void cMain::showMines() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (mines[y * width + x]) {
				btn[y * width + x]->SetLabel(wxT("X"));
				btn[y * width + x]->Enable(false);
			}
		}
	}
}

void cMain::resetGame() {
	firstClick = true;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			mines[y * width + x] = false;
			btn[y * width + x]->SetLabel("");
			btn[y * width + x]->Enable(true);
		}
	}
}

bool cMain::isWin() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (btn[y * width + x]->IsEnabled() && !mines[y * width + x]) {
				return false;
			}
		}
	}
	return true;
}
