all: tcpC tcpS

tcpC: tcpClient.c
	gcc tcpClient.c -o tcpC

tcpS: tcpServer.c
	gcc tcpServer.c -o tcpS

clean:
	rm -rf tcpC tcpS
