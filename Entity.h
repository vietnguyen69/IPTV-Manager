// Tac gia: Nguyen Quoc Viet - B24DCVT395
//Lop truu tuong entity

#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

//Lop co so truu tuong dai dien cho mot thuc the trong he thong IPTV
class Entity {
public:
    //Virtual Destructor: dam bao giai phong bo nho dung cach khi xoa qua con tro lop co so
    virtual ~Entity() {}

    //Tra ve khoa chinh (Primary Key) cua thuc the dung de dinh danh va quan ly trong Repository
    virtual string getId() const = 0;

    //Tra ve ten dai dien dung trong tim kiem gan dung va thong bao nguoi dung
    virtual string getDisplayName() const = 0;

    //Hien thi chi tiet day du thong tin cua mot ban ghi theo chieu doc
    virtual void display() const = 0;

    //In tieu de cac cot cua bang danh sach (TableHeader)
    virtual void displayTableHeader() const = 0;

    //In mot dong du lieu trong bang danh sach theo so thu tu (TableRow)
    virtual void displayTableRow(int stt) const = 0;

    //Tuan tu hoa du lieu thuc the thanh chuoi van ban ngan cach boi dau '|' de luu vao file
    virtual string toFileString() const = 0;

    //Giai tuan tu hoa: phan tich dong van ban doc tu file de nap lai vao cac thuoc tinh cua doi tuong
    virtual void fromFileString(const string& line) = 0;

    //Nhap moi toan bo du lieu thuc the tu ban phim (co kiem tra rang buoc tinh hop le)
    virtual void inputData() = 0;

    //Cap nhat du lieu cua thuc the (cho phep nhan Enter de giu nguyen gia tri cu)
    virtual void updateData() = 0;
};

#endif
