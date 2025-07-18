CC = gcc
CFLAGS = -Iinclude -Wall

all: scheduler

scheduler: src/main.c src/fcfs.c src/sjf.c src/srt.c src/rr.c src/hpf_non_preemptive.c src/hpf_preemptive.c src/common.c
	$(CC) $(CFLAGS) -o scheduler src/main.c src/fcfs.c src/sjf.c src/srt.c src/rr.c src/hpf_non_preemptive.c src/hpf_preemptive.c src/common.c

clean:
	rm -f scheduler
