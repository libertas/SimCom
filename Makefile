test_sl: SimCom.c PhysicalLayer.c DataLinkLayer.c ServiceLayer.c Verify.c CharQueue.c
	gcc -o test_sl -g -D TEST_SERVICE SimCom.c PhysicalLayer.c DataLinkLayer.c ServiceLayer.c Verify.c CharQueue.c

test_phy: SimCom.c PhysicalLayer.c CharQueue.c
	gcc -o test_phy -g -D TEST_PHYSICAL PhysicalLayer.c CharQueue.c SimCom.c

test_ddl: SimCom.c PhysicalLayer.c DataLinkLayer.c Verify.c CharQueue.c
	gcc -o test_ddl -g -D TEST_DATALINK SimCom.c PhysicalLayer.c DataLinkLayer.c Verify.c CharQueue.c

clean:
	rm -f *.o
	rm -f test_phy
	rm -f test_ddl
	rm -f test_sl
