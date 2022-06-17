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
    printf("Found ID #\n");
    printf(finger.fingerID);
    printf(" with confidence of \n");
    printf(finger.confidence);

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
    printf("Found ID #\n");
    printf(finger.fingerID);
    printf(" with confidence of \n");
    printf(finger.confidence);
    return finger.fingerID;
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
    printf(finger.templateCount);
    printf(" templates\n");
    printf("Waiting for valid finger...\n");

    while (1)
    {
        delay(100);
        getFingerprintIDez();
    }
    return 1;
}
