void turn(int times); 
//Motors
#define Motor_Left 2
#define Motor_Right 0
#define Motor_Up 1

//Servos
#define	Servo_Left 3
#define Servo_Right 1
#define Servo_Back 0

#define Servo_Back_Up 2000
#define Servo_Back_Down 0
#define Servo_Left_Open 1250
#define Servo_Left_Closed 375
#define Servo_Right_Open 1000
#define Servo_Right_Closed 2030

//Button
#define Button_Up 14

//Sensor Wert
#define Sensor_Up 1
#define Sensor_Up_Value 600

//Camera Channel
#define Yellow_Channel 0

//Motor Werte
#define Motor_up_speed 100
#define Motor_down_speed -30

//turn
//#define turn_x 0

#define wert 575
void main()
{
	
	turn(4);
}

void turn(int times)
{
	int turn_x = 0;
	
	if(times > 0)
	{
		while(turn_x < times)
		{	
			motor(Motor_Left,100);
			motor(Motor_Right,-100);
			msleep(680);
			freeze(Motor_Left);
			freeze(Motor_Right);
			printf("%d",turn_x*90);
			turn_x++;
			msleep(1000);
		}
	}
	else
	{
		while(turn_x < times*-1)
		{	
			motor(Motor_Left,100*-1);
			motor(Motor_Right,-100*-1);
			msleep(680);
			freeze(Motor_Left);
			freeze(Motor_Right);
			printf("%d",turn_x*90);
			turn_x++;
			msleep(1000);
		}
	}
	
	
}
