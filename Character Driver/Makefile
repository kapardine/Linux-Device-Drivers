obj-m += main.o
obj-m += main_random.o
obj-m += main_murmur.o

all :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
