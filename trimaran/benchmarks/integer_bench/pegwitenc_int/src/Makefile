#
#
#
CC = gcc 
CFLAGS = -DLITTLE_ENDIAN
LDFLAGS = 

MAIN = pegwit
OBJS = $(MAIN).o ec_crypt.o ec_curve.o ec_field.o ec_param.o ec_vlong.o \
       square.o sqcts.o sha1.o binasc.o
HDRS = ec_crypt.h ec_curve.h ec_field.h ec_param.h ec_vlong.h sha1.h \
       square.h sqcts.h square.tab binasc.h

all: $(MAIN)
	echo "DONE..."

$(MAIN): $(OBJS) $(HDRS)
	$(CC) $(LDFLAGS) -o $(MAIN) $(OBJS)

clean:
	rm -f *~ ; rm -f *.o ; rm -f $(MAIN)
