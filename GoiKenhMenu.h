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
        GoiKenh gk;
        try {
            gk.nhapDuLieu();
            repo.add(gk);
            Display::inThongBaoOK("Them goi kenh '" + gk.getId() + "' thanh cong!");
        } catch (const DuplicateIdException& e) {
            Display::inThongBaoLoi(e.what());
        } catch (const InvalidInputException& e) {
            Display::inThongBaoLoi(e.what());
        } catch (const FileException& e) {
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 2. XEM DANH SACH -------
    void xemDanhSach() {
        Display::inTieuDe("DANH SACH GOI KENH", 65);
        const auto& ds = repo.getAll();
        if (ds.empty()) {
            cout << "  (Chua co goi kenh nao trong he thong)\n";
            Display::nhanPhimDeTiep();
            return;
        }
        GoiKenh::inTieuDeBang();
        for (const auto& gk : ds) {
            gk.hienThiDanhSach();
        }
        cout << "\n  Tong so: " << ds.size() << " goi kenh.\n";
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
            if (gk) gk->hienThi();
            else Display::inThongBaoLoi("Khong tim thay goi kenh voi ma '" + ma + "'!");
        }
        else if (ch == 2) {
            string ten = InputHelper::nhapChuoiBatBuoc("  Nhap tu khoa ten goi: ");
            string tenLower = StrUtils::toLower(ten);
            auto kq = repo.findWhere([&tenLower](const GoiKenh& gk) {
                return StrUtils::toLower(gk.getTenGoi()).find(tenLower) != string::npos;
            });
            if (kq.empty()) {
                cout << "  Khong tim thay ket qua phu hop.\n";
            } else {
                cout << "  Tim thay " << kq.size() << " ket qua:\n\n";
                GoiKenh::inTieuDeBang();
                for (auto* gk : kq) gk->hienThiDanhSach();
            }
        }
        else if (ch == 3) {
            cout << "  Loai goi (Co ban / Nang cao / Premium / VOD): ";
            string loai;
            getline(cin, loai);
            auto kq = repo.findWhere([&loai](const GoiKenh& gk) {
                return gk.getLoaiGoi() == loai;
            });
            if (kq.empty()) {
                cout << "  Khong co goi kenh loai '" << loai << "'.\n";
            } else {
                cout << "  Tim thay " << kq.size() << " goi kenh loai '" << loai << "':\n\n";
                GoiKenh::inTieuDeBang();
                for (auto* gk : kq) gk->hienThiDanhSach();
            }
        }
        else if (ch == 4) {
            double giaMin = InputHelper::nhapDouble("  Gia thue tu (VND): ", 0.0);
            double giaMax = InputHelper::nhapDouble("  Gia thue den (VND): ", giaMin);
            auto kq = repo.findWhere([giaMin, giaMax](const GoiKenh& gk) {
                return gk.getGiaThue() >= giaMin && gk.getGiaThue() <= giaMax;
            });
            if (kq.empty()) {
                cout << "  Khong co goi kenh trong khoang gia nay.\n";
            } else {
                cout << "  Tim thay " << kq.size() << " ket qua:\n\n";
                GoiKenh::inTieuDeBang();
                for (auto* gk : kq) gk->hienThiDanhSach();
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

        if (ch == 1) {
            repo.sortBy([](const GoiKenh& a, const GoiKenh& b) {
                return StrUtils::toLower(a.getTenGoi()) < StrUtils::toLower(b.getTenGoi());
            });
            cout << "  Da sap xep theo ten A-Z.\n";
        } else if (ch == 2) {
            repo.sortBy([](const GoiKenh& a, const GoiKenh& b) {
                return a.getGiaThue() < b.getGiaThue();
            });
            cout << "  Da sap xep theo gia tang dan.\n";
        } else if (ch == 3) {
            repo.sortBy([](const GoiKenh& a, const GoiKenh& b) {
                return a.getGiaThue() > b.getGiaThue();
            });
            cout << "  Da sap xep theo gia giam dan.\n";
        } else if (ch == 4) {
            repo.sortBy([](const GoiKenh& a, const GoiKenh& b) {
                return a.getSoKenh() < b.getSoKenh();
            });
            cout << "  Da sap xep theo so kenh tang dan.\n";
        }
        // Hien thi ket qua sau khi sap xep
        cout << "\n";
        GoiKenh::inTieuDeBang();
        for (const auto& gk : repo.getAll()) gk.hienThiDanhSach();
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
        gk->hienThi();
        cout << "  (Nhan Enter de giu nguyen gia tri cu)\n\n";

        try {
            // Cap nhat tung truong, bo trong = giu nguyen
            gk->setTenGoi(InputHelper::nhapChuoiTuyCon("  Ten goi kenh", gk->getTenGoi()));
            gk->setMoTa(InputHelper::nhapChuoiTuyCon("  Mo ta", gk->getMoTa()));

            // Loai goi
            cout << "  Loai goi hien tai: " << gk->getLoaiGoi() << "\n";
            cout << "  Loai moi (Co ban/Nang cao/Premium/VOD, Enter de giu): ";
            string loaiMoi;
            getline(cin, loaiMoi);
            if (!loaiMoi.empty()) gk->setLoaiGoi(loaiMoi);

            gk->setGiaThue(InputHelper::nhapDoubleTuyCon("  Gia thue (VND/thang)", gk->getGiaThue(), 0.01));
            gk->setSoKenh(InputHelper::nhapIntTuyCon("  So luong kenh", gk->getSoKenh(), 0, 9999));

            // Trang thai
            cout << "  Trang thai hien tai: " << gk->getTrangThai() << "\n";
            cout << "  Trang thai moi:\n"
                 << "    1. Dang cung cap\n"
                 << "    2. Ngung cung cap\n"
                 << "    0. Giu nguyen\n";
            int tt = InputHelper::nhapInt("  Chon (0-2): ", 0, 2);
            if (tt == 1) gk->setTrangThai("Dang cung cap");
            else if (tt == 2) gk->setTrangThai("Ngung cung cap");

            repo.update(*gk);
            Display::inThongBaoOK("Cap nhat goi kenh '" + ma + "' thanh cong!");
        } catch (const InvalidInputException& e) {
            Display::inThongBaoLoi(e.what());
        } catch (const FileException& e) {
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 6. XOA -------
    void xoaGoiKenh(Repository<GoiKenh>& goiKenhRepo,
                    function<bool(const string&)> ktraKhachHangDungGoi) {
        Display::inTieuDe("XOA GOI KENH", 60);
        string ma = InputHelper::nhapChuoiBatBuoc("  Nhap ma goi kenh can xoa: ");
        GoiKenh* gk = goiKenhRepo.findById(ma);
        if (!gk) {
            Display::inThongBaoLoi("Khong tim thay goi kenh voi ma '" + ma + "'!");
            Display::nhanPhimDeTiep();
            return;
        }
        gk->hienThi();

        // Kiem tra rang buoc: co khach hang nao dang dung goi nay khong?
        if (ktraKhachHangDungGoi(ma)) {
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
            goiKenhRepo.remove(ma);
            Display::inThongBaoOK("Xoa goi kenh '" + ma + "' thanh cong!");
        } catch (const exception& e) {
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

public:
    GoiKenhMenu(Repository<GoiKenh>& r) : repo(r) {}

    // Ham hien thi menu va xu ly
    void hienThiMenu(function<bool(const string&)> ktraKhachHangDungGoi) {
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
                case 6: xoaGoiKenh(repo, ktraKhachHangDungGoi); break;
                case 0: break;
            }
        } while (luaChon != 0);
    }
};

#endif // GOI_KENH_MENU_H
