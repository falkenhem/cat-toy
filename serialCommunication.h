/*
 * serialCommunication.h
 *
 *  Created on: 18 nov. 2021
 *      Author: 40118563
 */

#ifndef SERIALCOMMUNICATION_H_
#define SERIALCOMMUNICATION_H_

void uart_init();
void uart_TransmitPolling();
void uart_putstr(char *data);

#endif /* SERIALCOMMUNICATION_H_ */
