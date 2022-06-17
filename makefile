R305_test: main.o Adafruit_Fingerprint.o
	g++ -o R305_test main.o Adafruit_Fingerprint.o -lwiringPi
main.o: main.cpp Adafruit_Fingerprint.h
	g++ -c main.cpp
Adafruit_Fingerprint.o: Adafruit_Fingerprint.cpp Adafruit_Fingerprint.h
	g++ -c Adafruit_Fingerprint.cpp