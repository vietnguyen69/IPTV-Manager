/*
 * GoiKenhMenu.h
 * Menu quan ly Goi Kenh - Giao dien console cho use case CRUD Goi Kenh
 * De tai 7: Quan ly dich vu truyen hinh so/IPTV
 */
#ifndef GOI_KENH_MENU_H
#define GOI_KENH_MENU_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include "GoiKenh.h"
#include "Repository.h"
#include "Utils.h"

using namespace std;

class GoiKenhMenu {
private:
    Repository<GoiKenh>& repo;

    // ------- 1. THEM MOI -------
    void themGoiKenh() {
        Display::inTieuDe("THEM GOI KENH MOI", 60);
        try {
            GoiKenh gk;
            gk.inputData();
            if (repo.add(gk)) {
                Display::inThongBaoOK("Them goi kenh '" + gk.getId() + "' thanh cong!");
            } else {
                Display::inThongBaoLoi("Ma goi kenh '" + gk.getId() + "' da ton tai trong he thong!");
            }
        } catch (const exception& e) {
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 2. XEM DANH SACH -------
    void xemDanhSach() {
        Display::inTieuDe("DANH SACH GOI KENH", 65);
        Display::inBang(repo.getAll());
        Display::nhanPhimDeTiep();
    }

    // ------- 3. TIM KIEM -------
    void timKiem() {
        Display::inTieuDe("TIM KIEM GOI KENH", 60);
        cout << "  1. Tim theo ma goi\n";
        cout << "  2. Tim theo ten goi (gan dung)\n";
        cout << "  3. Tim theo loai goi\n";
        cout << "  4. Tim theo khoang gia thue\n";
        cout << "  0. Quay lai\n";
        int ch = InputHelper::nhapInt("  Lua chon: ", 0, 4);

        if (ch == 0) return;
        else if (ch == 1) {
            string ma = InputHelper::nhapChuoiBatBuoc("  Nhap ma goi: ");
            GoiKenh* gk = repo.findById(ma);
            if (gk) gk->display();
            else Display::inThongBaoLoi("Khong tim thay goi kenh voi ma '" + ma + "'!");
        }
        else if (ch == 2) {
            string ten = InputHelper::nhapChuoiBatBuoc("  Nhap tu khoa ten goi: ");
            string tenLower = StrUtils::toLower(ten);
            vector<GoiKenh> kq;
            for (const auto& gk : repo.getAll()) {
                if (StrUtils::toLower(gk.getTenGoi()).find(tenLower) != string::npos) {
                    kq.push_back(gk);
                }
            }
            if (kq.empty()) {
                cout << "  Khong tim thay ket qua phu hop.\n";
            } else {
                cout << "  Tim thay " << kq.size() << " ket qua:\n";
                Display::inBang(kq);
            }
        }
        else if (ch == 3) {
            cout << "  Loai goi (Co ban / Nang cao / Premium / VOD): ";
            string loai;
            getline(cin, loai);
            loai = StrUtils::trim(loai);
            vector<GoiKenh> kq;
            for (const auto& gk : repo.getAll()) {
                if (gk.getLoaiGoi() == loai) {
                    kq.push_back(gk);
                }
            }
            if (kq.empty()) {
                cout << "  Khong co goi kenh loai '" << loai << "'.\n";
            } else {
                cout << "  Tim thay " << kq.size() << " goi kenh loai '" << loai << "':\n";
                Display::inBang(kq);
            }
        }
        else if (ch == 4) {
            double giaMin = InputHelper::nhapDouble("  Gia thue tu (VND): ", 0.0);
            double giaMax = InputHelper::nhapDouble("  Gia thue den (VND): ", giaMin);
            vector<GoiKenh> kq;
            for (const auto& gk : repo.getAll()) {
                if (gk.getGiaThue() >= giaMin && gk.getGiaThue() <= giaMax) {
                    kq.push_back(gk);
                }
            }
            if (kq.empty()) {
                cout << "  Khong co goi kenh trong khoang gia nay.\n";
            } else {
                cout << "  Tim thay " << kq.size() << " ket qua:\n";
                Display::inBang(kq);
            }
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 4. SAP XEP -------
    void sapXep() {
        Display::inTieuDe("SAP XEP DANH SACH GOI KENH", 60);
        cout << "  1. Sap xep theo ten A-Z\n";
        cout << "  2. Sap xep theo gia tang dan\n";
        cout << "  3. Sap xep theo gia giam dan\n";
        cout << "  4. Sap xep theo so kenh tang dan\n";
        cout << "  0. Quay lai\n";
        int ch = InputHelper::nhapInt("  Lua chon: ", 0, 4);
        if (ch == 0) return;

        vector<GoiKenh> ds = repo.getAll();
        if (ds.empty()) {
            cout << "  (Chua co goi kenh nao trong he thong)\n";
            Display::nhanPhimDeTiep();
            return;
        }

        if (ch == 1) {
            sort(ds.begin(), ds.end(), [](const GoiKenh& a, const GoiKenh& b) {
                return StrUtils::toLower(a.getTenGoi()) < StrUtils::toLower(b.getTenGoi());
            });
            cout << "  Da sap xep theo ten A-Z.\n";
        } else if (ch == 2) {
            sort(ds.begin(), ds.end(), [](const GoiKenh& a, const GoiKenh& b) {
                return a.getGiaThue() < b.getGiaThue();
            });
            cout << "  Da sap xep theo gia tang dan.\n";
        } else if (ch == 3) {
            sort(ds.begin(), ds.end(), [](const GoiKenh& a, const GoiKenh& b) {
                return a.getGiaThue() > b.getGiaThue();
            });
            cout << "  Da sap xep theo gia giam dan.\n";
        } else if (ch == 4) {
            sort(ds.begin(), ds.end(), [](const GoiKenh& a, const GoiKenh& b) {
                return a.getSoKenh() < b.getSoKenh();
            });
            cout << "  Da sap xep theo so kenh tang dan.\n";
        }

        Display::inBang(ds);
        Display::nhanPhimDeTiep();
    }

    // ------- 5. CAP NHAT -------
    void capNhat() {
        Display::inTieuDe("CAP NHAT THONG TIN GOI KENH", 60);
        string ma = InputHelper::nhapChuoiBatBuoc("  Nhap ma goi kenh can sua: ");
        GoiKenh* gk = repo.findById(ma);
        if (!gk) {
            Display::inThongBaoLoi("Khong tim thay goi kenh voi ma '" + ma + "'!");
            Display::nhanPhimDeTiep();
            return;
        }

        gk->display();
        GoiKenh backup = *gk;

        try {
            gk->updateData();
            repo.saveAfterUpdate();
            Display::inThongBaoOK("Cap nhat goi kenh '" + ma + "' thanh cong!");
        } catch (const exception& e) {
            *gk = backup;
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 6. XOA -------
    void xoaGoiKenh(function<bool(const string&)> ktraKhachHangDungGoi) {
        Display::inTieuDe("XOA GOI KENH", 60);
        string ma = InputHelper::nhapChuoiBatBuoc("  Nhap ma goi kenh can xoa: ");
        GoiKenh* gk = repo.findById(ma);
        if (!gk) {
            Display::inThongBaoLoi("Khong tim thay goi kenh voi ma '" + ma + "'!");
            Display::nhanPhimDeTiep();
            return;
        }
        gk->display();

        // Kiem tra rang buoc: co khach hang nao dang dung goi nay khong?
        if (ktraKhachHangDungGoi && ktraKhachHangDungGoi(ma)) {
            Display::inThongBaoLoi("Khong the xoa! Con khach hang dang su dung goi kenh nay.");
            Display::nhanPhimDeTiep();
            return;
        }

        if (!InputHelper::xacNhan("  Ban co chac muon xoa goi kenh '" + ma + "'?")) {
            cout << "  Da huy thao tac xoa.\n";
            Display::nhanPhimDeTiep();
            return;
        }

        try {
            if (repo.remove(ma)) {
                Display::inThongBaoOK("Xoa goi kenh '" + ma + "' thanh cong!");
            } else {
                Display::inThongBaoLoi("Xoa that bai!");
            }
        } catch (const exception& e) {
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

public:
    GoiKenhMenu(Repository<GoiKenh>& r) : repo(r) {}

    void chay(function<bool(const string&)> ktraKhachHangDungGoi = nullptr) {
        hienThiMenu(ktraKhachHangDungGoi);
    }

    // Ham hien thi menu va xu ly
    void hienThiMenu(function<bool(const string&)> ktraKhachHangDungGoi = nullptr) {
        int luaChon;
        do {
            Display::inTieuDe("QUAN LY GOI KENH TRUYEN HINH", 60);
            cout << "  1. Them goi kenh moi\n";
            cout << "  2. Xem danh sach goi kenh\n";
            cout << "  3. Tim kiem goi kenh\n";
            cout << "  4. Sap xep danh sach\n";
            cout << "  5. Cap nhat thong tin goi kenh\n";
            cout << "  6. Xoa goi kenh\n";
            cout << "  0. Quay lai menu chinh\n";
            Display::inDuongKe(60);
            luaChon = InputHelper::nhapInt("  Nhap lua chon: ", 0, 6);

            switch (luaChon) {
                case 1: themGoiKenh(); break;
                case 2: xemDanhSach(); break;
                case 3: timKiem(); break;
                case 4: sapXep(); break;
                case 5: capNhat(); break;
                case 6: xoaGoiKenh(ktraKhachHangDungGoi); break;
                case 0: break;
            }
        } while (luaChon != 0);
    }
};

#endif // GOI_KENH_MENU_H
