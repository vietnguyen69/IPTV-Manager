// Tac gia: Nguyen Quoc Viet - B24DCVT395
// Giao dien Console

#ifndef COMMON_UI_H
#define COMMON_UI_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// Ham xoa man hinh
inline void clearScreen() {
    system("cls");
}

// Ham tam dung man hinh cho nguoi dung nhan phim Enter truoc khi tiep tuc
inline void pauseScreen() {
    cout << "\n  Nhan Enter de tiep tuc...";
    string dummy;
    getline(cin, dummy);
}

// In tieu de chuc nang voi khung vien '=' can doi
inline void printTitle(const string& title) {
    cout << "\n  " << string(58, '=') << endl;
    cout << "    " << title << endl;
    cout << "  " << string(58, '=') << endl;
}

// In duong ke ngang phan cach voi do dai tuy chinh
inline void printLine(int width = 58) {
    cout << "  " << string(width, '-') << endl;
}

// Ham template tong quat in danh sach thuc the bat ky (T ke thua Entity) duoi dang bang
template <typename T>
inline void displayTable(const vector<T>& items) {
    // Neu danh sach rong thi thong bao va dung
    if (items.empty()) {
        cout << "\n  [Danh sach trong!]" << endl;
        return;
    }
    cout << endl;
    // Goi da hinh in dong tieu de bang
    items[0].displayTableHeader();
    // Duyet qua tung phan tu de in tung dong theo so thu tu (1-indexed)
    for (size_t i = 0; i < items.size(); i++) {
        items[i].displayTableRow((int)(i + 1));
    }
    cout << "\n  Tong cong: " << items.size() << " ban ghi." << endl;
}

#endif
