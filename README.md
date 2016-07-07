# WebSockets for ESP8266


## About WebSock8A
	WebSock8A is a arduino ide based firmaware for the ESP8266.  
	WebSock8A is a web Socket firmware for the ESP8266 microcontroller built.  
	The websocket example file is called WebSock8A.ino  
	It is located in folder WebSockets/ANewWebSocks/examples/WebSock8A/  
	It uses c++ a library that is contain with folder WebSockets.  
	It requires the arduino ide, and the esp sdk devellopment stack.  
	This particular example is intended for a Linux distribution.  
	To Compile it follow the instruction Steps below.  
	Although there are many details, every distribution is different, this one is based on Debian 8.  
	There is enough direction in here for you to have a succesful outcome.  


## FEATURES
1.      A web server on port 80, serving the websocket page.  
2.	A web socket server on port 81, handling the traffic using tcp/ip.    
3.	It uses a Static IP Address of 192.168.1.123  
4.	A Speed tester to show how fast websocket are.  
5.	A LED Toggle button on pin 2. the Toggle button shows status and can be toggle by all connected devices.  
6.	Many devices can be connected and interact with each other.  
7.	A Scan for Wifi AP is integrated.  
8.	IMPORTANT --> Change the SSID  and Password before compiling.  


## INTRUCTION STEPS
1.	esp-open-sdk Installation.  
2.	arduno ide Installation.  
3.	Compile WebSock8A.ino  
	


## 1.	esp-open-sdk Installation. 
	The size of the esp-open-sdk folder when done after 90 minutes of compiling will be in excess of 144,000 files and 3GB.  
1.	$ sudo apt-get install -y git autoconf automake build-essential gperf bison flex texinfo libtool libtool-bin  
        $ sudo apt-get install -y ncurses-dev libncurses5-dev wget gawk libc6-dev-i386 python-serial libexpat-dev  
	$ sudo apt-get install -y gperf flex bison texinfo help2man python2.7-dev make unrar-free sed  
2.	$ cd ~  
3.	$ git clone --recursive https://github.com/pfalcon/esp-open-sdk  
4.	$ cd $HOME/esp-open-sdk  
5.	$ sudo chown -R $HOME/esp-open-sdk  
6.	$ make STANDALONE=y  
	Go grab a coffee this step may take over one hour.  

	Xtensa toolchain is built, to use it:  
	export PATH=$HOME/esp-open-sdk/xtensa-lx106-elf/bin:$PATH  
	Espressif ESP8266 SDK is installed, its libraries and headers are merged with the toolchain  

7.	$ cd ~; nano .profile  
	export PATH=$HOME/esp-open-sdk/xtensa-lx106-elf/bin:$PATH  
	alias xgcc="xtensa-lx106-elf-gcc"  

	$ cd ~  
	$ nano ~/.bashrc  
	export PATH=$HOME/esp-open-sdk/xtensa-lx106-elf/bin:$PATH  

	or  
	echo 'PATH=$PATH:~/esp-open-sdk/xtensa-lx106-elf/bin' >> ~/.profile  
	echo 'PATH=$PATH:~/esp-open-sdk/esptool' >> ~/.profile  
	PATH=$PATH:~/esp-open-sdk/xtensa-lx106-elf/bin  
	PATH=$PATH:~/esp-open-sdk/esptool  

8.	$ sudo reboot   
9.	Connect the ESP8266 dev board to USB port ttyUSB0, and Test it.  
	$ cd $HOME/esp-open-sdk/examples/blinky  
	$ make ESPPORT=/dev/ttyUSB0 flash  

	$ cd $HOME/esp-open-sdk/examples/blinky/  
	$ make clean  
	rm -f blinky blinky.o blinky-0x00000.bin blinky-0x40000.bin  
	$ make  
	xtensa-lx106-elf-gcc -I. -mlongcalls   -c -o blinky.o blinky.c  
	xtensa-lx106-elf-gcc -Teagle.app.v6.ld  blinky.o  -nostdlib -Wl,--start-group -lmain -lnet80211 -lwpa -llwip -lpp -lphy -Wl,--end-group -lgcc -o blinky  
	esptool.py elf2image blinky  

	$ make flash  
	esptool.py write_flash 0 blinky-0x00000.bin 0x40000 blinky-0x40000.bin  
	Connecting...  
	Erasing flash...  
	Took 0.29s to erase flash block  
	Wrote 28672 bytes at 0x00000000 in 1.5 seconds (149.3 kbit/s)...  
	Erasing flash...  
	Took 1.32s to erase flash block  
	Wrote 186368 bytes at 0x00040000 in 10.0 seconds (149.0 kbit/s)...  

	Leaving...  

10.	Updating  
	cd ~/esp-open-sdk  
	make clean  
	git pull  
	git submodule update  



## 2.	arduno ide Installation. 

	$ cd ~  
	$ sudo wget http://blog.spitzenpfeil.org/arduino/arduino-1.6.8-linux64.tar.xz  
	$ tar xf arduino-1.6.8-linux64.tar.xz  
	$ cd $HOME/arduino-1.6.8/hardware  
	$ mkdir esp8266com  
	$ cd esp8266com  
	$ git clone https://github.com/esp8266/Arduino.git esp8266  

	You should end up with the following directory structure:  

	Arduino  
	|  
	--- hardware  
	    |  
	    --- esp8266com  
		|  
		--- esp8266  
		    |  
		    --- bootloaders  
		    --- cores  
		    --- doc  
		    --- libraries  
		    --- package  
		    --- tests  
		    --- tools  
		    --- variants  
		    --- platform.txt  
		    --- programmers.txt  
		    --- README.md  
		    --- boards.txt  
		    --- LICENSE  

	Download binary tools  

	cd esp8266/tools  
	python get.py  




## 3.	Compile WebSock8A.ino 

1.	cd ~/arduino-1.6.8/hardware/esp8266com/esp8266/libraries/
2	$ git clone install https://github.com/ru4reel/WebSockets.git 
3.	Start the arduino ide and select "Examples --> ANewWebSocks --> WebSock8A
4.	Select your tty port and your board.
5.	Make sure you select the correct memory size for your board.
6.	The ESP8266-01 is 512K, the ESP8266-12 is 1M
	Best wishes.

## End of Document

