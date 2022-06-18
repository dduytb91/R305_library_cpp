R305_test: main.o user.o Adafruit_Fingerprint.o
	g++ -o R305_test main.o user.o Adafruit_Fingerprint.o -lwiringPi
main.o: main.cpp user.h
	g++ -c main.cpp
user.o: user.cpp user.h Adafruit_Fingerprint.h
	g++ -c user.cpp
Adafruit_Fingerprint.o: Adafruit_Fingerprint.cpp Adafruit_Fingerprint.h
	g++ -c Adafruit_Fingerprint.cpp
