#ifndef BluetoothSender_H
#define BluetoothSender_H

#include "ev3api.h"
#include "kernel.h"

typedef struct BluetoothSender
{
} BluetoothSender;

void BluetoothSender_init(BluetoothSender* this);
void BluetoothSender_send(BluetoothSender* this);
char BluetoothSender_receive(BluetoothSender* this);

#endif // BluetoothSender_H
