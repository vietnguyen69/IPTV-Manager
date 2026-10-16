#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cstdlib>

#include "Entity.h"
#include "Repository.h"

using namespace std;

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
