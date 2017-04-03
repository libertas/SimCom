CC = clang++
CFLAGS = -O2 -std=c++11
CV_INCLUDE = -I/usr/local/include/opencv -I/usr/local/include
CV_LIBS = -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab

TARGET = simcom
OBJS = SimCom.o PhysicalLayer.o DataLinkLayer.o ServiceLayer.o Verify.o CharQueue.o


$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(CFLAGS) $(CV_LIBS) $(OBJS) -l pthread -pthread -l serial

$(OBJS):%.o:%.c
	$(CC) -c $(CFLAGS) $(CV_INCLUDE) -pthread $< -o $@


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
	rm -f $(TARGET)
	rm -f test_phy
	rm -f test_ddl
	rm -f test_sl
