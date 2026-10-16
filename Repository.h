#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
#endif

using namespace std;

template <typename T>
class Repository {
private:
    vector<T> items;
    string fileName;

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
    Repository(const string& file) : fileName(file) {
        ensureDirectoryExists();
        loadFromFile();
    }

    void loadFromFile() {
        items.clear();
        ifstream fin(fileName);

        if (!fin.is_open()) {
            ensureDirectoryExists();
            ofstream fout(fileName);
            if (fout.is_open()) {
                fout.close();
            }
            return;
        }

        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            try {
                T item;
                item.fromFileString(line);
                items.push_back(item);
            } catch (const exception& e) {
                cerr << "  [CANH BAO] Bo qua dong loi trong file: " << e.what() << endl;
            }
        }
        fin.close();
    }

    void saveToFile() {
        ensureDirectoryExists();
        ofstream fout(fileName);
        if (!fout.is_open()) {
            throw runtime_error("Khong the mo file '" + fileName + "' de ghi!");
        }
        for (const auto& item : items) {
            fout << item.toFileString() << endl;
        }
        fout.close();
    }

    bool add(const T& item) {
        for (const auto& existing : items) {
            if (existing.getId() == item.getId()) return false;
        }
        items.push_back(item);
        saveToFile();
        return true;
    }

    T* findById(const string& id) {
        for (auto& item : items) {
            if (item.getId() == id) return &item;
        }
        return nullptr;
    }

    void saveAfterUpdate() {
        saveToFile();
    }

    bool remove(const string& id) {
        auto it = find_if(items.begin(), items.end(),
            [&id](const T& item) {
                return item.getId() == id;
            });

        if (it != items.end()) {
            items.erase(it);
            saveToFile();
            return true;
        }
        return false;
    }

    vector<T>& getAll() { return items; }
    const vector<T>& getAll() const { return items; }

    vector<T> search(const string& keyword) const {
        vector<T> results;
        string lowerKey = keyword;
        transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), ::tolower);

        for (const auto& item : items) {
            string id = item.getId();
            string name = item.getDisplayName();
            transform(id.begin(), id.end(), id.begin(), ::tolower);
            transform(name.begin(), name.end(), name.begin(), ::tolower);

            if (id.find(lowerKey) != string::npos || name.find(lowerKey) != string::npos) {
                results.push_back(item);
            }
        }
        return results;
    }

    int count() const { return (int)items.size(); }
    bool isEmpty() const { return items.empty(); }
};

#endif
