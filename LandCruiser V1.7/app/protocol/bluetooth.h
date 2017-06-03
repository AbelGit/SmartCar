#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#include "stm32f10x.h"

// abcde fghij klmno pqrst uvwxyz
// ABCDE FGHIJ KLMNO PQRST UVWXYZ
// 80 == 0x50 == 'P'
// 100 == 0x64 == 'd'



typedef enum
{
	Mannual = 0x61, //a
	Auto,			//b
	FindLine,		//c
	
}_mode;

typedef enum
{
	Empty = 0x41,   //A
	Start,			//B
	Stop,			//C
	Forward,		//D
	Backward,		//E
	TurnLeft,		//F
	TurnRight,		//G
	LeftRotate,		//H
	RightRotate,	//I
	Sg90UpDown,		//J
	Sg90LeftRight,	//K
	Sg90Reset,		//L
	
}_cmd;

/*  
	* kai shi
	# jie shu 
*/
typedef struct
{
	u8 start;
	u8 mode;
	u8 cmd;
	u8 time;
	u8 data_l;
	u8 data_h;
	u8 end;
}_protocol;	

extern _protocol protocol;

void pack_data(u8 dev, u8 data_flag, u8 data_len, u8 *data);
void send_data(void);

void parse_recv_data(const _protocol *data);
void run_mannual(const _protocol *data);
void run_auto(const _protocol *data);
void run_find_line(const _protocol *data);
void run_empty(void);


#endif 


