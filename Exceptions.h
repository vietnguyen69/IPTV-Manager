/*
 * File   : Exceptions.h
 * Tac gia: Duong Duc Manh - MSSV: B24DCVT240
 * Mo ta  : Cac lop ngoai le tu dinh nghia dung chung cho toan chuong trinh.
 */
#pragma once
#include <stdexcept>
#include <string>

/** Ngoai le goc cua ung dung. */
class AppException : public std::runtime_error {
public:
    explicit AppException(const std::string& msg) : std::runtime_error(msg) {}
};

/** Du lieu nhap vao / doc tu file khong hop le. */
class ValidationException : public AppException {
public:
    explicit ValidationException(const std::string& msg) : AppException(msg) {}
};

/** Ma dinh danh bi trung. */
class DuplicateException : public ValidationException {
public:
    explicit DuplicateException(const std::string& msg) : ValidationException(msg) {}
};

/** Khong tim thay ban ghi. */
class NotFoundException : public AppException {
public:
    explicit NotFoundException(const std::string& msg) : AppException(msg) {}
};

/** Loi doc/ghi file. */
class FileException : public AppException {
public:
    explicit FileException(const std::string& msg) : AppException(msg) {}
};

/** Luong nhap (stdin) da ket thuc (EOF) - dung de tranh lap vo han. */
class InputEndedException : public AppException {
public:
    InputEndedException() : AppException("Luong nhap da ket thuc") {}
};
