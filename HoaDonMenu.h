// Tac gia: Do Gia Huy - B24DCVT172
//Menu quan ly hoa don

#ifndef HOADON_MENU_H
#define HOADON_MENU_H

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <functional>
#include "Repository.h"
#include "HoaDon.h"
#include "HopDong.h"
#include "CommonUI.h"

using namespace std;

class HoaDonMenu {
private:
    Repository<HoaDon>&  repoHoaDon;   // Tham chieu den kho luu tru Hoa Don
    Repository<HopDong>& repoHopDong;  // Tham chieu den kho Hop Dong de kiem tra khoa ngoai

    // 1. Chuc nang them hoa don moi
    void themHoaDon() {
        printTitle("THEM HOA DON MOI");
        // Neu chua co hop dong nao trong he thong thi khong the phat sinh hoa don
        if (repoHopDong.isEmpty()) {
            cout << "\n  [CANH BAO] Chua co hop dong nao trong he thong. Hay them hop dong truoc!" << endl;
            return;
        }
        // Hien thi danh sach hop dong hien co de nguoi dung chon ma hop dong
        cout << "\n  --- Danh sach hop dong hien co ---" << endl;
        displayTable(repoHopDong.getAll());

        try {
            HoaDon hd;
            hd.inputData(); // Nhap du lieu hoa don tu ban phim

            // Kiem tra khoa ngoai: maHopDong vua nhap phai ton tai
            if (repoHopDong.findById(hd.getMaHopDong()) == nullptr) {
                throw invalid_argument("Ma hop dong '" + hd.getMaHopDong() + "' khong ton tai!");
            }

            // Them vao repository va kiem tra trung maHoaDon
            if (repoHoaDon.add(hd)) {
                cout << "\n  >> Them hoa don thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Ma hoa don '" << hd.getId() << "' da ton tai!" << endl;
            }
        } catch (const exception& e) {
            cout << "\n  [LOI] " << e.what() << endl;
        }
    }

    // 2. Chuc nang hien thi toan bo danh sach hoa don
    void hienThiHoaDon() {
        printTitle("DANH SACH HOA DON");
        displayTable(repoHoaDon.getAll());
    }

    // 3. Chuc nang tim kiem hoa don theo tu khoa
    void timKiemHoaDon() {
        printTitle("TIM KIEM HOA DON");
        string keyword;
        cout << "\n  Nhap tu khoa (ma hoa don, ma hop dong hoac ky cuoc): ";
        getline(cin, keyword);
        if (keyword.empty()) {
            cout << "\n  [LOI] Tu khoa khong duoc de trong!" << endl;
            return;
        }
        displayTable(repoHoaDon.search(keyword));
    }

    // 4. Chuc nang cap nhat thong tin hoa don
    void capNhatHoaDon() {
        printTitle("CAP NHAT HOA DON");
        string id;
        cout << "\n  Nhap ma hoa don can cap nhat: ";
        getline(cin, id);
        HoaDon* found = repoHoaDon.findById(id);
        if (!found) {
            cout << "\n  [LOI] Khong tim thay hoa don '" << id << "'!" << endl;
            return;
        }
        found->display();
        // Luu ban sao de rollback neu gap loi
        HoaDon backup = *found;
        try {
            found->updateData();
            // Kiem tra khoa ngoai maHopDong sau khi sua
            if (repoHopDong.findById(found->getMaHopDong()) == nullptr) {
                *found = backup;
                throw invalid_argument("Ma hop dong '" + found->getMaHopDong() + "' khong ton tai!");
            }
            repoHoaDon.saveAfterUpdate();
            cout << "\n  >> Cap nhat hoa don thanh cong!" << endl;
        } catch (const exception& e) {
            *found = backup;
            cout << "\n  [LOI] " << e.what() << "\n  Thay doi da duoc huy." << endl;
        }
    }

    // 5. Chuc nang xu ly thanh toan hoa don (chuyen trang thai)
    void thanhToanHoaDon() {
        printTitle("THANH TOAN HOA DON");
        string id;
        cout << "\n  Nhap ma hoa don can thanh toan: ";
        getline(cin, id);
        HoaDon* found = repoHoaDon.findById(id);
        if (!found) {
            cout << "\n  [LOI] Khong tim thay hoa don '" << id << "'!" << endl;
            return;
        }
        found->display();
        if (found->getTrangThaiThanhToan() == "Da thanh toan") {
            cout << "\n  [THONG BAO] Hoa don nay DA duoc thanh toan truoc do." << endl;
            return;
        }
        cout << "\n  Xac nhan danh dau hoa don '" << id << "' da thanh toan? (Y/N): ";
        string cf;
        getline(cin, cf);
        if (cf == "Y" || cf == "y") {
            found->setTrangThaiThanhToan("Da thanh toan");
            repoHoaDon.saveAfterUpdate(); // Dong bo file
            cout << "\n  >> Thanh toan hoa don thanh cong!" << endl;
        } else {
            cout << "\n  >> Da huy thao tac thanh toan." << endl;
        }
    }

    // 6. Chuc nang xoa hoa don
    void xoaHoaDon() {
        printTitle("XOA HOA DON");
        string id;
        cout << "\n  Nhap ma hoa don can xoa: ";
        getline(cin, id);
        HoaDon* found = repoHoaDon.findById(id);
        if (!found) {
            cout << "\n  [LOI] Khong tim thay hoa don '" << id << "'!" << endl;
            return;
        }
        found->display();
        cout << "\n  Ban co chac chan muon xoa hoa don '" << id << "'? (Y/N): ";
        string confirm;
        getline(cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            if (repoHoaDon.remove(id)) {
                cout << "\n  >> Xoa hoa don thanh cong!" << endl;
            } else {
                cout << "\n  [LOI] Xoa that bai!" << endl;
            }
        } else {
            cout << "\n  >> Da huy thao tac xoa." << endl;
        }
    }

    // 7. Chuc nang thong ke doanh thu va cong no chua thu
    void thongKeHoaDon() {
        printTitle("THONG KE HOA DON VA DOANH THU");
        const auto& ds = repoHoaDon.getAll();
        if (ds.empty()) {
            cout << "\n  [Chua co du lieu hoa don de thong ke]\n";
            return;
        }

        int daTT = 0, chuaTT = 0, quaHan = 0;
        double tongDoanhThu = 0;
        double tongChuaThu = 0;

        // Duyet qua toan bo danh sach de tong hop so lieu
        for (const auto& hd : ds) {
            if (hd.getTrangThaiThanhToan() == "Da thanh toan") {
                daTT++;
                tongDoanhThu += hd.getSoTien();
            } else if (hd.getTrangThaiThanhToan() == "Qua han") {
                quaHan++;
                tongChuaThu += hd.getSoTien();
            } else {
                chuaTT++;
                tongChuaThu += hd.getSoTien();
            }
        }

        cout << "\n  Tong so hoa don       : " << ds.size() << endl;
        cout << "  So hoa don da tra     : " << daTT << endl;
        cout << "  So hoa don chua tra   : " << chuaTT << endl;
        cout << "  So hoa don qua han    : " << quaHan << endl;
        cout << "  ----------------------------------------" << endl;
        cout << "  Tong doanh thu da thu : " << fixed << setprecision(0) << tongDoanhThu << " VND" << endl;
        cout << "  Tong cong no chua thu : " << fixed << setprecision(0) << tongChuaThu << " VND" << endl;
    }

public:
    // Constructor truyen cac Repository phu thuoc
    HoaDonMenu(Repository<HoaDon>& rHD, Repository<HopDong>& rHopDong)
        : repoHoaDon(rHD), repoHopDong(rHopDong) {}

    // Ham chay menu con cua Hoa Don
    void chay() {
        hienThiMenu();
    }

    // Vong lap dieu huong menu con
    void hienThiMenu() {
        int choice = -1;
        do {
            clearScreen();
            printTitle("QUAN LY HOA DON VA THANH TOAN");
            cout << "\n  1. Them hoa don" << endl;
            cout << "  2. Hien thi danh sach hoa don" << endl;
            cout << "  3. Tim kiem hoa don" << endl;
            cout << "  4. Cap nhat thong tin hoa don" << endl;
            cout << "  5. Xoa hoa don" << endl;
            cout << "  6. Thanh toan hoa don" << endl;
            cout << "  7. Thong ke doanh thu va cong no" << endl;
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
                case 1: themHoaDon(); pauseScreen(); break;
                case 2: hienThiHoaDon(); pauseScreen(); break;
                case 3: timKiemHoaDon(); pauseScreen(); break;
                case 4: capNhatHoaDon(); pauseScreen(); break;
                case 5: xoaHoaDon(); pauseScreen(); break;
                case 6: thanhToanHoaDon(); pauseScreen(); break;
                case 7: thongKeHoaDon(); pauseScreen(); break;
                case 0: break;
                default:
                    cout << "\n  [LOI] Lua chon khong hop le (0-7)!" << endl;
                    pauseScreen();
            }
        } while (choice != 0);
    }
};

#endif