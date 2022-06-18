#include "user.h"

user::user(char *device, uint32_t baud, uint32_t password)
{
    finger = Adafruit_Fingerprint(device);
    finger.begin(baud);
    delay(50);
    if (finger.verifyPassword())
    {
        printf("Found fingerprint sensor!\n");
    }
    else
    {
        printf("Did not find fingerprint sensor :(\n");
    }
    finger.getTemplateCount();
    printf("Sensor contains \n");
    printf("%u", finger.templateCount);
    printf(" templates\n");
    printf("Waiting for valid finger...\n");
}

user::~user()
{
}

void user::checkSensor()
{
    int fingerID = getFingerprintIDez();
    if (fingerID >= 0)
    {
        buffer.write(fingerID);
    }
    delay(10);
}

int user::getBuffer()
{
    if (buffer.read_index != buffer.write_index)
    {
        return buffer.read();
    }
    else
    {
        return BUFFER_EMPTY;
    }
}

int user::addUser(uint8_t id, string name)
{
    uint8_t fingerTemplate[512]; // the real template
    json file_data;
    json user_obj;
    char result[10];
    string temp;
    int index;
    json user_json_file;

    if (!getUserName(id).empty())
    {
        deleteUser(id);
    }

    user_obj["id"] = id;
    user_obj["name"] = name;
    user_obj["fingerprint_id_1"] = id * 2;
    user_obj["fingerprint_id_2"] = (id * 2) + 1;

    printf("Add User\n");

    printf("Get Fingerprint 1\n");
    if (getFingerprintEnroll(id * 2) != FINGERPRINT_OK)
        return -1;
    if (downloadFingerprintTemplate(id * 2, fingerTemplate) != FINGERPRINT_OK)
        return -1;
    temp.clear();
    for (index = 0; index < 512; ++index)
    {
        sprintf(result, "%X ", fingerTemplate[index]);
        string str(result);
        temp += str;
    }
    user_obj["fingerprint_model_1"] = temp;

    printf("Get Fingerprint 2\n");
    if (getFingerprintEnroll((id * 2) + 1) != FINGERPRINT_OK)
        return -1;
    if (downloadFingerprintTemplate((id * 2) + 1, fingerTemplate) != FINGERPRINT_OK)
        return -1;
    temp.clear();
    for (index = 0; index < 512; ++index)
    {
        sprintf(result, "%X ", fingerTemplate[index]);
        string str(result);
        temp += str;
    }
    user_obj["fingerprint_model_2"] = temp;

    // read a JSON file
    std::ifstream in_file(USER_JSON_FILE);
    in_file >> user_json_file;
    in_file.close();
    user_json_file.push_back(user_obj);

    // write prettified JSON to another file
    std::ofstream out_file(USER_JSON_FILE);
    out_file << user_json_file;
    return 1;
}

int user::deleteUser(uint8_t id)
{
    json user_json_file;

    // read a JSON file
    std::ifstream in_file(USER_JSON_FILE);
    in_file >> user_json_file;
    in_file.close();

    for (json &obj : user_json_file)
    {
        if (obj["id"] == id)
        {
            obj.clear();
            if (deleteFingerprint(id * 2) != FINGERPRINT_OK)
                return -1;
            if (deleteFingerprint((id * 2) + 1) != FINGERPRINT_OK)
                return -1;
        }
    }

    // write prettified JSON to another file
    std::ofstream out_file(USER_JSON_FILE);
    out_file << user_json_file;
    return 1;
}

string user::getUserName(uint8_t id)
{
    json user_json_file;
    string user_name;

    // read a JSON file
    std::ifstream in_file(USER_JSON_FILE);
    in_file >> user_json_file;
    in_file.close();

    for (json &obj : user_json_file)
    {
        if (obj["id"] == id)
        {
            user_name = obj["name"];
        }
    }
    return user_name;
}
int user::getUserID(uint8_t fingerprint_id)
{
    json user_json_file;

    // read a JSON file
    std::ifstream in_file(USER_JSON_FILE);
    in_file >> user_json_file;
    in_file.close();

    for (json &obj : user_json_file)
    {
        if ((obj["fingerprint_id_1"] == fingerprint_id) || (obj["fingerprint_id_2"] == fingerprint_id))
        {
            return obj["id"];
        }
    }
    return -1;
}

uint8_t user::getFingerprintID()
{
    uint8_t p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
        printf("Image taken\n");
        break;
    case FINGERPRINT_NOFINGER:
        printf("No finger detected\n");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        printf("Communication error\n");
        return p;
    case FINGERPRINT_IMAGEFAIL:
        printf("Imaging error\n");
        return p;
    default:
        printf("Unknown error\n");
        return p;
    }

    // OK success!

    p = finger.image2Tz();
    switch (p)
    {
    case FINGERPRINT_OK:
        printf("Image converted\n");
        break;
    case FINGERPRINT_IMAGEMESS:
        printf("Image too messy\n");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        printf("Communication error\n");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        printf("Could not find fingerprint features\n");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        printf("Could not find fingerprint features\n");
        return p;
    default:
        printf("Unknown error\n");
        return p;
    }

    // OK converted!
    p = finger.fingerFastSearch();
    if (p == FINGERPRINT_OK)
    {
        printf("Found a print match!\n");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        printf("Communication error\n");
        return p;
    }
    else if (p == FINGERPRINT_NOTFOUND)
    {
        printf("Did not find a match\n");
        return p;
    }
    else
    {
        printf("Unknown error\n");
        return p;
    }

    // found a match!
    printf("Found ID #");
    printf("%u", finger.fingerID);
    printf(" with confidence of ");
    printf("%u\n", finger.confidence);

    return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int user::getFingerprintIDez()
{
    uint8_t p = finger.getImage();
    if (p != FINGERPRINT_OK)
        return -1;

    p = finger.image2Tz();
    if (p != FINGERPRINT_OK)
        return -1;

    p = finger.fingerFastSearch();
    if (p != FINGERPRINT_OK)
        return -1;

    // found a match!
    printf("Found ID #");
    printf("%u", finger.fingerID);
    printf(" with confidence of ");
    printf("%u\n", finger.confidence);
    return finger.fingerID;
}

uint8_t user::getFingerprintEnroll(uint16_t id)
{
    int p = -1;
    printf("Waiting for valid finger to enroll as #");
    printf("%u\n", id);
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            printf("Image taken\n");
            break;
        case FINGERPRINT_NOFINGER:
            printf(".\n");
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            printf("Communication error\n");
            break;
        case FINGERPRINT_IMAGEFAIL:
            printf("Imaging error\n");
            break;
        default:
            printf("Unknown error\n");
            break;
        }
    }

    // OK success!

    p = finger.image2Tz(1);
    switch (p)
    {
    case FINGERPRINT_OK:
        printf("Image converted\n");
        break;
    case FINGERPRINT_IMAGEMESS:
        printf("Image too messy\n");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        printf("Communication error\n");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        printf("Could not find fingerprint features\n");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        printf("Could not find fingerprint features\n");
        return p;
    default:
        printf("Unknown error\n");
        return p;
    }

    printf("Remove finger\n");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER)
    {
        p = finger.getImage();
    }
    printf("ID ");
    printf("%u\n", id);
    p = -1;
    printf("Place same finger again\n");
    while (p != FINGERPRINT_OK)
    {
        p = finger.getImage();
        switch (p)
        {
        case FINGERPRINT_OK:
            printf("Image taken\n");
            break;
        case FINGERPRINT_NOFINGER:
            printf(".\n");
            break;
        case FINGERPRINT_PACKETRECIEVEERR:
            printf("Communication error\n");
            break;
        case FINGERPRINT_IMAGEFAIL:
            printf("Imaging error\n");
            break;
        default:
            printf("Unknown error\n");
            break;
        }
    }

    // OK success!

    p = finger.image2Tz(2);
    switch (p)
    {
    case FINGERPRINT_OK:
        printf("Image converted\n");
        break;
    case FINGERPRINT_IMAGEMESS:
        printf("Image too messy\n");
        return p;
    case FINGERPRINT_PACKETRECIEVEERR:
        printf("Communication error\n");
        return p;
    case FINGERPRINT_FEATUREFAIL:
        printf("Could not find fingerprint features\n");
        return p;
    case FINGERPRINT_INVALIDIMAGE:
        printf("Could not find fingerprint features\n");
        return p;
    default:
        printf("Unknown error\n");
        return p;
    }

    // OK converted!
    printf("Creating model for #");
    printf("%u\n", id);

    p = finger.createModel();
    if (p == FINGERPRINT_OK)
    {
        printf("Prints matched!\n");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        printf("Communication error\n");
        return p;
    }
    else if (p == FINGERPRINT_ENROLLMISMATCH)
    {
        printf("Fingerprints did not match\n");
        return p;
    }
    else
    {
        printf("Unknown error\n");
        return p;
    }

    printf("ID ");
    printf("%u\n", id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK)
    {
        printf("Stored!\n");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        printf("Communication error\n");
        return p;
    }
    else if (p == FINGERPRINT_BADLOCATION)
    {
        printf("Could not store in that location\n");
        return p;
    }
    else if (p == FINGERPRINT_FLASHERR)
    {
        printf("Error writing to flash\n");
        return p;
    }
    else
    {
        printf("Unknown error\n");
        return p;
    }
    return FINGERPRINT_OK;
}

uint8_t user::downloadFingerprintTemplate(uint16_t id, uint8_t *fingerTemplate)
{
    printf("------------------------------------\n");
    printf("Attempting to load #");
    printf("%u\n", id);
    uint8_t p = finger.loadModel(id);
    switch (p)
    {
    case FINGERPRINT_OK:
        printf("Template ");
        printf("%u", id);
        printf(" loaded\n");
        break;
    case FINGERPRINT_PACKETRECIEVEERR:
        printf("Communication error\n");
        return p;
    default:
        printf("Unknown error ");
        printf("%u\n", p);
        return p;
    }

    // OK success!

    printf("Attempting to get #");
    printf("%u\n", id);
    p = finger.getModel();
    switch (p)
    {
    case FINGERPRINT_OK:
        printf("Template ");
        printf("%u", id);
        printf(" transferring:\n");
        break;
    default:
        printf("Unknown error ");
        printf("%u\n", p);
        return p;
    }

    // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
    uint8_t bytesReceived[534]; // 2 data packets
    memset(bytesReceived, 0xff, 534);

    finger.receiveModel(bytesReceived);
    printf("Decoding packet...\n");

    // uint8_t fingerTemplate[512]; // the real template
    memset(fingerTemplate, 0xff, 512);

    // filtering only the data packets
    int uindx = 9, index = 0;
    while (index < 534)
    {
        while (index < uindx)
            ++index;
        uindx += 256;
        while (index < uindx)
        {
            fingerTemplate[index++] = bytesReceived[index];
        }
        uindx += 2;
        while (index < uindx)
            ++index;
        uindx = index + 9;
    }
    for (int i = 0; i < 512; ++i)
    {
        printf("%2X ", fingerTemplate[i]);
    }
    printf("\ndone.\n");
}

uint8_t user::deleteFingerprint(uint16_t id)
{
    uint8_t p = -1;

    p = finger.deleteModel(id);

    if (p == FINGERPRINT_OK)
    {
        printf("Deleted!\n");
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
        printf("Communication error\n");
        return p;
    }
    else if (p == FINGERPRINT_BADLOCATION)
    {
        printf("Could not delete in that location\n");
        return p;
    }
    else if (p == FINGERPRINT_FLASHERR)
    {
        printf("Error writing to flash\n");
        return p;
    }
    else
    {
        printf("Unknown error: 0x");
        printf("%X\n", p);
        return p;
    }
    return FINGERPRINT_OK;
}