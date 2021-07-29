
all:
	g++ src/xfetch.cpp -o xfetch

install:
	sudo cp xfetch /usr/bin
	sudo chmod +x /usr/bin/xfetch