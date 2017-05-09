CC = clang++
CFLAGS = -O2 -std=c++11

TARGET = simcom
OBJS = SimCom.o PhysicalLayer.o DataLinkLayer.o ServiceLayer.o Verify.o CharQueue.o

all: libsimcom.so

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(CFLAGS) $(OBJS) -l pthread -pthread -l serial

$(OBJS):%.o:%.cpp
	$(CC) -c $(CFLAGS) -fPIC -pthread $< -o $@

libsimcom.so: $(OBJS)
	$(CC) -o libsimcom.so $(CFLAGS) $(OBJS) -shared -l pthread -pthread -l serial

test_sl: SimCom.cpp PhysicalLayer.cpp DataLinkLayer.cpp ServiceLayer.cpp Verify.cpp CharQueue.cpp
	$(CC) $(CFLAGS) -o test_sl -l serial -l pthread -g -D TEST_SERVICE\
		SimCom.cpp PhysicalLayer.cpp DataLinkLayer.cpp ServiceLayer.cpp Verify.cpp CharQueue.cpp

test_phy: SimCom.cpp PhysicalLayer.cpp CharQueue.cpp
	$(CC) $(CFLAGS) -o test_phy -l serial -l pthread -g -D TEST_PHYSICAL\
		PhysicalLayer.cpp DataLinkLayer.cpp ServiceLayer.cpp Verify.cpp CharQueue.cpp SimCom.cpp

test_ddl: SimCom.cpp PhysicalLayer.cpp DataLinkLayer.cpp Verify.cpp CharQueue.cpp
	$(CC) $(CFLAGS) -o test_ddl -l serial -l pthread -g -D TEST_DATALINK\
		SimCom.cpp PhysicalLayer.cpp DataLinkLayer.cpp ServiceLayer.cpp Verify.cpp CharQueue.cpp

clean:
	rm -f *.o
	rm -f *.so
	rm -f $(TARGET)
	rm -f test_phy
	rm -f test_ddl
	rm -f test_sl
