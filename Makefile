CC = clang++
CFLAGS =

test_sl: SimCom.c PhysicalLayer.c DataLinkLayer.c ServiceLayer.c Verify.c CharQueue.c
	$(CC) $(CFLAGS) -o test_sl -l serial -l pthread -g -D TEST_SERVICE\
		SimCom.c PhysicalLayer.c DataLinkLayer.c ServiceLayer.c Verify.c CharQueue.c

test_phy: SimCom.c PhysicalLayer.c CharQueue.c
	$(CC) $(CFLAGS) -o test_phy -l serial -l pthread -g -D TEST_PHYSICAL\
		PhysicalLayer.c CharQueue.c SimCom.c

test_ddl: SimCom.c PhysicalLayer.c DataLinkLayer.c Verify.c CharQueue.c
	$(CC) $(CFLAGS) -o test_ddl -l serial -l pthread -g -D TEST_DATALINK\
		SimCom.c PhysicalLayer.c DataLinkLayer.c Verify.c CharQueue.c

clean:
	rm -f *.o
	rm -f test_phy
	rm -f test_ddl
	rm -f test_sl
