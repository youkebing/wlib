#ifndef EVENT_H
#define EVENT_H

#define MAX_EVENT_NUM			512

typedef void (*evcb_func)(void *arg);

typedef struct event_callback {
	int fd;
	void *arg;
        struct epoll_event *event;
	//evcb_fun func;
	void (*func)(void *arg);
}EVENT_CALLBACK;

typedef struct event_base {
	int fd;
	int event_num;
	struct epoll_event *event;
}EVENT_BASE;

EVENT_BASE *event_init(int event_num);
void event_destroy(EVENT_BASE *event_base);
int add_event(int fd, int ev_flags, EVENT_BASE *base, evcb_func func, void *arg);
int del_event(int fd, int ev_flags, EVENT_BASE *base, struct epoll_event *event);
int update_event(EVENT_BASE *base, int ev_flags, struct epoll_event *event);
int event_loop(EVENT_BASE *base);

#endif
