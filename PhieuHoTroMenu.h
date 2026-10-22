/*
 * Tac gia : Nguyen Ba Thanh Van - B24DCVT389
 * Giao dien console (menu) cua use case: Quan ly Phieu ho tro ky thuat
 */
#ifndef PHIEUHOTROMENU_H
#define PHIEUHOTROMENU_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "Repository.h"
#include "PhieuHoTro.h"
using namespace std;

// ===================== CRUD (menu console) =====================
// stbTonTai(maSTB): set-top-box co ton tai khong
class PhieuHoTroMenu {
    Repository<PhieuHoTro>& repo;
    function<bool(const string&)> stbTonTai;

    static string hoi(const string& c) { cout << c; string s; getline(cin, s); return s; }
    static void tieuDe(const string& t) { cout << "\n  ===== " << t << " =====\n"; }
    static void bang(const vector<PhieuHoTro>& v) {
        if (v.empty()) { cout << "\n  [Danh sach trong!]\n"; return; }
        v[0].displayTableHeader();
        for (size_t i = 0; i < v.size(); i++) v[i].displayTableRow((int)i + 1);
        cout << "  Tong cong: " << v.size() << " ban ghi.\n";
    }

    void them() {
        tieuDe("THEM PHIEU HO TRO");
        try {
            PhieuHoTro p; p.inputData();
            if (!stbTonTai(p.getMaSTB()))
                throw invalid_argument("Set-top-box '" + p.getMaSTB() + "' khong ton tai");
            cout << (repo.add(p) ? "\n  >> Them thanh cong!\n" : "\n  [LOI] Ma phieu da ton tai!\n");
        } catch (const exception& e) { cout << "\n  [LOI] " << e.what() << endl; }
    }
    void tim() {
        tieuDe("TIM KIEM PHIEU HO TRO");
        string kw = hoi("  Nhap tu khoa (ma phieu / ma STB / mo ta): ");
        if (kw.empty()) { cout << "  [LOI] Tu khoa khong duoc de trong!\n"; return; }
        bang(repo.search(kw));
    }
    void sua() {
        tieuDe("CAP NHAT PHIEU HO TRO");
        string id = hoi("  Nhap ma phieu: ");
        PhieuHoTro* p = repo.findById(id);
        if (!p) { cout << "  [LOI] Khong tim thay!\n"; return; }
        p->display();
        PhieuHoTro backup = *p;
        try { p->updateData(); repo.saveAfterUpdate(); cout << "\n  >> Cap nhat thanh cong!\n"; }
        catch (const exception& e) { *p = backup; cout << "\n  [LOI] " << e.what() << "\n  Thay doi da bi huy.\n"; }
    }
    void xoa() {
        tieuDe("XOA PHIEU HO TRO");
        string id = hoi("  Nhap ma phieu can xoa: ");
        PhieuHoTro* p = repo.findById(id);
        if (!p) { cout << "  [LOI] Khong tim thay!\n"; return; }
        p->display();
        if (p->getTrangThai() != 3) { cout << "\n  [LOI] Khong the xoa: phieu chua hoan thanh!\n"; return; }
        string c = hoi("\n  Chac chan xoa? (Y/N): ");
        if (c == "Y" || c == "y") { repo.remove(id); cout << "  >> Xoa thanh cong!\n"; }
        else cout << "  >> Da huy.\n";
    }

public:
    PhieuHoTroMenu(Repository<PhieuHoTro>& r, function<bool(const string&)> stb) : repo(r), stbTonTai(stb) {}

    void chay() { hienThiMenu(); }

    void hienThiMenu() {
        string in;
        do {
            cout << "\n  ===== QUAN LY PHIEU HO TRO KY THUAT =====\n  1. Them\n  2. Xem danh sach\n  3. Tim kiem\n"
                    "  4. Cap nhat\n  5. Xoa\n  0. Quay lai\n  Lua chon: ";
            if (!getline(cin, in)) return;
            if (in == "1") them();
            else if (in == "2") { tieuDe("DANH SACH PHIEU HO TRO"); bang(repo.getAll()); }
            else if (in == "3") tim();
            else if (in == "4") sua();
            else if (in == "5") xoa();
            else if (in != "0") cout << "  [LOI] Lua chon khong hop le (0-5)!\n";
        } while (in != "0");
    }
};

#endif
