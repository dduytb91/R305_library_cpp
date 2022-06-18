#include "user.h"

char port[] = "/dev/ttyUSB0";

user _user = user(port);
int id;
string name;
string line;
int num;

int main()
{
    cout << "Nhap ki tu:\n";
    while (1)
    {
        getline(std::cin,line);
        num = stoi(line);
        if (num == 1)
        {
            cout << "Quet van tay:\n";
            while (1)
            {
                _user.checkSensor();
                int i = _user.getBuffer();
                if (i != BUFFER_EMPTY)
                {
                    id = _user.getUserID(i);
                    name = _user.getUserName(id);
                    cout << "User: " << name << std::endl;
                    break;
                }
            }
        }
        if (num == 2)
        {
            cout << "Them User:\n";
            cout << "Nhap vao ID cua User: ";
            getline(std::cin,line);
            num = stoi(line);
            cout << "\nNhap vao ho ten cua User: ";
            getline(std::cin,line);
            if (_user.addUser(num, line) < 0)
                return 0;
            cout << "Them User thanh cong.\n";
        }
        if (num == 3)
        {
            cout << "Xoa User:\n";
            cout << "Nhap vao ID cua User: ";
            getline(std::cin,line);
            num = stoi(line);
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
