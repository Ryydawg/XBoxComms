all: XboxInput.o
	g++ -DOPENSSL -I.. -I/usr/local/include -D_NDEBUG -Wall -std=c++11 -O2 -o xbox_publish xbox_publish.cpp ../XboxInput.o -L~/Mqtt/paho.mqtt.cpp/lib -L/usr/local/lib -lpaho-mqttpp3 -lpaho-mqtt3a

XboxInput.o: ../XboxInput.c
	gcc -c ../XboxInput.c -o ../XboxInput.o

clean:
	rm publish_test