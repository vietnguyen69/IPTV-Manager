/*
 * File   : LichPhatSongMenu.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Giao dien console cho use case Quan ly lich phat song (CRUD).
 */
#ifndef LICHPHATSONGMENU_H
#define LICHPHATSONGMENU_H

#include <iostream>
#include <string>
#include <vector>
#include "CommonUI.h"
#include "Repository.h"
#include "LichPhatSong.h"
#include "KenhTruyenHinh.h"

using namespace std;

class LichPhatSongMenu {
private:
    Repository<LichPhatSong>&   repoLichPS;
    Repository<KenhTruyenHinh>& repoKenh;

    void themLichPS() {
        printTitle("THEM LICH PHAT SONG MOI");
        if (repoKenh.isEmpty()) {
            cout << "\n  [CANH BAO] Chua co kenh truyen hinh nao!" << endl;
            return;
        }

        cout << "\n  --- Danh sach kenh hien co ---";
        displayTable(repoKenh.getAll());
        try {
            LichPhatSong lps;
            lps.inputData();
            if (repoKenh.findById(lps.getMaKenh()) == nullptr) {
                throw invalid_argument("Ma kenh khong ton tai!");
            }
            if (repoLichPS.add(lps)) {
                cout << "\n  >> Them lich phat song thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma lich da ton tai!" << endl;
            }
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    void hienThiLichPS() {
        printTitle("DANH SACH LICH PHAT SONG");
        displayTable(repoLichPS.getAll());
    }

    void timKiemLichPS() {
        printTitle("TIM KIEM LICH PHAT SONG");
        string keyword;
        cout << "\n  Nhap tu khoa: ";
        getline(cin, keyword);
        if (keyword.empty()) return;
        vector<LichPhatSong> results = repoLichPS.search(keyword);
        cout << "\n  Ket qua tim kiem cho '" << keyword << "':";
        displayTable(results);
    }

    void capNhatLichPS() {
        printTitle("CAP NHAT LICH PHAT SONG");
        string id;
        cout << "\n  Nhap ma lich can cap nhat: ";
        getline(cin, id);
        LichPhatSong* found = repoLichPS.findById(id);
        if (found == nullptr) return;

        found->display();
        cout << "\n  --- Danh sach kenh hien co ---";
        displayTable(repoKenh.getAll());

        LichPhatSong backup = *found;
        try {
            found->updateData();
            if (repoKenh.findById(found->getMaKenh()) == nullptr) {
                *found = backup;
                throw invalid_argument("Ma kenh khong ton tai!");
            }
            repoLichPS.saveAfterUpdate();
            cout << "\n  >> Cap nhat thanh cong!" << endl;
        } catch (const exception& e) {
            *found = backup;
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    void xoaLichPS() {
        printTitle("XOA LICH PHAT SONG");
        string id;
        cout << "\n  Nhap ma lich can xoa: ";
        getline(cin, id);
        LichPhatSong* found = repoLichPS.findById(id);
        if (found == nullptr) return;
        found->display();

        string confirm;
        cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
        getline(cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            if (repoLichPS.remove(id)) cout << "\n  >> Xoa thanh cong!" << endl;
        }
    }

public:
    LichPhatSongMenu(Repository<LichPhatSong>& rLPS, Repository<KenhTruyenHinh>& rK)
        : repoLichPS(rLPS), repoKenh(rK) {}

    void chay() {
        int choice = -1;
        do {
            clearScreen();
            printTitle("QUAN LY LICH PHAT SONG");
            cout << "\n  1. Them lich phat song\n  2. Hien thi danh sach\n  3. Tim kiem\n  4. Cap nhat\n  5. Xoa\n  0. << Quay lai" << endl;
            printLine();
            cout << "  Lua chon cua ban: ";
            string input;
            getline(cin, input);
            try { choice = stoi(input); } catch (...) { choice = -1; }

            switch (choice) {
                case 1: themLichPS(); pauseScreen(); break;
                case 2: hienThiLichPS(); pauseScreen(); break;
                case 3: timKiemLichPS(); pauseScreen(); break;
                case 4: capNhatLichPS(); pauseScreen(); break;
                case 5: xoaLichPS(); pauseScreen(); break;
                case 0: break;
                default:
                    cout << "\n  [LOI] Lua chon khong hop le!" << endl;
                    pauseScreen();
            }
        } while (choice != 0);
    }
};

#endif
