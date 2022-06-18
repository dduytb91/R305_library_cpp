#ifndef USER_H
#define USER_H

#include "Adafruit_Fingerprint.h"
#include "json.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>

using json = nlohmann::json;
using namespace std;

#define BUFFER_EMPTY -1

#define USER_JSON_FILE "user.json"

struct Ring_Buffer
{
    Ring_Buffer()
    {
        this->read_index = 0;
        this->write_index = 0;
    }
    uint8_t read()
    {
        uint8_t data;
        if (this->read_index != this->write_index)
        {
            data = this->data[read_index];
            this->read_index++;
            if (this->read_index > 99)
            {
                this->read_index = 0;
            }
            return data;
        }
        else
        {
            return BUFFER_EMPTY;
        }
    }
    void write(uint8_t data)
    {
        this->data[this->write_index] = data;
        this->write_index++;
        if (this->write_index > 99)
        {
            this->write_index = 0;
        }
    }
    uint8_t data[100];
    uint8_t read_index;
    uint8_t write_index;
};

class user
{
private:
    Adafruit_Fingerprint finger;
    Ring_Buffer buffer;

    uint8_t getFingerprintID(uint8_t *fingerID);
    int getFingerprintIDez(uint8_t *fingerID);
    uint8_t getFingerprintEnroll(uint16_t id);
    uint8_t downloadFingerprintTemplate(uint16_t id, uint8_t *fingerTemplate);
    uint8_t deleteFingerprint(uint16_t id);

public:
    user(char *device = NULL, uint32_t baud = 57600, uint32_t password = 0x0);
    ~user();

    int addUser(uint8_t id, string name = "");
    int deleteUser(uint8_t id);
    string getUserName(uint8_t id);
    uint8_t getUserID(uint8_t fingerprint_id);

    void checkSensor();
    int getBuffer();
};

#endif