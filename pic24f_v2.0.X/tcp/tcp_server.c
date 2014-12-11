#include "../HardwareProfile.h"
#include "tcp_server.h"
#include "../uart2.h"

#include "../uip/uip.h"

void tcp_hello_init(void)
{
    // Init tcp server
    uip_listen(HTONS(1000), hello_world_appcall);
}

int hello_world_appcall(void)
{
    uip_tcp_appstate_t *s = &(uip_conn->appstate);
    // PrintString("app\n");

    if(uip_connected()) {
      PSOCK_INIT(&s->p, s->inputbuffer, sizeof(s->inputbuffer));
    }

    PSOCK_BEGIN(&s->p);
    PSOCK_SEND_STR(&s->p, "Hello. What is your name?\n");
    PSOCK_READTO(&s->p, '\n');
    strncpy(s->name, s->inputbuffer, sizeof(s->name));
    PSOCK_SEND_STR(&s->p, "Hello ");
    PSOCK_SEND_STR(&s->p, s->name);
    PSOCK_SEND_STR(&s->p, " !");
    PSOCK_CLOSE(&s->p);
    PSOCK_END(&s->p);
}
