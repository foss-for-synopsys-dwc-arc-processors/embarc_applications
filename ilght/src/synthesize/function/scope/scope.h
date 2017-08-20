#ifndef _SCOPE_H_
#define _SCOPE_H_

#include "embARC.h"
#include "embARC_debug.h"

/* Initial the scope using uart0 */
/* Attention: only use without blueteeth */
extern uint32_t scope_init(uint32_t baudrate);

/* Output the date to scope */
extern uint32_t output_data(int16_t *out_data);

#endif /*_SCOPE_H_ */
