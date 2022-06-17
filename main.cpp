#include "Adafruit_Fingerprint.h"

char port[] = "/dev/ttyUSB0";

Adafruit_Fingerprint finger = Adafruit_Fingerprint(port);

uint8_t getFingerprintID()
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
int getFingerprintIDez()
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

uint8_t getFingerprintEnroll(uint8_t id)
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
            printf(".");
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
    printf("%u", id);
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
            printf(".");
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
}

int main()
{
    finger.begin(57600);
    delay(5);
    if (finger.verifyPassword())
    {
        printf("Found fingerprint sensor!\n");
    }
    else
    {
        printf("Did not find fingerprint sensor :(\n");
        while (1)
        {
            delay(1);
        }
    }
    finger.getTemplateCount();
    printf("Sensor contains \n");
    printf("%u", finger.templateCount);
    printf(" templates\n");
    printf("Waiting for valid finger...\n");

    printf("Enrolling ID #3\n");
    getFingerprintEnroll(3);

    while (1)
    {
        delay(50);
        getFingerprintIDez();
    }
    return 1;
}
