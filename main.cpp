#include "user.h"

char port[] = "/dev/ttyUSB0";

user _user = user(port);

int main()
{
    addUser(10, "Phan Dinh Duy");

    while (1)
    {
        // delay(50);
        // getFingerprintIDez();
    }
    return 1;
}
