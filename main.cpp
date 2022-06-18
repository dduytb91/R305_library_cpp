#include "user.h"

char port[] = "/dev/ttyUSB0"; // Ten cong ket noi voi module R305

user _user = user(port); // Khoi tao doi tuong + ket noi module

string str;
int num;

int main()
{
    cout << "Nhap ki tu:\n";
    while (1)
    {
        getline(std::cin, str);
        num = stoi(str);
        if (num == 1)
        {
            cout << "Quet van tay!\n";
            while (1)
            {
                _user.checkSensor();       // Goi ham checkSensor lien tuc de quet van tay
                int i = _user.getBuffer(); // Kiem tra buffer xem co van tay nao duoc phat hien khong
                if (i != BUFFER_EMPTY)     // Phat hien van tay
                {
                    num = _user.getUserID(i);             // Doc ID nguoi dung tu van tay nhap vao
                    str = _user.getUserName(num);         // Doc ten nguoi dung tu ma nguoi dung
                    cout << "User: " << str << std::endl; // In ten nguoi dung ra man hinh
                    break;
                }
            }
        }
        if (num == 2)
        {
            cout << "Them User!\n";
            cout << "Nhap vao ID cua User: ";
            getline(std::cin, str);
            num = stoi(str);
            cout << "Nhap vao ho ten cua User: ";
            getline(std::cin, str);
            if (_user.addUser(num, str) < 0) // Them nguoi dung vao CSDL voi thong tin duoc nhap tu ban phim.
                return 0;
            cout << "Them User thanh cong.\n";
        }
        if (num == 3)
        {
            cout << "Xoa User!\n";
            cout << "Nhap vao ID cua User: ";
            getline(std::cin, str);
            num = stoi(str);
            if (_user.deleteUser(num) < 0) // Xoa nguoi dung khoi CSDL
                return 0;
            cout << "Xoa User thanh cong.\n";
        }
        if (num == 4)
        {
        }
    }
    return 1;
}
