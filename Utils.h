//Tac gia: Nguyen Quoc Viet - B24DCVT395
//Cac ham tien ich dung chung

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Lop ngoai le tu dinh nghia
class AppException : public exception {
    string message;
public:
    AppException(const string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class InvalidInputException : public AppException {
public:
    InvalidInputException(const string& msg) : AppException("Loi nhap lieu: " + msg) {}
};

class FileException : public AppException {
public:
    FileException(const string& msg) : AppException("Loi file: " + msg) {}
};

class DuplicateIdException : public AppException {
public:
    DuplicateIdException(const string& id) : AppException("Ma '" + id + "' da ton tai!") {}
};

class NotFoundException : public AppException {
public:
    NotFoundException(const string& id) : AppException("Khong tim thay ban ghi voi ma '" + id + "'!") {}
};

class ReferentialIntegrityException : public AppException {
public:
    ReferentialIntegrityException(const string& msg) : AppException("Rang buoc du lieu: " + msg) {}
};

// Lop InputHelper - Nhap lieu an toan
class InputHelper {
public:
    // Nhap so nguyen an toan
    static int nhapInt(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
        int val;
        while (true) {
            cout << prompt;
            if (cin >> val) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (val >= minVal && val <= maxVal) return val;
                cout << "  [!] Vui long nhap so trong khoang [" << minVal << ", " << maxVal << "].\n";
            } else {
                cout << "  [!] Du lieu khong hop le, vui long nhap lai.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    // Nhap so thuc an toan
    static double nhapDouble(const string& prompt, double minVal = 0.0) {
        double val;
        while (true) {
            cout << prompt;
            if (cin >> val) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (val >= minVal) return val;
                cout << "  [!] Gia tri phai >= " << minVal << ".\n";
            } else {
                cout << "  [!] Du lieu khong hop le, vui long nhap lai.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    }

    // Nhap chuoi, cho phep rong
    static string nhapChuoi(const string& prompt) {
        cout << prompt;
        string s;
        getline(cin, s);
        return s;
    }

    // Nhap chuoi, khong cho phep rong
    static string nhapChuoiBatBuoc(const string& prompt) {
        while (true) {
            string s = nhapChuoi(prompt);
            if (!s.empty()) return s;
            cout << "  [!] Khong duoc de trong, vui long nhap lai.\n";
        }
    }

    // Nhap chuoi tuy chon (Enter de giu nguyen)
    static string nhapChuoiTuyCon(const string& prompt, const string& current) {
        cout << prompt << " [" << current << "]: ";
        string s;
        getline(cin, s);
        return s.empty() ? current : s;
    }

    // Nhap so nguyen tuy chon (Enter de giu nguyen)
    static int nhapIntTuyCon(const string& prompt, int current, int minVal = INT_MIN, int maxVal = INT_MAX) {
        cout << prompt << " [" << current << "]: ";
        string line;
        getline(cin, line);
        if (line.empty()) return current;
        try {
            int val = stoi(line);
            if (val >= minVal && val <= maxVal) return val;
            cout << "  [!] Ngoai khoang hop le, giu nguyen gia tri cu.\n";
            return current;
        } catch (...) {
            cout << "  [!] Du lieu khong hop le, giu nguyen gia tri cu.\n";
            return current;
        }
    }

    // Nhap so thuc tuy chon (Enter de giu nguyen)
    static double nhapDoubleTuyCon(const string& prompt, double current, double minVal = 0.0) {
        cout << prompt << " [" << current << "]: ";
        string line;
        getline(cin, line);
        if (line.empty()) return current;
        try {
            double val = stod(line);
            if (val >= minVal) return val;
            cout << "  [!] Gia tri phai >= " << minVal << ", giu nguyen gia tri cu.\n";
            return current;
        } catch (...) {
            cout << "  [!] Du lieu khong hop le, giu nguyen gia tri cu.\n";
            return current;
        }
    }

    // Xac nhan y/n
    static bool xacNhan(const string& prompt) {
        while (true) {
            cout << prompt << " (y/n): ";
            string s;
            getline(cin, s);
            if (s == "y" || s == "Y") return true;
            if (s == "n" || s == "N") return false;
            cout << "  [!] Vui long nhap 'y' hoac 'n'.\n";
        }
    }
};

// Cac ham tien ich xu ly chuoi
namespace StrUtils {
    // Tach chuoi theo ky tu phan cach
    inline vector<string> split(const string& s, char delim) {
        vector<string> tokens;
        stringstream ss(s);
        string token;
        while (getline(ss, token, delim)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Chuyen chuoi sang chu thuong
    inline string toLower(const string& s) {
        string res = s;
        transform(res.begin(), res.end(), res.begin(), ::tolower);
        return res;
    }

    // Trim khoang trang dau cuoi
    inline string trim(const string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        size_t end = s.find_last_not_of(" \t\r\n");
        return (start == string::npos) ? "" : s.substr(start, end - start + 1);
    }
}

// Hien thi giao dien
namespace Display {
    inline void inDuongKe(int width = 60) {
        cout << string(width, '=') << "\n";
    }

    inline void inTieuDe(const string& title, int width = 60) {
        inDuongKe(width);
        int pad = (width - (int)title.size()) / 2;
        if (pad > 0) cout << string(pad, ' ');
        cout << title << "\n";
        inDuongKe(width);
    }

    inline void inThongBaoLoi(const string& msg) {
        cout << "\n  [LOI] " << msg << "\n";
    }

    inline void inThongBaoOK(const string& msg) {
        cout << "\n  [OK] " << msg << "\n";
    }

    inline void nhanPhimDeTiep() {
        cout << "\n  Nhan Enter de tiep tuc...";
        string dummy;
        getline(cin, dummy);
    }

    template <typename T>
    inline void inBang(const vector<T>& items) {
        if (items.empty()) {
            cout << "\n  [Danh sach trong!]\n";
            return;
        }
        cout << "\n";
        items[0].displayTableHeader();
        for (size_t i = 0; i < items.size(); ++i) {
            items[i].displayTableRow((int)(i + 1));
        }
        cout << "\n  Tong cong: " << items.size() << " ban ghi.\n";
    }
}

#endif