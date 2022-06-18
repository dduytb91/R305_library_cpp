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
        if (c == '2')
        {
            cout << "Them User:\n";
            cout << "Nhap vao ID cua User: ";
            int i;
            cin >> i;
            cout << "\nNhap vao ho ten cua User: ";
            string line;
            cin >> line;
            _user.addUser(i, line);
        }
        if (c == '3')
        {
            cout << "Them User:\n";
            cout << "Nhap vao ID cua User: ";
            int i;
            cin >> i;
            _user.deleteUser(i);
        }
        if (c == '4')
        {
        }
        if (c == '\n')
        {
            return 1;
        }
    }
    return 1;
}
