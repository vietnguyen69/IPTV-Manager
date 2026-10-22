/*
 * File   : KenhTruyenHinhMenu.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Giao dien console cho use case Quan ly kenh truyen hinh (CRUD).
 */
#ifndef KENHTRUYENHINHMENU_H
#define KENHTRUYENHINHMENU_H

#include <iostream>
#include <string>
#include <vector>
#include "CommonUI.h"
#include "Repository.h"
#include "KenhTruyenHinh.h"
#include "TheLoaiKenh.h"
#include "LichPhatSong.h"

using namespace std;

class KenhTruyenHinhMenu {
private:
    Repository<KenhTruyenHinh>& repoKenh;
    Repository<TheLoaiKenh>&    repoTheLoai;
    Repository<LichPhatSong>&   repoLichPS;

    void themKenh() {
        printTitle("THEM KENH TRUYEN HINH MOI");
        if (repoTheLoai.isEmpty()) {
            cout << "\n  [CANH BAO] Chua co the loai kenh!" << endl;
            return;
        }

        cout << "\n  --- Danh sach the loai hien co ---";
        displayTable(repoTheLoai.getAll());
        cout << endl;
        try {
            KenhTruyenHinh kenh;
            kenh.inputData();
            if (repoTheLoai.findById(kenh.getMaTheLoai()) == nullptr) {
                throw invalid_argument("Ma the loai '" + kenh.getMaTheLoai() + "' khong ton tai!");
            }
            if (repoKenh.add(kenh)) {
                cout << "\n  >> Them kenh truyen hinh thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma kenh da ton tai!" << endl;
            }
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    void hienThiKenh() {
        printTitle("DANH SACH KENH TRUYEN HINH");
        displayTable(repoKenh.getAll());
    }

    void timKiemKenh() {
        printTitle("TIM KIEM KENH TRUYEN HINH");
        string keyword;
        cout << "\n  Nhap tu khoa: ";
        getline(cin, keyword);
        if (keyword.empty()) return;
        vector<KenhTruyenHinh> results = repoKenh.search(keyword);
        cout << "\n  Ket qua tim kiem cho '" << keyword << "':";
        displayTable(results);
    }

    void capNhatKenh() {
        printTitle("CAP NHAT KENH TRUYEN HINH");
        string id;
        cout << "\n  Nhap ma kenh can cap nhat: ";
        getline(cin, id);
        KenhTruyenHinh* found = repoKenh.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay ma '" << id << "'!" << endl;
            return;
        }

        found->display();
        cout << "\n  --- Danh sach the loai hien co ---";
        displayTable(repoTheLoai.getAll());

        KenhTruyenHinh backup = *found;
        try {
            found->updateData();
            if (repoTheLoai.findById(found->getMaTheLoai()) == nullptr) {
                *found = backup;
                throw invalid_argument("Ma the loai khong ton tai!");
            }
            repoKenh.saveAfterUpdate();
            cout << "\n  >> Cap nhat thanh cong!" << endl;
        } catch (const exception& e) {
            *found = backup;
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    void xoaKenh() {
        printTitle("XOA KENH TRUYEN HINH");
        string id;
        cout << "\n  Nhap ma kenh can xoa: ";
        getline(cin, id);
        KenhTruyenHinh* found = repoKenh.findById(id);
        if (found == nullptr) return;
        found->display();

        bool dangSuDung = false;
        for (const auto& lps : repoLichPS.getAll()) {
            if (lps.getMaKenh() == id) {
                dangSuDung = true;
                break;
            }
        }
        if (dangSuDung) {
            cout << "\n  [LOI] Khong the xoa! Co lich phat song dang dung kenh nay." << endl;
            return;
        }

        string confirm;
        cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
        getline(cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            if (repoKenh.remove(id)) cout << "\n  >> Xoa thanh cong!" << endl;
            else cout << "\n  [LOI] Xoa that bai!" << endl;
        } else {
            cout << "\n  >> Da huy thao tac xoa." << endl;
        }
    }

public:
    KenhTruyenHinhMenu(Repository<KenhTruyenHinh>& rK, Repository<TheLoaiKenh>& rTL, Repository<LichPhatSong>& rLPS)
        : repoKenh(rK), repoTheLoai(rTL), repoLichPS(rLPS) {}

    void chay() {
        int choice = -1;
        do {
            clearScreen();
            printTitle("QUAN LY KENH TRUYEN HINH");
            cout << "\n  1. Them kenh\n  2. Hien thi danh sach\n  3. Tim kiem\n  4. Cap nhat\n  5. Xoa\n  0. << Quay lai" << endl;
            printLine();
            cout << "  Lua chon cua ban: ";
            string input;
            getline(cin, input);
            try { choice = stoi(input); } catch (...) { choice = -1; }

            switch (choice) {
                case 1: themKenh(); pauseScreen(); break;
                case 2: hienThiKenh(); pauseScreen(); break;
                case 3: timKiemKenh(); pauseScreen(); break;
                case 4: capNhatKenh(); pauseScreen(); break;
                case 5: xoaKenh(); pauseScreen(); break;
                case 0: break;
                default:
                    cout << "\n  [LOI] Lua chon khong hop le!" << endl;
                    pauseScreen();
            }
        } while (choice != 0);
    }
};

#endif
