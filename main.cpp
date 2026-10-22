/*
 * =====================================================================================
 * Kien truc:
 *  - Entity.h: Interface lop co so thuan ao cho tat ca cac doi tuong (10 entities)
 *  - Repository.h: Lop mau Repository<T> quan ly danh sach va doc/ghi file
 *  - CommonUI.h & Utils.h: Tien ich giao dien console, nhap lieu va bang bieu
 *  - 10 Use Cases (Model + Menu):
 *      1. TheLoaiKenh       & TheLoaiKenhMenu
 *      2. NoiDungVOD        & NoiDungVODMenu
 *      3. KenhTruyenHinh    & KenhTruyenHinhMenu
 *      4. LichPhatSong      & LichPhatSongMenu
 *      5. GoiKenh           & GoiKenhMenu
 *      6. KhachHang         & KhachHangMenu
 *      7. SetTopBox         & SetTopBoxMenu
 *      8. PhieuHoTro        & PhieuHoTroMenu
 *      9. HopDong           & HopDongMenu
 *     10. HoaDon            & HoaDonMenu
 * =====================================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cstdlib>

#include "Entity.h"
#include "Repository.h"
#include "CommonUI.h"
#include "Utils.h"

#include "TheLoaiKenh.h"
#include "NoiDungVOD.h"
#include "KenhTruyenHinh.h"
#include "LichPhatSong.h"
#include "GoiKenh.h"
#include "KhachHang.h"
#include "SetTopBox.h"
#include "PhieuHoTro.h"
#include "HopDong.h"
#include "HoaDon.h"

// 10 Menus
#include "TheLoaiKenhMenu.h"
#include "NoiDungVODMenu.h"
#include "KenhTruyenHinhMenu.h"
#include "LichPhatSongMenu.h"
#include "GoiKenhMenu.h"
#include "KhachHangMenu.h"
#include "SetTopBoxMenu.h"
#include "PhieuHoTroMenu.h"
#include "HopDongMenu.h"
#include "HoaDonMenu.h"

using namespace std;

void inMenuChinh() {
    clearScreen();
    cout << "\n  ====================================================================\n";
    cout << "             HE THONG QUAN LY DICH VU TRUYEN HINH SO / IPTV           \n";
    cout << "  ====================================================================\n";
    cout << "    1.  Quan ly The loai kenh\n";
    cout << "    2.  Quan ly Noi dung VOD (Video On Demand)\n";
    cout << "    3.  Quan ly Kenh truyen hinh\n";
    cout << "    4.  Quan ly Lich phat song\n";
    cout << "    5.  Quan ly Goi kenh truyen hinh\n";
    cout << "    6.  Quan ly Khach hang\n";
    cout << "    7.  Quan ly Thiet bi Set-Top-Box (STB)\n";
    cout << "    8.  Quan ly Phieu ho tro ky thuat\n";
    cout << "    9.  Quan ly Hop dong IPTV\n";
    cout << "    10. Quan ly Hoa don va Thanh toan\n";
    cout << "    0.  Thoat chuong trinh\n";
    cout << "  --------------------------------------------------------------------\n";
}

int main() {
    // 1. Khoi tao 10 Repository gan voi cac file du lieu trong thu muc data/
    Repository<TheLoaiKenh>    repoTheLoai("data/theloaikenh.txt");
    Repository<NoiDungVOD>     repoVOD("data/noidungvod.txt");
    Repository<KenhTruyenHinh> repoKenh("data/kenhtruyenhinh.txt");
    Repository<LichPhatSong>   repoLichPS("data/lichphatsong.txt");
    Repository<GoiKenh>        repoGoiKenh("data/goi_kenh.txt");
    Repository<KhachHang>      repoKhachHang("data/khach_hang.txt");
    Repository<SetTopBox>      repoSTB("data/settopbox.txt");
    Repository<PhieuHoTro>     repoPhieuHoTro("data/phieuhotro.txt");
    Repository<HopDong>        repoHopDong("data/hopdong.txt");
    Repository<HoaDon>         repoHoaDon("data/hoadon.txt");

    // 2. Thiet lap cac ham callback kiem tra rang buoc khoa ngoai
    auto ktraGoi = [&repoKhachHang](const string& maGoi) -> bool {
        const auto& dsKH = repoKhachHang.getAll();
        for (const auto& kh : dsKH) {
            if (kh.getMaGoiKenh() == maGoi) return true;
        }
        return false;
    };

    auto khTonTai = [&repoKhachHang](const string& maKH) -> bool {
        if (maKH.empty()) return true;
        return repoKhachHang.findById(maKH) != nullptr;
    };

    auto coPhieuHoTro = [&repoPhieuHoTro](const string& maSTB) -> bool {
        const auto& dsPhieu = repoPhieuHoTro.getAll();
        for (const auto& p : dsPhieu) {
            if (p.getMaSTB() == maSTB) return true;
        }
        return false;
    };

    auto stbTonTai = [&repoSTB](const string& maSTB) -> bool {
        if (maSTB.empty()) return false;
        return repoSTB.findById(maSTB) != nullptr;
    };

    // 3. Khoi tao 10 Menu usecase tuong ung
    TheLoaiKenhMenu    menuTheLoai(repoTheLoai, repoVOD);
    NoiDungVODMenu     menuVOD(repoVOD, repoTheLoai);
    KenhTruyenHinhMenu menuKenh(repoKenh, repoTheLoai, repoLichPS);
    LichPhatSongMenu   menuLichPS(repoLichPS, repoKenh);
    GoiKenhMenu        menuGoiKenh(repoGoiKenh);
    KhachHangMenu      menuKhachHang(repoKhachHang, repoGoiKenh);
    SetTopBoxMenu      menuSTB(repoSTB, khTonTai, coPhieuHoTro);
    PhieuHoTroMenu     menuPhieuHoTro(repoPhieuHoTro, stbTonTai);
    HopDongMenu        menuHopDong(repoHopDong, repoHoaDon, khTonTai);
    HoaDonMenu         menuHoaDon(repoHoaDon, repoHopDong);

    // 4. Vong lap dieu huong menu chinh
    int luaChon = -1;
    do {
        inMenuChinh();
        cout << "  Nhap lua chon cua ban (0-10): ";
        string input;
        getline(cin, input);
        try {
            luaChon = stoi(input);
        } catch (...) {
            luaChon = -1;
        }

        switch (luaChon) {
            case 1:
                menuTheLoai.chay();
                break;
            case 2:
                menuVOD.chay();
                break;
            case 3:
                menuKenh.chay();
                break;
            case 4:
                menuLichPS.chay();
                break;
            case 5:
                menuGoiKenh.chay(ktraGoi);
                break;
            case 6:
                menuKhachHang.chay();
                break;
            case 7:
                menuSTB.chay();
                break;
            case 8:
                menuPhieuHoTro.chay();
                break;
            case 9:
                menuHopDong.chay();
                break;
            case 10:
                menuHoaDon.chay();
                break;
            case 0:
                cout << "\n  ======================================================\n";
                cout << "    Cam on ban da su dung he thong IPTV Manager!\n";
                cout << "    Chuong trinh ket thuc. Tam biet!\n";
                cout << "  ======================================================\n\n";
                break;
            default:
                cout << "\n  [LOI] Lua chon khong hop le! Vui long nhap so tu 0 den 10.\n";
                pauseScreen();
                break;
        }
    } while (luaChon != 0);

    return 0;
}
