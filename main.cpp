#include "user.h"

char port[] = "/dev/ttyUSB0";

user _user = user(port);
int id;
string name;
uint8_t c;

int main()
{
    cout << "Nhap ki tu:\n";
    while (1)
    {
        c = getchar();
        if (c == '1')
        {
            cout << "Quet van tay, nhan q de dung lai:\n";
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
                c = getchar();
                if (c == 'q')
                {
                    cout << "STOP!\n";
                    break;
                }
            }
        }
        if (c == '2')
        {
            cout << "Them User:\n";
            cout << "Nhap vao ID cua User: ";
            int i;
            cin >> i;
            cout << "\nNhap vao ho ten cua User: ";
            string line;
            cin >> line;
            if (_user.addUser(i, line) < 0)
                return 0;
            cout << "Them User thanh cong.\n";
        }
        if (c == '3')
        {
            cout << "Xoa User:\n";
            cout << "Nhap vao ID cua User: ";
            int i;
            cin >> i;
            if (_user.deleteUser(i) < 0)
                return 0;
            cout << "Xoa User thanh cong.\n";
        }
        if (c == '4')
        {
        }
    }
    return 1;
}
