all: xbox_pub

xbox_pub: xbox_pub.cpp XboxInput.o
	g++ -DOPENSSL -I.. -I/usr/local/include -D_NDEBUG -Wall -std=c++11 -O2 -o xbox_pub xbox_pub.cpp ../XboxInput.o -L~/Mqtt/paho.mqtt.cpp/lib -L/usr/local/lib -lpaho-mqttpp3 -lpaho-mqtt3a

#xbox_sub: xbox_sub.cpp
#	g++ -DOPENSSL -I.. -I/usr/local/include -D_NDEBUG -Wall -std=c++11 -O2 -o xbox_sub xbox_sub.cpp -L~/Mqtt/paho.mqtt.cpp/lib -L/usr/local/lib -lpaho-mqttpp3 -lpaho-mqtt3a

XboxInput.o: ../XboxInput.c
	gcc -c ../XboxInput.c -o ../XboxInput.o

clean:
	rm xbox_pub ../XboxInput.o