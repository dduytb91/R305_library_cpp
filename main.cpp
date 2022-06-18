#include "user.h"

char port[] = "/dev/ttyUSB0";

user _user = user(port);
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
            cout << "Quet van tay:\n";
            while (1)
            {
                _user.checkSensor();
                int i = _user.getBuffer();
                if (i != BUFFER_EMPTY)
                {
                    num = _user.getUserID(i);
                    str = _user.getUserName(num);
                    cout << "User: " << str << std::endl;
                    break;
                }
            }
        }
        if (num == 2)
        {
            cout << "Them User:\n";
            cout << "Nhap vao ID cua User: ";
            getline(std::cin, str);
            num = stoi(str);
            cout << "\nNhap vao ho ten cua User: ";
            getline(std::cin, str);
            if (_user.addUser(num, str) < 0)
                return 0;
            cout << "Them User thanh cong.\n";
        }
        if (num == 3)
        {
            cout << "Xoa User:\n";
            cout << "Nhap vao ID cua User: ";
            getline(std::cin, str);
            num = stoi(str);
            if (_user.deleteUser(num) < 0)
                return 0;
            cout << "Xoa User thanh cong.\n";
        }
        if (num == 4)
        {
        }
    }
    return 1;
}
