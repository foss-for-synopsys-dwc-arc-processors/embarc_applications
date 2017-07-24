#ifndef PRINT_MSG_H
#define PRINT_MSG_H

/** print message for debug major function */
extern void  print_msg_func(void);

/** print message for debug awake event detecting function */
extern void  print_msg_awake(void);

/** print message for debug sleep-wake state monitoring function */
extern void  print_msg_sleep(uint state);

#endif