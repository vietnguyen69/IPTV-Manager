// Tac gia: Nguyen Quoc Viet - B24DCVT395
// Repository

#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

// Ho tro tao thu muc tu dong tren Windows va Linux/macOS
#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
#endif

using namespace std;

// Lop mau quan ly thuc the T (T phai la lop con ke thua tu Entity)
template <typename T>
class Repository {
private:
    vector<T> items;      // Danh sach doi tuong luu tru tren bo nho RAM
    string fileName;      // Duong dan file luu tru tren dia (VD: "data/theloaikenh.txt")

    // Ham noi bo kiem tra va tu dong tao thu muc cha (VD: "data/") neu chua ton tai
    void ensureDirectoryExists() {
        size_t slashPos = fileName.find_last_of("/\\");
        if (slashPos != string::npos) {
            string dir = fileName.substr(0, slashPos);
            #ifdef _WIN32
                _mkdir(dir.c_str());
            #else
                mkdir(dir.c_str(), 0777);
            #endif
        }
    }

public:
    // Constructor khoi tao gan voi file du lieu, tu tao thu muc va nap du lieu ban dau
    Repository(const string& file) : fileName(file) {
        ensureDirectoryExists();
        loadFromFile();
    }

    // Doc toan bo du lieu tu file van ban vao danh sach items tren RAM
    void loadFromFile() {
        items.clear();
        ifstream fin(fileName);

        // Neu file chua ton tai thi tao file rong de san sang su dung
        if (!fin.is_open()) {
            ensureDirectoryExists();
            ofstream fout(fileName);
            if (fout.is_open()) {
                fout.close();
            }
            return;
        }

        string line;
        // Doc tung dong van ban trong file
        while (getline(fin, line)) {
            if (line.empty()) continue; // Bo qua dong trong
            try {
                T item;
                item.fromFileString(line); // Goi giai tuan tu hoa nap vao doi tuong
                items.push_back(item);
            } catch (const exception& e) {
                // Bo qua cac dong bi loi dinh dang va bao loi ra luong cerr
                cerr << "  [CANH BAO] Bo qua dong loi trong file: " << e.what() << endl;
            }
        }
        fin.close();
    }

    // Luu toan bo danh sach items tu RAM xuong file dia
    void saveToFile() {
        ensureDirectoryExists();
        ofstream fout(fileName);
        if (!fout.is_open()) {
            throw runtime_error("Khong the mo file '" + fileName + "' de ghi!");
        }
        // Duyet qua tung phan tu de ghi chuoi serialized (toFileString) xuong file
        for (const auto& item : items) {
            fout << item.toFileString() << endl;
        }
        fout.close();
    }

    // Them mot phan tu moi vao danh sach (tu dong kiem tra trung khoa chinh)
    bool add(const T& item) {
        for (const auto& existing : items) {
            if (existing.getId() == item.getId()) return false; // Trung khoa chinh -> tu choi
        }
        items.push_back(item);
        saveToFile(); // Tu dong luu thay doi xuong file
        return true;
    }

    // Tim kiem chinh xac theo khoa chinh (Primary Key), tra ve con tro den doi tuong trong RAM
    T* findById(const string& id) {
        for (auto& item : items) {
            if (item.getId() == id) return &item;
        }
        return nullptr; // Khong tim thay
    }

    // Dong bo du lieu xuong file sau khi chinh sua thong tin qua con tro findById
    void saveAfterUpdate() {
        saveToFile();
    }

    // Xoa mot phan tu theo khoa chinh
    bool remove(const string& id) {
        auto it = find_if(items.begin(), items.end(),
            [&id](const T& item) {
                return item.getId() == id;
            });

        if (it != items.end()) {
            items.erase(it);
            saveToFile(); // Dong bo file sau khi xoa
            return true;
        }
        return false;
    }

    // Lay toan bo danh sach (ho tro ca doc va sua doi)
    vector<T>& getAll() { return items; }
    const vector<T>& getAll() const { return items; }

    // Tim kiem gan dung khong phan biet hoa thuong tren ma (getId) va ten (getDisplayName)
    vector<T> search(const string& keyword) const {
        vector<T> results;
        string lowerKey = keyword;
        transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);

        for (const auto& item : items) {
            string id = item.getId();
            string name = item.getDisplayName();
            transform(id.begin(), id.end(), id.begin(), ::tolower);
            transform(name.begin(), name.end(), name.begin(), ::tolower);

            // Kiem tra tu khoa co xuat hien trong ma hoac ten khong
            if (id.find(lowerKey) != string::npos || name.find(lowerKey) != string::npos) {
                results.push_back(item);
            }
        }
        return results;
    }

    // Dem so luong ban ghi hien co
    int count() const { return (int)items.size(); }

    // Kiem tra danh sach co rong hay khong
    bool isEmpty() const { return items.empty(); }
};

#endif