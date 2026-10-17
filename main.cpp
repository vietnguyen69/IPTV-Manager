#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cstdlib>

#include "Entity.h"
#include "Repository.h"
#include "NoiDungVOD.h"
#include "TheLoaiKenh.h"

using namespace std;

Repository<NoiDungVOD>		repoVOD("data/noidungvod.txt")
Repository<TheLoaiKenh>		repoTheLoai("data/theloaikenh.txt")

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pauseScreen() {
    cout << "\n  Nhan Enter de tiep tuc...";
    string dummy;
    getline(cin, dummy);
}

void printTitle(const string& title) {
    cout << "\n  " << string(58, '=') << endl;
    cout << "    " << title << endl;
    cout << "  " << string(58, '=') << endl;
}

void printLine() {
    cout << "  " << string(58, '-') << endl;
}

template <typename T>
void displayTable(const vector<T>& items) {
    if (items.empty()) {
        cout << "\n  [Danh sach trong!]" << endl;
        return;
    }
    cout << endl;
    items[0].displayTableHeader();
    for (size_t i = 0; i < items.size(); i++) {
        items[i].displayTableRow((int)(i + 1));
    }
    cout << "\n  Tong cong: " << items.size() << " ban ghi." << endl;
}

//1. Quan ly noi dung VOD
//Them the loai
void themTheLoai() {
    printTitle("THEM THE LOAI KENH MOI");
    try {
        TheLoaiKenh tl;
        tl.inputData();  // Nhap du lieu tu ban phim

        if (repoTheLoai.add(tl)) {
            cout << "\n  >> Them the loai kenh thanh cong!" << endl;
        } else {
            cout << "\n  [LOI] Ma the loai '" << tl.getId()
                 << "' da ton tai!" << endl;
        }
    } catch (const invalid_argument& e) {
        cout << "\n  [LOI] Du lieu khong hop le: " << e.what() << endl;
    } catch (const exception& e) {
        cout << "\n  [LOI] " << e.what() << endl;
    }
}

//Hien thi danh sach the loai
void hienThiTheLoai() {
    printTitle("DANH SACH THE LOAI KENH");
    displayTable(repoTheLoai.getAll());
}

//Tim kiem the loai
void timKiemTheLoai() {
    printTitle("TIM KIEM THE LOAI KENH");
    string keyword;
    cout << "\n  Nhap tu khoa (ma hoac ten): ";
    getline(cin, keyword);

    if (keyword.empty()) {
        cout << "\n  [LOI] Tu khoa khong duoc de trong!" << endl;
        return;
    }

    vector<TheLoaiKenh> results = repoTheLoai.search(keyword);
    cout << "\n  Ket qua tim kiem cho '" << keyword << "':";
    displayTable(results);
}

//Cap nhat the loai
void capNhatTheLoai() {
    printTitle("CAP NHAT THE LOAI KENH");
    string id;
    cout << "\n  Nhap ma the loai can cap nhat: ";
    getline(cin, id);

    TheLoaiKenh* found = repoTheLoai.findById(id);
    if (found == nullptr) {
        cout << "\n  [LOI] Khong tim thay the loai voi ma '" << id
             << "'!" << endl;
        return;
    }

    cout << "\n  --- Thong tin hien tai ---" << endl;
    found->display();
    cout << "\n  --- Nhap thong tin moi ---" << endl;

    try {
        found->updateData();             // Sua truc tiep qua con tro
        repoTheLoai.saveAfterUpdate();   // Ghi lai xuong file
        cout << "\n  >> Cap nhat thanh cong!" << endl;
    } catch (const exception& e) {
        cout << "\n  [LOI] " << e.what() << endl;
        repoTheLoai.loadFromFile();      // Huy thay doi, tai lai tu file
    }
}

//Xoa the loai
void xoaTheLoai() {
    printTitle("XOA THE LOAI KENH");
    string id;
    cout << "\n  Nhap ma the loai can xoa: ";
    getline(cin, id);

    TheLoaiKenh* found = repoTheLoai.findById(id);
    if (found == nullptr) {
        cout << "\n  [LOI] Khong tim thay the loai voi ma '" << id
             << "'!" << endl;
        return;
    }

    cout << "\n  --- Thong tin se bi xoa ---" << endl;
    found->display();

    // Kiem tra rang buoc: co noi dung VOD nao dang dung the loai nay khong?
    bool dangSuDung = false;
    for (const auto& vod : repoVOD.getAll()) {
        if (vod.getMaTheLoai() == id) {
            dangSuDung = true;
            break;
        }
    }

    if (dangSuDung) {
        cout << "\n  [LOI] Khong the xoa! Co noi dung VOD dang su dung "
             << "the loai nay." << endl;
        cout << "  Hay xoa hoac doi the loai cua cac VOD truoc." << endl;
        return;
    }

    // Xac nhan xoa
    string confirm;
    cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
    getline(cin, confirm);

    if (confirm == "Y" || confirm == "y") {
        if (repoTheLoai.remove(id)) {
            cout << "\n  >> Xoa thanh cong!" << endl;
        } else {
            cout << "\n  [LOI] Xoa that bai!" << endl;
        }
    } else {
        cout << "\n  >> Da huy thao tac xoa." << endl;
    }
}

//Menu the loai kenh
void menuTheLoai() {
    int choice = -1;
    do {
        clearScreen();
        printTitle("QUAN LY THE LOAI KENH");
        cout << "\n  1. Them the loai kenh" << endl;
        cout << "  2. Hien thi danh sach" << endl;
        cout << "  3. Tim kiem" << endl;
        cout << "  4. Cap nhat thong tin" << endl;
        cout << "  5. Xoa the loai" << endl;
        cout << "  0. << Quay lai menu chinh" << endl;
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
            case 1: themTheLoai();    pauseScreen(); break;
            case 2: hienThiTheLoai(); pauseScreen(); break;
            case 3: timKiemTheLoai(); pauseScreen(); break;
            case 4: capNhatTheLoai(); pauseScreen(); break;
            case 5: xoaTheLoai();     pauseScreen(); break;
            case 0: break;
            default:
                cout << "\n  [LOI] Lua chon khong hop le (0-5)!" << endl;
                pauseScreen();
        }
    } while (choice != 0);
}


//2. Noi dung VOD
//Them noi dung VOD
void themVOD() {
    printTitle("THEM NOI DUNG VOD MOI");

    // Kiem tra da co the loai kenh chua
    if (repoTheLoai.isEmpty()) {
        cout << "\n  [CANH BAO] Chua co the loai kenh nao trong he thong!"
             << endl;
        cout << "  Hay them the loai kenh truoc khi them noi dung VOD."
             << endl;
        return;
    }

    // Hien thi danh sach the loai de nguoi dung tham khao
    cout << "\n  --- Danh sach the loai hien co ---";
    displayTable(repoTheLoai.getAll());

    cout << endl;
    try {
        NoiDungVOD vod;
        vod.inputData();  // Nhap du lieu

        // Kiem tra ma the loai co ton tai khong (rang buoc khoa ngoai)
        if (repoTheLoai.findById(vod.getMaTheLoai()) == nullptr) {
            throw invalid_argument(
                "Ma the loai '" + vod.getMaTheLoai() + "' khong ton tai! "
                "Hay chon ma the loai tu danh sach phia tren."
            );
        }

        if (repoVOD.add(vod)) {
            cout << "\n  >> Them noi dung VOD thanh cong!" << endl;
        } else {
            cout << "\n  [LOI] Ma VOD '" << vod.getId()
                 << "' da ton tai!" << endl;
        }
    } catch (const invalid_argument& e) {
        cout << "\n  [LOI] Du lieu khong hop le: " << e.what() << endl;
    } catch (const exception& e) {
        cout << "\n  [LOI] " << e.what() << endl;
    }
}

//Hien thi danh sach VOD
void hienThiVOD() {
    printTitle("DANH SACH NOI DUNG VOD");
    displayTable(repoVOD.getAll());
}

//Tim kiem VOD
void timKiemVOD() {
    printTitle("TIM KIEM NOI DUNG VOD");
    string keyword;
    cout << "\n  Nhap tu khoa (ma hoac tieu de): ";
    getline(cin, keyword);

    if (keyword.empty()) {
        cout << "\n  [LOI] Tu khoa khong duoc de trong!" << endl;
        return;
    }

    vector<NoiDungVOD> results = repoVOD.search(keyword);
    cout << "\n  Ket qua tim kiem cho '" << keyword << "':";
    displayTable(results);
}

//Cap nhat VOD
void capNhatVOD() {
    printTitle("CAP NHAT NOI DUNG VOD");
    string id;
    cout << "\n  Nhap ma VOD can cap nhat: ";
    getline(cin, id);

    NoiDungVOD* found = repoVOD.findById(id);
    if (found == nullptr) {
        cout << "\n  [LOI] Khong tim thay noi dung VOD voi ma '" << id
             << "'!" << endl;
        return;
    }

    cout << "\n  --- Thong tin hien tai ---" << endl;
    found->display();

    // Hien thi danh sach the loai de tham khao
    cout << "\n  --- Danh sach the loai hien co ---";
    displayTable(repoTheLoai.getAll());

    cout << "\n  --- Nhap thong tin moi ---" << endl;

    // Luu ban sao de phuc hoi neu loi
    NoiDungVOD backup = *found;

    try {
        found->updateData();  // Sua truc tiep qua con tro

        // Kiem tra ma the loai moi co ton tai khong
        if (repoTheLoai.findById(found->getMaTheLoai()) == nullptr) {
            *found = backup;  // Phuc hoi ban sao
            throw invalid_argument(
                "Ma the loai '" + found->getMaTheLoai() + "' khong ton tai!"
            );
        }

        repoVOD.saveAfterUpdate();  // Ghi lai xuong file
        cout << "\n  >> Cap nhat thanh cong!" << endl;
    } catch (const exception& e) {
        *found = backup;  // Phuc hoi ban sao khi co loi
        cout << "\n  [LOI] " << e.what() << endl;
        cout << "  Thay doi da duoc huy bo." << endl;
    }
}

//Xoa VOD
void xoaVOD() {
    printTitle("XOA NOI DUNG VOD");
    string id;
    cout << "\n  Nhap ma VOD can xoa: ";
    getline(cin, id);

    NoiDungVOD* found = repoVOD.findById(id);
    if (found == nullptr) {
        cout << "\n  [LOI] Khong tim thay noi dung VOD voi ma '" << id
             << "'!" << endl;
        return;
    }

    cout << "\n  --- Thong tin se bi xoa ---" << endl;
    found->display();

    // Xac nhan xoa
    string confirm;
    cout << "\n  Ban co chac chan muon xoa? (Y/N): ";
    getline(cin, confirm);

    if (confirm == "Y" || confirm == "y") {
        if (repoVOD.remove(id)) {
            cout << "\n  >> Xoa thanh cong!" << endl;
        } else {
            cout << "\n  [LOI] Xoa that bai!" << endl;
        }
    } else {
        cout << "\n  >> Da huy thao tac xoa." << endl;
    }
}

//Menu noi dung VOD
void menuVOD() {
    int choice = -1;
    do {
        clearScreen();
        printTitle("QUAN LY NOI DUNG VOD");
        cout << "\n  1. Them noi dung VOD" << endl;
        cout << "  2. Hien thi danh sach" << endl;
        cout << "  3. Tim kiem" << endl;
        cout << "  4. Cap nhat thong tin" << endl;
        cout << "  5. Xoa noi dung VOD" << endl;
        cout << "  0. << Quay lai menu chinh" << endl;
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
            case 1: themVOD();    pauseScreen(); break;
            case 2: hienThiVOD(); pauseScreen(); break;
            case 3: timKiemVOD(); pauseScreen(); break;
            case 4: capNhatVOD(); pauseScreen(); break;
            case 5: xoaVOD();     pauseScreen(); break;
            case 0: break;
            default:
                cout << "\n  [LOI] Lua chon khong hop le (0-5)!" << endl;
                pauseScreen();
        }
    } while (choice != 0);
}

int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul"); // Ho tro tieng Viet console
    #endif

    int choice = -1;
    do {
        clearScreen();
        cout << endl;
        cout << "  ==========================================================" << endl;
        cout << "  |        HE THONG QUAN LY DICH VU TRUYEN HINH (IPTV)     |" << endl;
        cout << "  ==========================================================" << endl;
        cout << endl;
        
        cout << "  0. Thoat chuong trinh" << endl;
        cout << "  ----------------------------------------------------------" << endl;
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
            
            case 0:
                cout << "\n  Cam on ban da su dung chuong trinh! Tam biet!" << endl;
                break;
            default:
                cout << "\n  [LOI] Lua chon khong hop le!" << endl;
                pauseScreen();
        }
    } while (choice != 0);

    return 0;
}
