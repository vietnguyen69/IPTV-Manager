/*
 * Tac gia : Nguyen Ba Thanh Van - B24DCVT389
 * Giao dien console (menu) cua use case: Quan ly Set-top-box
 */
#ifndef SETTOPBOXMENU_H
#define SETTOPBOXMENU_H

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include "Repository.h"
#include "SetTopBox.h"
using namespace std;

// ===================== CRUD (menu console) =====================
// khTonTai(maKH): ma khach hang co ton tai khong
// coPhieu(maSTB): set-top-box co phieu ho tro tham chieu khong
class SetTopBoxMenu {
    Repository<SetTopBox>& repo;
    function<bool(const string&)> khTonTai, coPhieu;

    static string hoi(const string& c) { cout << c; string s; getline(cin, s); return s; }
    static void tieuDe(const string& t) { cout << "\n  ===== " << t << " =====\n"; }
    static void bang(const vector<SetTopBox>& v) {
        if (v.empty()) { cout << "\n  [Danh sach trong!]\n"; return; }
        v[0].displayTableHeader();
        for (size_t i = 0; i < v.size(); i++) v[i].displayTableRow((int)i + 1);
        cout << "  Tong cong: " << v.size() << " ban ghi.\n";
    }
    void them() {
        tieuDe("THEM SET-TOP-BOX");
        try {
            SetTopBox s; s.inputData();
            if (!s.getMaKH().empty() && !khTonTai(s.getMaKH()))
                throw invalid_argument("Ma khach hang '" + s.getMaKH() + "' khong ton tai");
            cout << (repo.add(s) ? "\n  >> Them thanh cong!\n" : "\n  [LOI] Ma STB da ton tai!\n");
        } catch (const exception& e) { cout << "\n  [LOI] " << e.what() << endl; }
    }
    void tim() {
        tieuDe("TIM KIEM SET-TOP-BOX");
        string kw = hoi("  Nhap tu khoa (ma / model / serial): ");
        if (kw.empty()) { cout << "  [LOI] Tu khoa khong duoc de trong!\n"; return; }
        bang(repo.search(kw));
    }
    void sua() {
        tieuDe("CAP NHAT SET-TOP-BOX");
        string id = hoi("  Nhap ma STB: ");
        SetTopBox* p = repo.findById(id);
        if (!p) { cout << "  [LOI] Khong tim thay!\n"; return; }
        p->display();
        SetTopBox backup = *p;
        try {
            p->updateData();
            if (!p->getMaKH().empty() && !khTonTai(p->getMaKH()))
                throw invalid_argument("Ma khach hang '" + p->getMaKH() + "' khong ton tai");
            repo.saveAfterUpdate();
            cout << "\n  >> Cap nhat thanh cong!\n";
        } catch (const exception& e) { *p = backup; cout << "\n  [LOI] " << e.what() << "\n  Thay doi da bi huy.\n"; }
    }
    void xoa() {
        tieuDe("XOA SET-TOP-BOX");
        string id = hoi("  Nhap ma STB can xoa: ");
        SetTopBox* p = repo.findById(id);
        if (!p) { cout << "  [LOI] Khong tim thay!\n"; return; }
        p->display();
        if (p->getTrangThai() == 2) { cout << "\n  [LOI] Khong the xoa: thiet bi dang duoc su dung!\n"; return; }
        if (coPhieu(id)) { cout << "\n  [LOI] Khong the xoa: con phieu ho tro tham chieu den thiet bi nay!\n"; return; }
        string c = hoi("\n  Chac chan xoa? (Y/N): ");
        if (c == "Y" || c == "y") { repo.remove(id); cout << "  >> Xoa thanh cong!\n"; }
        else cout << "  >> Da huy.\n";
    }

public:
    SetTopBoxMenu(Repository<SetTopBox>& r, function<bool(const string&)> kh, function<bool(const string&)> phieu)
        : repo(r), khTonTai(kh), coPhieu(phieu) {}

    void chay() { hienThiMenu(); }

    void hienThiMenu() {
        string in;
        do {
            cout << "\n  ===== QUAN LY SET-TOP-BOX =====\n  1. Them\n  2. Xem danh sach\n  3. Tim kiem\n"
                    "  4. Cap nhat\n  5. Xoa\n  0. Quay lai\n  Lua chon: ";
            if (!getline(cin, in)) return;
            if (in == "1") them();
            else if (in == "2") { tieuDe("DANH SACH SET-TOP-BOX"); bang(repo.getAll()); }
            else if (in == "3") tim();
            else if (in == "4") sua();
            else if (in == "5") xoa();
            else if (in != "0") cout << "  [LOI] Lua chon khong hop le (0-5)!\n";
        } while (in != "0");
    }
};

#endif
