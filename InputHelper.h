/*
 * File   : InputHelper.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Ham nhap lieu an toan tu ban phim (khong treo khi nhap sai, khong lap vo han khi EOF).
 */
#pragma once
#include <iostream>
#include <string>
#include "Exceptions.h"
#include "Validator.h"

class InputHelper {
public:
    /** Doc mot dong; nem InputEndedException khi het du lieu. */
    static std::string readLine(const std::string& prompt) {
        std::cout << prompt << std::flush;
        std::string line;
        if (!std::getline(std::cin, line)) throw InputEndedException();
        return line;
    }

    /** Nhap den khi apply(chuoi) khong nem ValidationException. Dung cho chuc nang Them. */
    template <class F>
    static void promptField(const std::string& label, F apply) {
        while (true) {
            std::string s = Validator::trim(readLine(label + ": "));
            try {
                apply(s);
                return;
            } catch (const ValidationException& e) {
                std::cout << "  Loi: " << e.what() << ". Vui long nhap lai.\n";
            }
        }
    }

    /** Nhap de sua: bo trong = giu nguyen gia tri hien tai. Dung cho chuc nang Cap nhat. */
    template <class F>
    static void updateField(const std::string& label, const std::string& current, F apply) {
        while (true) {
            std::string s = Validator::trim(readLine(label + " [" + current + "] (Enter = giu nguyen): "));
            if (s.empty()) return;
            try {
                apply(s);
                return;
            } catch (const ValidationException& e) {
                std::cout << "  Loi: " << e.what() << ". Vui long nhap lai.\n";
            }
        }
    }

    /** Nhap so nguyen trong [lo, hi]. */
    static int readInt(const std::string& prompt, int lo, int hi) {
        int result = lo;
        promptField(prompt, [&](const std::string& s) {
            int v = Validator::toInt(s);
            if (v < lo || v > hi)
                throw ValidationException("Gia tri phai nam trong khoang " + std::to_string(lo) +
                                          " den " + std::to_string(hi));
            result = v;
        });
        return result;
    }

    /** Hoi xac nhan y/n. */
    static bool confirm(const std::string& prompt) {
        while (true) {
            std::string s = Validator::toLower(Validator::trim(readLine(prompt + " (y/n): ")));
            if (s == "y" || s == "yes") return true;
            if (s == "n" || s == "no") return false;
            std::cout << "  Chi nhap y hoac n.\n";
        }
    }

    static void pause() { readLine("\nNhan Enter de tiep tuc..."); }
};
