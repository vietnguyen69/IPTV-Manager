// Tac gia: Do Gia Huy - B24DCVT172
// Menu Quan ly hop dong

#ifndef HOPDONG_MENU_H
#define HOPDONG_MENU_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "Repository.h"
#include "HopDong.h"
#include "HoaDon.h"
#include "CommonUI.h"

using namespace std;

class HopDongMenu {
private:
    Repository<HopDong>& repoHD;                           //Tham chieu den kho luu tru Hop Dong
    Repository<HoaDon>&  repoHoaDon;                      //Tham chieu den kho Hoa Don de chan xoa vi pham rang buoc
    function<bool(const string&)> khTonTai;               //Callback kiem tra maKhachHang co ton tai

    // 1. Chuc nang them hop dong moi
    void themHopDong() {
        printTitle("THEM HOP DONG MOI");
        try {
            HopDong hd;
            hd.inputData(); // Nhap thong tin hop dong

            // Kiem tra rang buoc khoa ngoai: khach hang co ton tai trong he thong khong
            if (khTonTai && !hd.getMaKhachHang().empty() && !khTonTai(hd.getMaKhachHang())) {
                cout << "\n  [CANH BAO] Ma khach hang '" << hd.getMaKhachHang() << "' chua co trong danh sach khach hang!" << endl;
                cout << "  Tiep tuc them hop dong? (Y/N): ";
                string cf; getline(cin, cf);
                if (cf != "Y" && cf != "y") {
                    cout << "  >> Da huy thao tac them." << endl;
                    return;
                }
            }

            // Them vao repository va kiem tra trung maHopDong
            if (repoHD.add(hd)) {
                cout << "\n  >> Them hop dong thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma hop dong '" << hd.getId() << "' da ton tai!" << endl;
            }
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    // 2. Chuc nang hien thi danh sach hop dong
    void hienThiHopDong() {
        printTitle("DANH SACH HOP DONG");
        displayTable(repoHD.getAll());
    }

    // 3. Chuc nang tim kiem hop dong theo tu khoa
    void timKiemHopDong() {
        printTitle("TIM KIEM HOP DONG");
        cout << "\n  Nhap tu khoa (ma hop dong, ma khach hang hoac goi dich vu): ";
        string keyword;
        getline(cin, keyword);
        if (keyword.empty()) {
            cout << "\n  [LOI] Tu khoa khong duoc de trong!" << endl;
            return;
        }
        displayTable(repoHD.search(keyword));
    }

    // 4. Chuc nang cap nhat hop dong
    void capNhatHopDong() {
        printTitle("CAP NHAT HOP DONG");
        cout << "\n  Nhap ma hop dong can cap nhat: ";
        string id;
        getline(cin, id);
        HopDong* found = repoHD.findById(id);
        if (!found) {
            cout << "\n  [LOI] Khong tim thay hop dong '" << id << "'!" << endl;
            return;
        }
        found->display();

        // Luu ban sao du phong truoc khi sua de rollback neu xay ra loi
        HopDong backup = *found;
        try {
            found->updateData();
            // Kiem tra lai ma khach hang sau khi cap nhat
            if (khTonTai && !found->getMaKhachHang().empty() && !khTonTai(found->getMaKhachHang())) {
                *found = backup;
                throw invalid_argument("Ma khach hang '" + found->getMaKhachHang() + "' khong ton tai!");
            }
            repoHD.saveAfterUpdate();
            cout << "\n  >> Cap nhat hop dong thanh cong!" << endl;
        } catch (const exception& e) {
            *found = backup;
            cout << "\n  [LOI] " << e.what() << "\n  Thay doi da duoc huy." << endl;
        }
    }

    // 5. Chuc nang xoa hop dong kem kiem tra rang buoc toan ven voi HoaDon
    void xoaHopDong() {
        printTitle("XOA HOP DONG");
        cout << "\n  Nhap ma hop dong can xoa: ";
        string id;
        getline(cin, id);
        HopDong* found = repoHD.findById(id);
        if (!found) {
            cout << "\n  [LOI] Khong tim thay hop dong '" << id << "'!" << endl;
            return;
        }
        found->display();

        // Kiem tra rang buoc: hop dong co hoa don nao dang tham chieu toi khong
        bool dangDuocDung = false;
        const auto& dsHoaDon = repoHoaDon.getAll();
        for (const auto& hd : dsHoaDon) {
            if (hd.getMaHopDong() == id) {
                dangDuocDung = true;
                break;
            }
        }
        if (dangDuocDung) {
            cout << "\n  [LOI] Khong the xoa: hop dong dang duoc hoa don su dung." << endl;
            cout << "  Hay xu ly cac hoa don lien quan truoc." << endl;
            return;
        }

        // Xac nhan truoc khi xoa
        cout << "\n  Ban co chac chan muon xoa hop dong '" << id << "'? (Y/N): ";
        string confirm;
        getline(cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            if (repoHD.remove(id)) {
                cout << "\n  >> Xoa hop dong thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Xoa that bai!" << endl;
            }
        } else {
            cout << "\n  >> Da huy thao tac xoa." << endl;
        }
    }

public:
    // Constructor nhan cac Repository va callback kiem tra khoa ngoai
    HopDongMenu(Repository<HopDong>& rHD, Repository<HoaDon>& rHoaDon,
                function<bool(const string&)> kh = nullptr)
        : repoHD(rHD), repoHoaDon(rHoaDon), khTonTai(kh) {}

    // Ham khoi dong menu con cua Hop Dong
    void chay() {
        hienThiMenu();
    }

    // Vong lap hien thi va xu ly lua chon menu
    void hienThiMenu() {
        int choice = -1;
        do {
            clearScreen();
            printTitle("QUAN LY HOP DONG IPTV");
            cout << "\n  1. Them hop dong" << endl;
            cout << "  2. Hien thi danh sach hop dong" << endl;
            cout << "  3. Tim kiem hop dong" << endl;
            cout << "  4. Cap nhat thong tin hop dong" << endl;
            cout << "  5. Xoa hop dong" << endl;
            cout << "  0. Quay lai menu chinh" << endl;
            printLine();
            cout << "  Lua chon cua ban: ";
            string input;
            getline(cin, input);
            try {
                choice = stoi(input);
            } catch (...) {
                cout << "\n  [LOI] Vui long nhap mot so!" << endl;
                pauseScreen();
                continue;
            }

            switch (choice) {
                case 1: themHopDong(); pauseScreen(); break;
                case 2: hienThiHopDong(); pauseScreen(); break;
                case 3: timKiemHopDong(); pauseScreen(); break;
                case 4: capNhatHopDong(); pauseScreen(); break;
                case 5: xoaHopDong(); pauseScreen(); break;
                case 0: break;
                default:
                    cout << "\n  [LOI] Lua chon khong hop le (0-5)!" << endl;
                    pauseScreen();
            }
        } while (choice != 0);
    }
};

#endif