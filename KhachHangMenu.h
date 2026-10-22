/*
 * KhachHangMenu.h
 * Menu quan ly Khach Hang - Giao dien console cho use case CRUD Khach Hang
 * De tai 7: Quan ly dich vu truyen hinh so/IPTV
 */
#ifndef KHACH_HANG_MENU_H
#define KHACH_HANG_MENU_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <functional>
#include "KhachHang.h"
#include "GoiKenh.h"
#include "Repository.h"
#include "Utils.h"

using namespace std;

class KhachHangMenu {
private:
    Repository<KhachHang>& repoKH;
    Repository<GoiKenh>&   repoGK;  // Tham chieu de kiem tra goi kenh ton tai

    // ------- 1. THEM MOI -------
    void themKhachHang() {
        Display::inTieuDe("THEM KHACH HANG MOI", 65);
        try {
            KhachHang kh;
            kh.inputData();

            // Kiem tra goi kenh co ton tai khong (neu da nhap)
            if (!kh.getMaGoiKenh().empty() && repoGK.findById(kh.getMaGoiKenh()) == nullptr) {
                Display::inThongBaoLoi("Ma goi kenh '" + kh.getMaGoiKenh() + "' khong ton tai trong he thong!");
                if (!InputHelper::xacNhan("  Tiep tuc them khach hang voi ma goi nay khong?")) {
                    cout << "  Da huy them khach hang.\n";
                    Display::nhanPhimDeTiep();
                    return;
                }
            }

            if (repoKH.add(kh)) {
                Display::inThongBaoOK("Them khach hang '" + kh.getId() + "' thanh cong!");
            } else {
                Display::inThongBaoLoi("Ma khach hang '" + kh.getId() + "' da ton tai!");
            }
        } catch (const exception& e) {
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 2. XEM DANH SACH -------
    void xemDanhSach() {
        Display::inTieuDe("DANH SACH KHACH HANG IPTV", 70);
        const auto& ds = repoKH.getAll();
        if (ds.empty()) {
            cout << "  (Chua co khach hang nao trong he thong)\n";
            Display::nhanPhimDeTiep();
            return;
        }

        Display::inBang(ds);

        // Thong ke nhanh
        int hoatDong = 0, tamNgung = 0, daHuy = 0;
        for (const auto& kh : ds) {
            if (kh.getTrangThai() == "Hoat dong") hoatDong++;
            else if (kh.getTrangThai() == "Tam ngung") tamNgung++;
            else daHuy++;
        }
        cout << "\n  Tong so: " << ds.size() << " khach hang"
             << " | Hoat dong: " << hoatDong
             << " | Tam ngung: " << tamNgung
             << " | Da huy: " << daHuy << "\n";
        Display::nhanPhimDeTiep();
    }

    // ------- 3. XEM CHI TIET -------
    void xemChiTiet() {
        Display::inTieuDe("XEM CHI TIET KHACH HANG", 65);
        string ma = InputHelper::nhapChuoiBatBuoc("  Nhap ma khach hang: ");
        KhachHang* kh = repoKH.findById(ma);
        if (!kh) {
            Display::inThongBaoLoi("Khong tim thay khach hang voi ma '" + ma + "'!");
        } else {
            kh->display();
            // Hien thi them thong tin goi kenh neu co
            if (!kh->getMaGoiKenh().empty()) {
                GoiKenh* gk = repoGK.findById(kh->getMaGoiKenh());
                if (gk) {
                    cout << "\n  --- THONG TIN GOI KENH DANG DUNG ---\n";
                    cout << "  Ten goi  : " << gk->getTenGoi() << "\n";
                    cout << "  Loai goi : " << gk->getLoaiGoi() << "\n";
                    cout << "  Gia thue : " << fixed << setprecision(0) << gk->getGiaThue() << " VND/thang\n";
                    cout << "  So kenh  : " << gk->getSoKenh() << " kenh\n";
                }
            }
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 4. TIM KIEM -------
    void timKiem() {
        Display::inTieuDe("TIM KIEM KHACH HANG", 65);
        cout << "  1. Tim theo ma khach hang\n";
        cout << "  2. Tim theo ho ten (gan dung)\n";
        cout << "  3. Tim theo so dien thoai\n";
        cout << "  4. Tim theo ma goi kenh\n";
        cout << "  5. Tim theo trang thai\n";
        cout << "  0. Quay lai\n";
        int ch = InputHelper::nhapInt("  Lua chon: ", 0, 5);
        if (ch == 0) return;

        if (ch == 1) {
            string ma = InputHelper::nhapChuoiBatBuoc("  Ma khach hang: ");
            KhachHang* kh = repoKH.findById(ma);
            if (kh) kh->display();
            else Display::inThongBaoLoi("Khong tim thay!");
        }
        else if (ch == 2) {
            string ten = InputHelper::nhapChuoiBatBuoc("  Nhap tu khoa ho ten: ");
            string tenL = StrUtils::toLower(ten);
            vector<KhachHang> kq;
            for (const auto& kh : repoKH.getAll()) {
                if (StrUtils::toLower(kh.getHoTen()).find(tenL) != string::npos) {
                    kq.push_back(kh);
                }
            }
            if (kq.empty()) cout << "  Khong tim thay ket qua.\n";
            else {
                cout << "  Tim thay " << kq.size() << " ket qua:\n";
                Display::inBang(kq);
            }
        }
        else if (ch == 3) {
            string sdt = InputHelper::nhapChuoiBatBuoc("  So dien thoai: ");
            vector<KhachHang> kq;
            for (const auto& kh : repoKH.getAll()) {
                if (kh.getSoDienThoai() == sdt) {
                    kq.push_back(kh);
                }
            }
            if (kq.empty()) cout << "  Khong tim thay.\n";
            else {
                for (const auto& kh : kq) kh.display();
            }
        }
        else if (ch == 4) {
            string maGoi = InputHelper::nhapChuoiBatBuoc("  Ma goi kenh: ");
            vector<KhachHang> kq;
            for (const auto& kh : repoKH.getAll()) {
                if (kh.getMaGoiKenh() == maGoi) {
                    kq.push_back(kh);
                }
            }
            if (kq.empty()) cout << "  Khong co khach hang nao dung goi '" << maGoi << "'.\n";
            else {
                cout << "  " << kq.size() << " khach hang dang dung goi '" << maGoi << "':\n";
                Display::inBang(kq);
            }
        }
        else if (ch == 5) {
            cout << "  Trang thai (Hoat dong / Tam ngung / Da huy): ";
            string tt;
            getline(cin, tt);
            tt = StrUtils::trim(tt);
            vector<KhachHang> kq;
            for (const auto& kh : repoKH.getAll()) {
                if (kh.getTrangThai() == tt) {
                    kq.push_back(kh);
                }
            }
            if (kq.empty()) cout << "  Khong co khach hang voi trang thai '" << tt << "'.\n";
            else {
                cout << "  Tim thay " << kq.size() << " ket qua:\n";
                Display::inBang(kq);
            }
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 5. SAP XEP -------
    void sapXep() {
        Display::inTieuDe("SAP XEP DANH SACH KHACH HANG", 65);
        cout << "  1. Sap xep theo ho ten A-Z\n";
        cout << "  2. Sap xep theo ma khach hang\n";
        cout << "  3. Sap xep theo ngay dang ky\n";
        cout << "  0. Quay lai\n";
        int ch = InputHelper::nhapInt("  Lua chon: ", 0, 3);
        if (ch == 0) return;

        vector<KhachHang> ds = repoKH.getAll();
        if (ds.empty()) {
            cout << "  (Chua co khach hang nao trong he thong)\n";
            Display::nhanPhimDeTiep();
            return;
        }

        if (ch == 1) {
            sort(ds.begin(), ds.end(), [](const KhachHang& a, const KhachHang& b) {
                return StrUtils::toLower(a.getHoTen()) < StrUtils::toLower(b.getHoTen());
            });
            cout << "  Da sap xep theo ho ten A-Z.\n";
        } else if (ch == 2) {
            sort(ds.begin(), ds.end(), [](const KhachHang& a, const KhachHang& b) {
                return a.getId() < b.getId();
            });
            cout << "  Da sap xep theo ma khach hang.\n";
        } else if (ch == 3) {
            sort(ds.begin(), ds.end(), [](const KhachHang& a, const KhachHang& b) {
                return a.getNgayDangKy() < b.getNgayDangKy();
            });
            cout << "  Da sap xep theo ngay dang ky.\n";
        }

        Display::inBang(ds);
        Display::nhanPhimDeTiep();
    }

    // ------- 6. CAP NHAT -------
    void capNhat() {
        Display::inTieuDe("CAP NHAT THONG TIN KHACH HANG", 65);
        string ma = InputHelper::nhapChuoiBatBuoc("  Nhap ma khach hang can sua: ");
        KhachHang* kh = repoKH.findById(ma);
        if (!kh) {
            Display::inThongBaoLoi("Khong tim thay khach hang voi ma '" + ma + "'!");
            Display::nhanPhimDeTiep();
            return;
        }

        kh->display();
        KhachHang backup = *kh;

        try {
            kh->updateData();

            // Kiem tra goi kenh moi co ton tai khong
            if (!kh->getMaGoiKenh().empty() && repoGK.findById(kh->getMaGoiKenh()) == nullptr) {
                *kh = backup;
                throw invalid_argument("Ma goi kenh '" + kh->getMaGoiKenh() + "' khong ton tai!");
            }

            repoKH.saveAfterUpdate();
            Display::inThongBaoOK("Cap nhat khach hang '" + ma + "' thanh cong!");
        } catch (const exception& e) {
            *kh = backup;
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 7. XOA -------
    void xoaKhachHang() {
        Display::inTieuDe("XOA KHACH HANG", 65);
        string ma = InputHelper::nhapChuoiBatBuoc("  Nhap ma khach hang can xoa: ");
        KhachHang* kh = repoKH.findById(ma);
        if (!kh) {
            Display::inThongBaoLoi("Khong tim thay khach hang voi ma '" + ma + "'!");
            Display::nhanPhimDeTiep();
            return;
        }
        kh->display();

        // Rang buoc: khach hang dang Hoat dong thi canh bao
        if (kh->getTrangThai() == "Hoat dong") {
            Display::inThongBaoLoi("Canh bao: Khach hang nay dang HOAT DONG!");
            if (!InputHelper::xacNhan("  Ban co chac chan muon xoa?")) {
                cout << "  Da huy thao tac.\n";
                Display::nhanPhimDeTiep();
                return;
            }
        }

        if (!InputHelper::xacNhan("  Xac nhan xoa khach hang '" + ma + "'?")) {
            cout << "  Da huy thao tac xoa.\n";
            Display::nhanPhimDeTiep();
            return;
        }

        try {
            if (repoKH.remove(ma)) {
                Display::inThongBaoOK("Xoa khach hang '" + ma + "' thanh cong!");
            } else {
                Display::inThongBaoLoi("Xoa that bai!");
            }
        } catch (const exception& e) {
            Display::inThongBaoLoi(e.what());
        }
        Display::nhanPhimDeTiep();
    }

    // ------- 8. THONG KE -------
    void thongKe() {
        Display::inTieuDe("THONG KE KHACH HANG", 65);
        const auto& ds = repoKH.getAll();
        if (ds.empty()) {
            cout << "  Chua co du lieu.\n";
            Display::nhanPhimDeTiep();
            return;
        }

        // Thong ke theo trang thai
        int hoatDong = 0, tamNgung = 0, daHuy = 0;
        // Thong ke theo goi kenh
        map<string, int> dsByGoi;
        for (const auto& kh : ds) {
            if (kh.getTrangThai() == "Hoat dong") hoatDong++;
            else if (kh.getTrangThai() == "Tam ngung") tamNgung++;
            else daHuy++;
            if (!kh.getMaGoiKenh().empty())
                dsByGoi[kh.getMaGoiKenh()]++;
        }

        cout << "\n  === THONG KE TONG HOP ===\n";
        cout << "  Tong so khach hang    : " << ds.size() << "\n";
        cout << "  Khach hang hoat dong  : " << hoatDong << "\n";
        cout << "  Khach hang tam ngung  : " << tamNgung << "\n";
        cout << "  Khach hang da huy     : " << daHuy << "\n";

        if (!dsByGoi.empty()) {
            cout << "\n  === SO LUONG KH THEO GOI KENH ===\n";
            cout << "  " << left << setw(12) << "Ma goi" << setw(8) << "So KH" << "Ten goi\n";
            cout << "  " << string(40, '-') << "\n";
            for (const auto& entry : dsByGoi) {
                GoiKenh* gk = repoGK.findById(entry.first);
                string tenGoi = gk ? gk->getTenGoi() : "(Khong xac dinh)";
                cout << "  " << left << setw(12) << entry.first
                     << setw(8) << entry.second
                     << tenGoi << "\n";
            }
        }
        Display::nhanPhimDeTiep();
    }

public:
    KhachHangMenu(Repository<KhachHang>& rKH, Repository<GoiKenh>& rGK)
        : repoKH(rKH), repoGK(rGK) {}

    void chay() {
        hienThiMenu();
    }

    void hienThiMenu() {
        int luaChon;
        do {
            Display::inTieuDe("QUAN LY KHACH HANG IPTV", 65);
            cout << "  1. Them khach hang moi\n";
            cout << "  2. Xem danh sach khach hang\n";
            cout << "  3. Xem chi tiet khach hang\n";
            cout << "  4. Tim kiem khach hang\n";
            cout << "  5. Sap xep danh sach\n";
            cout << "  6. Cap nhat thong tin khach hang\n";
            cout << "  7. Xoa khach hang\n";
            cout << "  8. Thong ke khach hang\n";
            cout << "  0. Quay lai menu chinh\n";
            Display::inDuongKe(65);
            luaChon = InputHelper::nhapInt("  Nhap lua chon: ", 0, 8);

            switch (luaChon) {
                case 1: themKhachHang(); break;
                case 2: xemDanhSach(); break;
                case 3: xemChiTiet(); break;
                case 4: timKiem(); break;
                case 5: sapXep(); break;
                case 6: capNhat(); break;
                case 7: xoaKhachHang(); break;
                case 8: thongKe(); break;
                case 0: break;
            }
        } while (luaChon != 0);
    }
};

#endif // KHACH_HANG_MENU_H
