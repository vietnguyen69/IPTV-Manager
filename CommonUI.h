#ifndef COMMON_UI_H
#define COMMON_UI_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline void pauseScreen() {
    cout << "\n  Nhan Enter de tiep tuc...";
    string dummy;
    getline(cin, dummy);
}

inline void printTitle(const string& title) {
    cout << "\n  " << string(58, '=') << endl;
    cout << "    " << title << endl;
    cout << "  " << string(58, '=') << endl;
}

inline void printLine(int width = 58) {
    cout << "  " << string(width, '-') << endl;
}

template <typename T>
inline void displayTable(const vector<T>& items) {
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

#endif // COMMON_UI_H
