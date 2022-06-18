#ifndef USER_H
#define USER_H

#include "Adafruit_Fingerprint.h"
#include "json.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace std;

#define USER_JSON_FILE "user.json"

class user
{
private:
    Adafruit_Fingerprint finger;

    uint8_t getFingerprintID();
    int getFingerprintIDez();
    uint8_t getFingerprintEnroll(uint16_t id);
    uint8_t downloadFingerprintTemplate(uint16_t id);
    uint8_t deleteFingerprint(uint16_t id);

public:
    user(char *device = NULL, uint32_t baud = 57600, uint32_t password = 0x0);
    ~user();

    addUser(uint8_t id, string name = "");
    deleteUser(uint8_t id);
    string getUserName(uint8_t id);
    uint8_t getUserID(uint8_t fingerprint_id);
};

#endif