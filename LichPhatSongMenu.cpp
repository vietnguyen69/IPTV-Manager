/*
 * File   : LichPhatSongMenu.cpp
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 */
#include "LichPhatSongMenu.h"
#include <algorithm>
#include <iostream>
#include "InputHelper.h"
#include "Validator.h"

using namespace std;

void LichPhatSongMenu::chay() {
    while (true) {
        cout << "\n======== QUAN LY LICH PHAT SONG ========\n"
             << " 1. Them lich phat song\n"
             << " 2. Xem danh sach lich\n"
             << " 3. Tim kiem lich\n"
             << " 4. Sap xep danh sach\n"
             << " 5. Cap nhat lich phat song\n"
             << " 6. Xoa lich phat song\n"
             << " 0. Quay lai menu chinh\n"
             << "========================================\n";
        int choice = InputHelper::readInt(" Nhap lua chon", 0, 6);
        if (choice == 0) return;
        try {
            switch (choice) {
                case 1: themLich(); break;
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

string LichPhatSongMenu::tenKenh(const string& channelId) {
    KenhTruyenHinh* c = dsKenh_.findById(channelId);
    return c ? c->layTen() : "(khong ro)";
}

void LichPhatSongMenu::inBang(const vector<LichPhatSong>& rows) {
    if (rows.empty()) {
        cout << "  (Khong co lich nao)\n";
        return;
    }
    rows.front().displayTableHeader();
    int stt = 0;
    for (const LichPhatSong& s : rows) s.inDongCoTenKenh(++stt, tenKenh(s.layMaKenh()));
    cout << "Tong so lich: " << rows.size() << "\n";
}

void LichPhatSongMenu::ganRangBuoc(LichPhatSong& sc) {
    sc.datKiemTraMa([this](const string& id) {
        if (dsLich_.findById(id)) throw DuplicateException("Ma lich " + id + " da ton tai");
    });
    sc.datKiemTraKenh([this](const string& cid) {
        if (!dsKenh_.findById(cid)) throw ValidationException("Ma kenh " + cid + " khong ton tai");
    });
    sc.datKiemTraKhungGio([this](const LichPhatSong& probe) {
        for (const auto& other : dsLich_.getAll())
            if (other.getId() != probe.getId() && probe.trungKhungGio(other))
                throw ValidationException("Khung gio bi trung voi lich " + other.getId() +
                                          " cua cung kenh trong ngay do");
    });
}

void LichPhatSongMenu::themLich() {
    cout << "\n--- Them lich phat song ---\n";
    if (dsKenh_.isEmpty()) throw ValidationException("Chua co kenh nao. Hay them kenh truoc.");
    LichPhatSong sc;
    ganRangBuoc(sc);
    sc.inputData();
    sc.xoaKiemTra();
    if (!dsLich_.add(sc)) throw DuplicateException("Ma lich " + sc.getId() + " da ton tai");
    cout << "  Da them lich " << sc.getId() << " va luu vao file.\n";
}

void LichPhatSongMenu::xemDanhSach() {
    cout << "\n--- Danh sach lich phat song ---\n";
    inBang(dsLich_.getAll());
    if (dsLich_.isEmpty()) return;

    string id = Validator::toUpper(Validator::trim(
        InputHelper::readLine("Nhap ma lich de xem chi tiet (Enter de bo qua): ")));
    if (id.empty()) return;
    LichPhatSong* s = dsLich_.findById(id);
    if (!s) throw NotFoundException("Khong tim thay lich " + id);
    const Entity& e = *s;  // da hinh qua tham chieu lop co so
    e.display();
    cout << "  Ten kenh       : " << tenKenh(s->layMaKenh()) << "\n";
}

void LichPhatSongMenu::timKiem() {
    cout << "\n--- Tim kiem lich phat song ---\n"
         << " 1. Theo ma lich (chinh xac)\n"
         << " 2. Theo ma kenh\n"
         << " 3. Theo ngay phat\n"
         << " 4. Theo tu khoa trong ma hoac ten chuong trinh (gan dung)\n"
         << " 0. Huy\n";
    int ch = InputHelper::readInt(" Chon", 0, 4);
    if (ch == 0) return;

    vector<LichPhatSong> rows;
    const auto& all = dsLich_.getAll();
    if (ch == 3) {
        Date d;
        InputHelper::promptField("Ngay (dd/mm/yyyy)", [&](const string& x) { d = Date::parse(x); });
        copy_if(all.begin(), all.end(), back_inserter(rows), [&](const LichPhatSong& s) { return s.layNgay() == d; });
    } else {
        string key = Validator::trim(InputHelper::readLine("Nhap tu khoa: "));
        if (key.empty()) throw ValidationException("Tu khoa khong duoc de trong");
        string up = Validator::toUpper(key);
        if (ch == 1) {
            if (LichPhatSong* s = dsLich_.findById(up)) rows.push_back(*s);
        } else if (ch == 2) {
            copy_if(all.begin(), all.end(), back_inserter(rows),
                    [&](const LichPhatSong& s) { return s.layMaKenh() == up; });
        } else {
            rows = dsLich_.search(key);  // Repository<T>::search: theo ma hoac ten chuong trinh
        }
    }
    inBang(rows);
}

void LichPhatSongMenu::sapXep() {
    cout << "\n--- Sap xep lich phat song ---\n"
         << " 1. Theo ngay va gio phat (tang dan)\n"
         << " 2. Theo ten chuong trinh A-Z\n"
         << " 3. Theo ma kenh, sau do theo thoi gian\n"
         << " 0. Huy\n";
    int ch = InputHelper::readInt(" Chon", 0, 3);
    if (ch == 0) return;
    auto byTime = [](const LichPhatSong& a, const LichPhatSong& b) {
        if (a.layNgay().key() != b.layNgay().key()) return a.layNgay().key() < b.layNgay().key();
        return a.layGioBatDau().minutes() < b.layGioBatDau().minutes();
    };
    vector<LichPhatSong> rows = dsLich_.getAll();  // sap xep tren ban sao
    if (ch == 1)
        sort(rows.begin(), rows.end(), byTime);
    else if (ch == 2)
        sort(rows.begin(), rows.end(), [](const LichPhatSong& a, const LichPhatSong& b) {
            return Validator::toLower(a.layTenChuongTrinh()) < Validator::toLower(b.layTenChuongTrinh());
        });
    else
        sort(rows.begin(), rows.end(), [&](const LichPhatSong& a, const LichPhatSong& b) {
            if (a.layMaKenh() != b.layMaKenh()) return a.layMaKenh() < b.layMaKenh();
            return byTime(a, b);
        });
    inBang(rows);
}

void LichPhatSongMenu::capNhat() {
    cout << "\n--- Cap nhat lich phat song ---\n";
    string id = Validator::toUpper(Validator::trim(InputHelper::readLine("Nhap ma lich can sua: ")));
    LichPhatSong* found = dsLich_.findById(id);
    if (!found) throw NotFoundException("Khong tim thay lich " + id);
    cout << "Thong tin hien tai:\n";
    found->display();

    LichPhatSong copy = *found;  // sua tren ban sao; ma lich khong duoc sua
    ganRangBuoc(copy);
    copy.updateData();
    copy.xoaKiemTra();
    *found = copy;
    dsLich_.saveAfterUpdate();
    cout << "  Da cap nhat lich " << id << " va luu vao file.\n";
}

void LichPhatSongMenu::xoa() {
    cout << "\n--- Xoa lich phat song ---\n";
    string id = Validator::toUpper(Validator::trim(InputHelper::readLine("Nhap ma lich can xoa: ")));
    LichPhatSong* s = dsLich_.findById(id);
    if (!s) throw NotFoundException("Khong tim thay lich " + id);
    s->display();
    if (!InputHelper::confirm("Ban co chac chan muon xoa lich nay?")) {
        cout << "  Da huy thao tac xoa.\n";
        return;
    }
    dsLich_.remove(id);
    cout << "  Da xoa lich " << id << " va cap nhat file.\n";
}
