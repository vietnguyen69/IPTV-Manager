// Cac ham CRUD cho Use Case Quan ly Hop dong va Quan ly Hoa don.
// Include file nay trong main.cpp SAU khi khai bao repoHopDong, repoHoaDon,
// printTitle(), pauseScreen(), displayTable() va printLine().
#ifndef QUAN_LY_HOP_DONG_HOA_DON_H
#define QUAN_LY_HOP_DONG_HOA_DON_H

#include "HopDong.h"
#include "HoaDon.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

// ======================== QUAN LY HOP DONG ========================
inline void themHopDong() {
    printTitle("THEM HOP DONG MOI");
    try {
        HopDong hd;
        hd.inputData();
        if (repoHopDong.add(hd)) cout << "\n  >> Them hop dong thanh cong!" << endl;
        else cout << "\n  [LOI] Ma hop dong '" << hd.getId() << "' da ton tai!" << endl;
    } catch (const exception& e) {
        cout << "\n  [LOI] " << e.what() << endl;
    }
}

inline void hienThiHopDong() {
    printTitle("DANH SACH HOP DONG");
    displayTable(repoHopDong.getAll());
}

inline void timKiemHopDong() {
    printTitle("TIM KIEM HOP DONG");
    string keyword;
    cout << "\n  Nhap tu khoa (ma hop dong, ma khach hang hoac goi dich vu): ";
    getline(cin, keyword);
    if (keyword.empty()) { cout << "\n  [LOI] Tu khoa khong duoc de trong!" << endl; return; }
    displayTable(repoHopDong.search(keyword));
}

inline void capNhatHopDong() {
    printTitle("CAP NHAT HOP DONG");
    string id;
    cout << "\n  Nhap ma hop dong can cap nhat: ";
    getline(cin, id);
    HopDong* found = repoHopDong.findById(id);
    if (!found) { cout << "\n  [LOI] Khong tim thay hop dong '" << id << "'!" << endl; return; }
    found->display();
    HopDong backup = *found;
    try {
        found->updateData();
        repoHopDong.saveAfterUpdate();
        cout << "\n  >> Cap nhat hop dong thanh cong!" << endl;
    } catch (const exception& e) {
        *found = backup;
        cout << "\n  [LOI] " << e.what() << "\n  Thay doi da duoc huy." << endl;
    }
}

inline void xoaHopDong() {
    printTitle("XOA HOP DONG");
    string id;
    cout << "\n  Nhap ma hop dong can xoa: ";
    getline(cin, id);
    HopDong* found = repoHopDong.findById(id);
    if (!found) { cout << "\n  [LOI] Khong tim thay hop dong '" << id << "'!" << endl; return; }
    found->display();

    // Rang buoc khoa ngoai: khong xoa hop dong dang duoc hoa don tham chieu.
    bool dangDuocDung = false;
    const vector<HoaDon>& dsHoaDon = repoHoaDon.getAll();
    for (size_t i = 0; i < dsHoaDon.size(); ++i) {
        if (dsHoaDon[i].getMaHopDong() == id) { dangDuocDung = true; break; }
    }
    if (dangDuocDung) {
        cout << "\n  [LOI] Khong the xoa: hop dong dang duoc hoa don su dung." << endl;
        cout << "  Hay xu ly cac hoa don lien quan truoc." << endl;
        return;
    }
    string confirm;
    cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
    getline(cin, confirm);
    if (confirm == "Y" || confirm == "y") {
        if (repoHopDong.remove(id)) cout << "\n  >> Xoa hop dong thanh cong!" << endl;
        else cout << "\n  [LOI] Xoa that bai!" << endl;
    } else cout << "\n  >> Da huy thao tac xoa." << endl;
}

inline void menuHopDong() {
    int choice = -1;
    do {
        clearScreen();
        printTitle("QUAN LY HOP DONG");
        cout << "\n  1. Them hop dong" << endl;
        cout << "  2. Hien thi danh sach" << endl;
        cout << "  3. Tim kiem" << endl;
        cout << "  4. Cap nhat thong tin" << endl;
        cout << "  5. Xoa hop dong" << endl;
        cout << "  0. Quay lai menu chinh" << endl;
        printLine();
        cout << "  Lua chon cua ban: ";
        string input; getline(cin, input);
        try { choice = stoi(input); }
        catch (...) { cout << "\n  [LOI] Vui long nhap mot so!" << endl; pauseScreen(); continue; }
        switch (choice) {
            case 1: themHopDong(); pauseScreen(); break;
            case 2: hienThiHopDong(); pauseScreen(); break;
            case 3: timKiemHopDong(); pauseScreen(); break;
            case 4: capNhatHopDong(); pauseScreen(); break;
            case 5: xoaHopDong(); pauseScreen(); break;
            case 0: break;
            default: cout << "\n  [LOI] Lua chon khong hop le (0-5)!" << endl; pauseScreen();
        }
    } while (choice != 0);
}

// ======================== QUAN LY HOA DON ========================
inline void themHoaDon() {
    printTitle("THEM HOA DON MOI");
    if (repoHopDong.isEmpty()) {
        cout << "\n  [CANH BAO] Chua co hop dong nao. Hay them hop dong truoc!" << endl;
        return;
    }
    cout << "\n  --- Danh sach hop dong hien co ---" << endl;
    displayTable(repoHopDong.getAll());
    try {
        HoaDon hd;
        hd.inputData();
        if (repoHopDong.findById(hd.getMaHopDong()) == NULL) {
            throw invalid_argument("Ma hop dong '" + hd.getMaHopDong() + "' khong ton tai!");
        }
        if (repoHoaDon.add(hd)) cout << "\n  >> Them hoa don thanh cong!" << endl;
        else cout << "\n  [LOI] Ma hoa don '" << hd.getId() << "' da ton tai!" << endl;
    } catch (const exception& e) {
        cout << "\n  [LOI] " << e.what() << endl;
    }
}

inline void hienThiHoaDon() {
    printTitle("DANH SACH HOA DON");
    displayTable(repoHoaDon.getAll());
}

inline void timKiemHoaDon() {
    printTitle("TIM KIEM HOA DON");
    string keyword;
    cout << "\n  Nhap tu khoa (ma hoa don, ma hop dong hoac ky cuoc): ";
    getline(cin, keyword);
    if (keyword.empty()) { cout << "\n  [LOI] Tu khoa khong duoc de trong!" << endl; return; }
    displayTable(repoHoaDon.search(keyword));
}

inline void capNhatHoaDon() {
    printTitle("CAP NHAT HOA DON");
    string id;
    cout << "\n  Nhap ma hoa don can cap nhat: ";
    getline(cin, id);
    HoaDon* found = repoHoaDon.findById(id);
    if (!found) { cout << "\n  [LOI] Khong tim thay hoa don '" << id << "'!" << endl; return; }
    found->display();
    cout << "\n  --- Danh sach hop dong hien co ---" << endl;
    displayTable(repoHopDong.getAll());
    HoaDon backup = *found;
    try {
        found->updateData();
        if (repoHopDong.findById(found->getMaHopDong()) == NULL)
            throw invalid_argument("Ma hop dong '" + found->getMaHopDong() + "' khong ton tai!");
        repoHoaDon.saveAfterUpdate();
        cout << "\n  >> Cap nhat hoa don thanh cong!" << endl;
    } catch (const exception& e) {
        *found = backup;
        cout << "\n  [LOI] " << e.what() << "\n  Thay doi da duoc huy." << endl;
    }
}

inline void xoaHoaDon() {
    printTitle("XOA HOA DON");
    string id;
    cout << "\n  Nhap ma hoa don can xoa: ";
    getline(cin, id);
    HoaDon* found = repoHoaDon.findById(id);
    if (!found) { cout << "\n  [LOI] Khong tim thay hoa don '" << id << "'!" << endl; return; }
    found->display();
    string confirm;
    cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
    getline(cin, confirm);
    if (confirm == "Y" || confirm == "y") {
        if (repoHoaDon.remove(id)) cout << "\n  >> Xoa hoa don thanh cong!" << endl;
        else cout << "\n  [LOI] Xoa that bai!" << endl;
    } else cout << "\n  >> Da huy thao tac xoa." << endl;
}

inline void menuHoaDon() {
    int choice = -1;
    do {
        clearScreen();
        printTitle("QUAN LY HOA DON");
        cout << "\n  1. Them hoa don" << endl;
        cout << "  2. Hien thi danh sach" << endl;
        cout << "  3. Tim kiem" << endl;
        cout << "  4. Cap nhat thong tin" << endl;
        cout << "  5. Xoa hoa don" << endl;
        cout << "  0. Quay lai menu chinh" << endl;
        printLine();
        cout << "  Lua chon cua ban: ";
        string input; getline(cin, input);
        try { choice = stoi(input); }
        catch (...) { cout << "\n  [LOI] Vui long nhap mot so!" << endl; pauseScreen(); continue; }
        switch (choice) {
            case 1: themHoaDon(); pauseScreen(); break;
            case 2: hienThiHoaDon(); pauseScreen(); break;
            case 3: timKiemHoaDon(); pauseScreen(); break;
            case 4: capNhatHoaDon(); pauseScreen(); break;
            case 5: xoaHoaDon(); pauseScreen(); break;
            case 0: break;
            default: cout << "\n  [LOI] Lua chon khong hop le (0-5)!" << endl; pauseScreen();
        }
    } while (choice != 0);
}

#endif