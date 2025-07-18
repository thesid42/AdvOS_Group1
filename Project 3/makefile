CC=gcc
CFLAGS=-Wall -pthread
OBJS=main.o globals.o customer.o seller.o seating.o

TicketSimulator: $(OBJS)
	$(CC) $(CFLAGS) -o TicketSimulator $(OBJS)

main.o: main.c globals.h
	$(CC) $(CFLAGS) -c main.c

globals.o: globals.c globals.h
	$(CC) $(CFLAGS) -c globals.c

customer.o: customer.c globals.h
	$(CC) $(CFLAGS) -c customer.c

seller.o: seller.c globals.h
	$(CC) $(CFLAGS) -c seller.c

seating.o: seating.c globals.h
	$(CC) $(CFLAGS) -c seating.c

clean:
	rm -f *.o TicketSimulator