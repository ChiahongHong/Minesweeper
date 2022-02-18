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
	wxGridSizer *grid = new wxGridSizer(width, height, 0, 0);

	n = new int[width * height];

	wxFont font(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			btn[y * width + x] = new wxButton(this, 10000 + (y * width + x));
			btn[y * width + x]->SetFont(font);
			grid->Add(btn[y * width + x], 1, wxEXPAND | wxALL);

			btn[y * width + x]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::onButtonClicked, this);
			n[y * width + x] = 0;
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

	if (firstClick) {
		generateMines(x, y);
		firstClick = false;
	}

	btn[y * width + x]->Enable(false);

	if (n[y * width + x] == -1) {
		showMines();
		wxMessageBox(wxT("Game Over!"), wxT("Minesweeper"));
		resetGame();
	} else {
		int mine_count = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (x + i >= 0 && x + i < width && y + j >= 0 && y + j < height) {
					if (n[(y + j) * width + (x + i)] == -1) {
						mine_count++;
					}
				}
			}
		}
		if (mine_count > 0) {
			btn[y * width + x]->SetLabel(std::to_string(mine_count));
		}

		if (isWin()) {
			wxMessageBox(wxT("You Win!"), wxT("Minesweeper"));
			resetGame();
		}
	}

	event.Skip();
}

void cMain::generateMines(int &x, int &y) {
	int mines = this->mines;
	srand(time(0));
	while (mines) {
		int rx = rand() % width;
		int ry = rand() % height;

		if (n[ry * width + rx] == 0 && rx != x && ry != y) {
			n[ry * width + rx] = -1;
			mines--;
		}
	}
}

void cMain::showMines() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (n[y * width + x] == -1) {
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
			n[y * width + x] = 0;
			btn[y * width + x]->SetLabel("");
			btn[y * width + x]->Enable(true);
		}
	}
}

bool cMain::isWin() {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (btn[y * width + x]->IsEnabled() && n[y * width + x] == 0) {
				return false;
			}
		}
	}
	return true;
}
