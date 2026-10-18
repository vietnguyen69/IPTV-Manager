/*
 * File   : KenhTruyenHinhMenu.cpp
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 */
#include "KenhTruyenHinhMenu.h"
#include <algorithm>
#include <iostream>
#include "InputHelper.h"
#include "Validator.h"

using namespace std;

void KenhTruyenHinhMenu::chay() {
    while (true) {
        cout << "\n====== QUAN LY KENH TRUYEN HINH ======\n"
             << " 1. Them kenh moi\n"
             << " 2. Xem danh sach kenh\n"
             << " 3. Tim kiem kenh\n"
             << " 4. Sap xep danh sach\n"
             << " 5. Cap nhat thong tin kenh\n"
             << " 6. Xoa kenh\n"
             << " 0. Quay lai menu chinh\n"
             << "======================================\n";
        int choice = InputHelper::readInt(" Nhap lua chon", 0, 6);
        if (choice == 0) return;
        try {
            switch (choice) {
                case 1: themKenh(); break;
                case 2: xemDanhSach(); break;
                case 3: timKiem(); break;
                case 4: sapXep(); break;
                case 5: capNhat(); break;
                case 6: xoa(); break;
            }
        } catch (const InputEndedException&) {
            throw;
        } catch (const exception& e) {  // gom ca loi ghi file (runtime_error) cua Repository
            cout << "  [LOI] " << e.what() << "\n";
        }
    }
}

void KenhTruyenHinhMenu::inBang(const vector<KenhTruyenHinh>& rows) {
    if (rows.empty()) {
        cout << "  (Khong co kenh nao)\n";
        return;
    }
    rows.front().displayTableHeader();
    int stt = 0;
    for (const KenhTruyenHinh& c : rows) c.displayTableRow(++stt);
    cout << "Tong so kenh: " << rows.size() << "\n";
}

void KenhTruyenHinhMenu::themKenh() {
    cout << "\n--- Them kenh moi ---\n";
    KenhTruyenHinh ch;
    // Rang buoc lien doi tuong: ma kenh khong duoc trung
    ch.datKiemTraMa([this](const string& id) {
        if (dsKenh_.findById(id)) throw DuplicateException("Ma kenh " + id + " da ton tai");
    });
    ch.inputData();
    ch.xoaKiemTra();
    if (!dsKenh_.add(ch)) throw DuplicateException("Ma kenh " + ch.getId() + " da ton tai");
    cout << "  Da them kenh " << ch.getId() << " va luu vao file.\n";
}

void KenhTruyenHinhMenu::xemDanhSach() {
    cout << "\n--- Danh sach kenh truyen hinh ---\n";
    inBang(dsKenh_.getAll());
    if (dsKenh_.isEmpty()) return;

    string id = Validator::toUpper(Validator::trim(
        InputHelper::readLine("Nhap ma kenh de xem chi tiet (Enter de bo qua): ")));
    if (id.empty()) return;
    KenhTruyenHinh* c = dsKenh_.findById(id);
    if (!c) throw NotFoundException("Khong tim thay kenh " + id);
    const Entity& e = *c;  // goi qua tham chieu lop co so (da hinh)
    e.display();
    int n = 0;
    for (const auto& s : dsLich_.getAll())
        if (s.layMaKenh() == id) ++n;
    cout << "  So lich phat song : " << n << "\n";
}

void KenhTruyenHinhMenu::timKiem() {
    cout << "\n--- Tim kiem kenh ---\n"
         << " 1. Theo ma (chinh xac)\n"
         << " 2. Theo tu khoa trong ma hoac ten (gan dung, khong phan biet hoa thuong)\n"
         << " 3. Theo do phan giai\n"
         << " 0. Huy\n";
    int ch = InputHelper::readInt(" Chon", 0, 3);
    if (ch == 0) return;
    string key = Validator::trim(InputHelper::readLine("Nhap tu khoa: "));
    if (key.empty()) throw ValidationException("Tu khoa khong duoc de trong");

    vector<KenhTruyenHinh> rows;
    if (ch == 1) {
        if (KenhTruyenHinh* c = dsKenh_.findById(Validator::toUpper(key))) rows.push_back(*c);
    } else if (ch == 2) {
        rows = dsKenh_.search(key);  // Repository<T>::search: theo ma hoac ten
    } else {
        string r = Validator::toUpper(key);
        const auto& all = dsKenh_.getAll();
        copy_if(all.begin(), all.end(), back_inserter(rows),
                [&](const KenhTruyenHinh& c) { return c.layDoPhanGiai() == r; });
    }
    inBang(rows);
}

void KenhTruyenHinhMenu::sapXep() {
    cout << "\n--- Sap xep danh sach kenh ---\n"
         << " 1. Theo ten A-Z\n"
         << " 2. Theo so kenh tang dan\n"
         << " 3. Theo so kenh giam dan\n"
         << " 0. Huy\n";
    int ch = InputHelper::readInt(" Chon", 0, 3);
    if (ch == 0) return;
    vector<KenhTruyenHinh> rows = dsKenh_.getAll();  // sap xep tren ban sao, khong doi thu tu trong file
    if (ch == 1)
        sort(rows.begin(), rows.end(), [](const KenhTruyenHinh& a, const KenhTruyenHinh& b) {
            return Validator::toLower(a.layTen()) < Validator::toLower(b.layTen());
        });
    else if (ch == 2)
        sort(rows.begin(), rows.end(), [](const KenhTruyenHinh& a, const KenhTruyenHinh& b) { return a.laySoKenh() < b.laySoKenh(); });
    else
        sort(rows.begin(), rows.end(), [](const KenhTruyenHinh& a, const KenhTruyenHinh& b) { return a.laySoKenh() > b.laySoKenh(); });
    inBang(rows);
}

void KenhTruyenHinhMenu::capNhat() {
    cout << "\n--- Cap nhat kenh ---\n";
    string id = Validator::toUpper(Validator::trim(InputHelper::readLine("Nhap ma kenh can sua: ")));
    KenhTruyenHinh* found = dsKenh_.findById(id);
    if (!found) throw NotFoundException("Khong tim thay kenh " + id);
    cout << "Thong tin hien tai:\n";
    found->display();

    KenhTruyenHinh copy = *found;  // sua tren ban sao; ma kenh khong duoc sua
    copy.updateData();
    *found = copy;
    dsKenh_.saveAfterUpdate();
    cout << "  Da cap nhat kenh " << id << " va luu vao file.\n";
}

void KenhTruyenHinhMenu::xoa() {
    cout << "\n--- Xoa kenh ---\n";
    string id = Validator::toUpper(Validator::trim(InputHelper::readLine("Nhap ma kenh can xoa: ")));
    KenhTruyenHinh* c = dsKenh_.findById(id);
    if (!c) throw NotFoundException("Khong tim thay kenh " + id);
    c->display();

    // Rang buoc tham chieu: khong xoa kenh con lich phat song
    int refs = 0;
    for (const auto& s : dsLich_.getAll())
        if (s.layMaKenh() == id) ++refs;
    if (refs > 0)
        throw ValidationException("Khong the xoa: kenh " + id + " con " + to_string(refs) +
                                  " lich phat song. Hay xoa cac lich do truoc.");
    if (!InputHelper::confirm("Ban co chac chan muon xoa kenh nay?")) {
        cout << "  Da huy thao tac xoa.\n";
        return;
    }
    dsKenh_.remove(id);
    cout << "  Da xoa kenh " << id << " va cap nhat file.\n";
}
