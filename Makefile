test_phy: SimCom.c PhysicalLayer.c CharQueue.c
	gcc -o test_phy -D TEST_PHYSICAL PhysicalLayer.c CharQueue.c SimCom.c

test_ddl: SimCom.c PhysicalLayer.c DataLinkLayer.c Verify.c CharQueue.c
	gcc -o test_ddl -D TEST_DATALINK SimCom.c PhysicalLayer.c DataLinkLayer.c Verify.c CharQueue.c

clean:
	rm -f *.o
	rm -f test_phy
	rm -f test_ddl
