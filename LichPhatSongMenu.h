// Tac gia: Duong Duc Manh - B24DCVT240
// Menu Quan ly lich phat song

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
    Repository<LichPhatSong>&   repoLichPS;  // Tham chieu den kho luu tru lich phat song
    Repository<KenhTruyenHinh>& repoKenh;    // Tham chieu den kho kenh (kiem tra khoa ngoai maKenh)

    // 1. Chuc nang them lich phat song moi
    void themLichPS() {
        printTitle("THEM LICH PHAT SONG MOI");
        // Neu chua co kenh nao thi khong the len lich
        if (repoKenh.isEmpty()) {
            cout << "\n  [CANH BAO] Chua co kenh truyen hinh nao! Vui long them kenh truoc." << endl;
            return;
        }

        // Hien thi danh sach kenh de nguoi dung chon ma kenh hop le
        cout << "\n  --- Danh sach kenh hien co ---";
        displayTable(repoKenh.getAll());
        try {
            LichPhatSong lps;
            lps.inputData(); // Nhap du lieu lich
            // Kiem tra khoa ngoai maKenh
            if (repoKenh.findById(lps.getMaKenh()) == nullptr) {
                throw invalid_argument("Ma kenh khong ton tai!");
            }
            // Them vao repository va kiem tra trung maLich
            if (repoLichPS.add(lps)) {
                cout << "\n  >> Them lich phat song thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma lich da ton tai!" << endl;
            }
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    // 2. Chuc nang hien thi toan bo danh sach lich phat song
    void hienThiLichPS() {
        printTitle("DANH SACH LICH PHAT SONG");
        displayTable(repoLichPS.getAll());
    }

    // 3. Chuc nang tim kiem lich theo tu khoa (ten chuong trinh / ma lich)
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

    // 4. Chuc nang cap nhat lich phat song
    void capNhatLichPS() {
        printTitle("CAP NHAT LICH PHAT SONG");
        string id;
        cout << "\n  Nhap ma lich can cap nhat: ";
        getline(cin, id);
        LichPhatSong* found = repoLichPS.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay ma lich '" << id << "'!" << endl;
            return;
        }

        found->display();
        cout << "\n  --- Danh sach kenh hien co ---";
        displayTable(repoKenh.getAll());

        // Luu ban sao du phong truoc khi sua de rollback neu loi
        LichPhatSong backup = *found;
        try {
            found->updateData();
            // Kiem tra khoa ngoai maKenh sau khi sua
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

    // 5. Chuc nang xoa lich phat song
    void xoaLichPS() {
        printTitle("XOA LICH PHAT SONG");
        string id;
        cout << "\n  Nhap ma lich can xoa: ";
        getline(cin, id);
        LichPhatSong* found = repoLichPS.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay lich can xoa!" << endl;
            return;
        }
        found->display();

        string confirm;
        cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
        getline(cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            if (repoLichPS.remove(id)) cout << "\n  >> Xoa thanh cong!" << endl;
            else cout << "\n  [LOI] Xoa that bai!" << endl;
        } else {
            cout << "\n  >> Da huy thao tac xoa." << endl;
        }
    }

public:
    // Constructor truyen cac kho du lieu phu thuoc
    LichPhatSongMenu(Repository<LichPhatSong>& rLPS, Repository<KenhTruyenHinh>& rK)
        : repoLichPS(rLPS), repoKenh(rK) {}

    // Vong lap dieu huong menu con cua Lich Phat Song
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