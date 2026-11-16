obj-m += syscall_shroud.o

all:
	$(MAKE) -C /opt/ddk/kdir/android16-6.12 M=$(PWD) modules

clean:
	$(MAKE) -C /opt/ddk/kdir/android16-6.12 M=$(PWD) clean
