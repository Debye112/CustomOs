#ifndef NIC_H
#define NIC_H

#include <stdint.h>
#include <stddef.h>

extern uint8_t NIC_MAC[6];

void nic_init(void);
void nic_send(const uint8_t* frame, size_t len);
// returns length of received frame, 0 if none
size_t nic_receive(uint8_t* buf, size_t max_len);

#endif
