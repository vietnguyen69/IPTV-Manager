// Tac gia: Duong Duc Manh - B24DCVT240
//Menu quan ly kenh truyen hinh

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
    Repository<KenhTruyenHinh>& repoKenh;     // Tham chieu den kho luu tru kenh truyen hinh
    Repository<TheLoaiKenh>&    repoTheLoai;  // Tham chieu den kho the loai (de kiem tra khoa ngoai)
    Repository<LichPhatSong>&   repoLichPS;   // Tham chieu den lich phat song (de kiem tra rang buoc xoa)

    //Chuc nang them kenh truyen hinh moi
    void themKenh() {
        printTitle("THEM KENH TRUYEN HINH MOI");
        // Neu chua co the loai nao thi khong the them kenh
        if (repoTheLoai.isEmpty()) {
            cout << "\n  [CANH BAO] Chua co the loai kenh! Vui long them the loai truoc." << endl;
            return;
        }

        //Hien thi danh sach the loai hien co de nguoi dung tien tham khao
        cout << "\n  --- Danh sach the loai hien co ---";
        displayTable(repoTheLoai.getAll());
        cout << endl;
        try {
            KenhTruyenHinh kenh;
            kenh.inputData(); //Nhap du lieu tu ban phim
            //Kiem tra khoa ngoai: ma the loai vua nhap co ton tai khong
            if (repoTheLoai.findById(kenh.getMaTheLoai()) == nullptr) {
                throw invalid_argument("Ma the loai '" + kenh.getMaTheLoai() + "' khong ton tai!");
            }
            //Them vao repository va kiem tra trung khoa chinh
            if (repoKenh.add(kenh)) {
                cout << "\n  >> Them kenh truyen hinh thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma kenh da ton tai!" << endl;
            }
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    //Chuc nang hien thi toan bo danh sach kenh truyen hinh
    void hienThiKenh() {
        printTitle("DANH SACH KENH TRUYEN HINH");
        displayTable(repoKenh.getAll());
    }

    //Chuc nang tim kiem kenh theo tu khoa (ma kenh hoac ten kenh)
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

    //Chuc nang cap nhat thong tin kenh
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

        //Luu ban sao du phong truoc khi sua, neu loi thi rollback
        KenhTruyenHinh backup = *found;
        try {
            found->updateData();
            //Kiem tra khoa ngoai ma the loai sau khi cap nhat
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

    //Chuc nang xoa kenh kem kiem tra rang buoc tham chieu
    void xoaKenh() {
        printTitle("XOA KENH TRUYEN HINH");
        string id;
        cout << "\n  Nhap ma kenh can xoa: ";
        getline(cin, id);
        KenhTruyenHinh* found = repoKenh.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay kenh can xoa!" << endl;
            return;
        }
        found->display();

        //Kiem tra rang buoc: Kenh nay co dang co lich phat song hay khong
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

        //Xac nhan truoc khi xoa
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
    //Constructor truyen cac Repository phu thuoc
    KenhTruyenHinhMenu(Repository<KenhTruyenHinh>& rK, Repository<TheLoaiKenh>& rTL, Repository<LichPhatSong>& rLPS)
        : repoKenh(rK), repoTheLoai(rTL), repoLichPS(rLPS) {}

    //Vong lap chay menu con cua Kenh Truyen Hinh
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