/**
 * =====================================================================================
 * De tai 7: Quan ly dich vu truyen hinh so / IPTV
 * File   : NoiDungVODMenu.h
 * Mo ta  : Lop Dieu khien giao dien (Controller/Menu) cho Use case: Quan ly Noi dung VOD.
 *          Thuc hien day du cac chuc nang CRUD, kiem tra toan ven rang buoc khoa ngoai
 *          voi bang TheLoaiKenh khi them va cap nhat noi dung.
 * =====================================================================================
 */
#ifndef NOIDUNGVODMENU_H
#define NOIDUNGVODMENU_H

#include <iostream>
#include <string>
#include <vector>
#include "CommonUI.h"
#include "Repository.h"
#include "NoiDungVOD.h"
#include "TheLoaiKenh.h"

using namespace std;

class NoiDungVODMenu {
private:
    Repository<NoiDungVOD>&   repoVOD;      // Kho luu tru noi dung VOD
    Repository<TheLoaiKenh>& repoTheLoai;  // Kho the loai kenh (dung kiem tra khoa ngoai maTheLoai)

    // ----- 1. THEM NOI DUNG VOD MOI (Create) -----
    void themVOD() {
        printTitle("THEM NOI DUNG VOD MOI");

        // Kiem tra tien dieu kien: neu chua co the loai kenh nao thi khong the them VOD
        if (repoTheLoai.isEmpty()) {
            cout << "\n  [CANH BAO] Chua co the loai kenh nao trong he thong!" << endl;
            cout << "  Vui long them the loai kenh truoc khi them noi dung VOD." << endl;
            return;
        }

        // Hien thi danh sach the loai hien co de nguoi dung tien tra cuu ma
        cout << "\n  --- Danh sach the loai hien co ---";
        displayTable(repoTheLoai.getAll());
        cout << endl;

        try {
            NoiDungVOD vod;
            vod.inputData(); // Nhap du lieu tu ban phim

            // Kiem tra rang buoc khoa ngoai: ma the loai phai ton tai trong he thong
            if (repoTheLoai.findById(vod.getMaTheLoai()) == nullptr) {
                throw invalid_argument("Ma the loai '" + vod.getMaTheLoai() + "' khong ton tai!");
            }

            // Them vao Repository: tu dong kiem tra trung ma VOD va ghi xuong file
            if (repoVOD.add(vod)) {
                cout << "\n  >> Them noi dung VOD thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma VOD '" << vod.getId() << "' da ton tai!" << endl;
            }
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    // ----- 2. HIEN THI DANH SACH VOD (Read All) -----
    void hienThiVOD() {
        printTitle("DANH SACH NOI DUNG VOD");
        displayTable(repoVOD.getAll()); // Dung template displayTable in bang chuan
    }

    // ----- 3. TIM KIEM NOI DUNG VOD (Search) -----
    void timKiemVOD() {
        printTitle("TIM KIEM NOI DUNG VOD");
        string keyword;
        cout << "\n  Nhap tu khoa (ma VOD hoac tieu de): ";
        getline(cin, keyword);

        if (keyword.empty()) {
            cout << "\n  [LOI] Tu khoa khong duoc de trong!" << endl;
            return;
        }

        // Tim kiem theo ma hoac tieu de khong phan biet hoa thuong
        vector<NoiDungVOD> results = repoVOD.search(keyword);
        cout << "\n  Ket qua tim kiem cho '" << keyword << "':";
        displayTable(results);
    }

    // ----- 4. CAP NHAT THONG TIN VOD (Update) -----
    void capNhatVOD() {
        printTitle("CAP NHAT NOI DUNG VOD");
        string id;
        cout << "\n  Nhap ma VOD can cap nhat: ";
        getline(cin, id);

        NoiDungVOD* found = repoVOD.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay ban ghi voi ma '" << id << "'!" << endl;
            return;
        }

        cout << "\n  --- Thong tin hien tai ---" << endl;
        found->display();

        cout << "\n  --- Danh sach the loai hien co ---";
        displayTable(repoTheLoai.getAll());

        NoiDungVOD backup = *found; // Sao luu trang thai cu
        try {
            found->updateData(); // Cho phep nhan Enter de giu nguyen gia tri

            // Kiem tra ma the loai sau khi sua co hop le khong
            if (repoTheLoai.findById(found->getMaTheLoai()) == nullptr) {
                *found = backup; // Khoi phuc neu the loai moi khong ton tai
                throw invalid_argument("Ma the loai '" + found->getMaTheLoai() + "' khong ton tai!");
            }

            repoVOD.saveAfterUpdate(); // Ghi nhan thay doi vao file
            cout << "\n  >> Cap nhat noi dung VOD thanh cong!" << endl;
        } catch (const exception& e) {
            *found = backup;
            cout << "\n  [LOI] " << e.what() << "\n  Thay doi da bi huy bo." << endl;
        }
    }

    // ----- 5. XOA NOI DUNG VOD (Delete) -----
    void xoaVOD() {
        printTitle("XOA NOI DUNG VOD");
        string id;
        cout << "\n  Nhap ma VOD can xoa: ";
        getline(cin, id);

        NoiDungVOD* found = repoVOD.findById(id);
        if (found == nullptr) {
            cout << "\n  [LOI] Khong tim thay ban ghi voi ma '" << id << "'!" << endl;
            return;
        }

        cout << "\n  --- Thong tin se bi xoa ---" << endl;
        found->display();

        // Xac nhan nguoi dung truoc khi thuc hien xoa
        string confirm;
        cout << "\n  Ban co chac chan muon xoa noi dung nay? (Y/N): ";
        getline(cin, confirm);

        if (confirm == "Y" || confirm == "y") {
            if (repoVOD.remove(id)) {
                cout << "\n  >> Xoa noi dung VOD thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Xoa that bai!" << endl;
            }
        } else {
            cout << "\n  >> Da huy thao tac xoa." << endl;
        }
    }

public:
    // Constructor nhan tham chieu 2 Repository
    NoiDungVODMenu(Repository<NoiDungVOD>& rVOD, Repository<TheLoaiKenh>& rTL)
        : repoVOD(rVOD), repoTheLoai(rTL) {}

    // Vong lap dieu huong menu use case
    void chay() {
        int choice = -1;
        do {
            clearScreen();
            printTitle("QUAN LY NOI DUNG VOD");
            cout << "\n  1. Them VOD moi\n"
                 << "  2. Hien thi danh sach VOD\n"
                 << "  3. Tim kiem VOD\n"
                 << "  4. Cap nhat thong tin VOD\n"
                 << "  5. Xoa noi dung VOD\n"
                 << "  0. << Quay lai menu chinh" << endl;
            printLine();
            cout << "  Lua chon cua ban: ";
            string input;
            getline(cin, input);
            try { 
                choice = stoi(input); 
            } catch (...) { 
                choice = -1; 
            }

            switch (choice) {
                case 1: themVOD();     pauseScreen(); break;
                case 2: hienThiVOD();  pauseScreen(); break;
                case 3: timKiemVOD();  pauseScreen(); break;
                case 4: capNhatVOD();  pauseScreen(); break;
                case 5: xoaVOD();      pauseScreen(); break;
                case 0: break;
                default:
                    cout << "\n  [LOI] Lua chon khong hop le! Vui long nhap tu 0 den 5." << endl;
                    pauseScreen();
            }
        } while (choice != 0);
    }
};

#endif // NOIDUNGVODMENU_H
