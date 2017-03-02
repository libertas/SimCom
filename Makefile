test_phy: SimCom.c PhysicalLayer.c CharQueue.c
	gcc -D TEST_PHYSICAL -o test_phy PhysicalLayer.c CharQueue.c SimCom.c

clean:
	rm -f *.o
	rm -f test_phy
