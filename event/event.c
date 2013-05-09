#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <assert.h>
#include <errno.h>

#include "event.h"

EVENT_BASE *event_init(int event_num)
{
	EVENT_BASE *event_base = NULL;
        int fd;

        fd = epoll_create(event_num);
        if (fd == -1) {
                perror("epoll_create");
                return NULL;
        }

	event_base = (EVENT_BASE *)malloc(sizeof(EVENT_BASE));
	if (!event_base) {
		close(fd);
		return NULL;
	}

	event_base->event = (struct epoll_event *)
			malloc(sizeof(struct epoll_event) * event_num);
	if (!event_base->event) {
		free(event_base);
		close(fd);
		return NULL;
	}
	event_base->fd = fd;
	event_base->event_num = event_num;
	
        return event_base;
}

void event_destroy(EVENT_BASE *event_base)
{
	close(event_base->fd);
	free(event_base->event);
	free(event_base);
}

int add_event(int fd, int ev_flags, EVENT_BASE *base, evcb_func func, void *arg)
{
        struct epoll_event *event;
        struct event_callback *call;

        call = (EVENT_CALLBACK *)malloc(sizeof(EVENT_CALLBACK));
        if (!call) {
                fprintf(stderr, "malloc failed.\n");
                return -1;
        }

        event = (struct epoll_event *)malloc(sizeof(struct epoll_event));
        if (!event) {
                fprintf(stderr, "malloc failed.\n");
                free(call);
                return -1;
        }

        event->data.ptr = (void *)call;
        event->events = ev_flags;

	call->fd = fd;
        call->arg = arg;
        call->func = func;
        call->event = event;

        if (epoll_ctl(base->fd, EPOLL_CTL_ADD, fd, event) == -1) {
                perror("epoll_ctl");
		free(event);
                free(call);
                close(fd);
                return -1;
        }

        return 0;
}

int del_event(int fd, int ev_flags, EVENT_BASE *base, struct epoll_event *event)
{
	event->events = ev_flags;

	if (epoll_ctl(base->fd, EPOLL_CTL_DEL, fd, event) == -1) {
		perror("epoll_ctl");
		return -1;
	}

	free(event->data.ptr);
	free(event);

	return 0;
}

int update_event(EVENT_BASE *base, int ev_flags, struct epoll_event *event)
{
        EVENT_CALLBACK *call = (EVENT_CALLBACK *)event->data.ptr;

        assert(call != NULL);

        event->events = ev_flags;
        event->data.ptr = call;

        if (epoll_ctl(base->fd, EPOLL_CTL_MOD, call->fd, event) == -1) {
                perror("epoll_ctl");
                return -1;
        }

        return 0;
}

int event_loop(EVENT_BASE *base)
{
        EVENT_CALLBACK *call;
        int fd_num, i;

        for (;;) {
                fd_num = epoll_wait(base->fd, base->event, base->event_num, -1);
                if (fd_num == -1) {
                        perror("epoll_wait");
                        break;
                }
                printf("fd_num: %d\n", fd_num);
                for (i = 0; i < fd_num; i++) {
                        call = (EVENT_CALLBACK *)((base->event)[i].data.ptr);
                        call->func(call->arg);
                }
        }

	return 0;
}
