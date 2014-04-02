
void start_position();
void watch_for_cube();
void calibrate();
void cube_is_near();
void take_position();
void claw_up();
void claw_down();
void claw_close();
void claw_drop();
void claw_open();
void drive_till_line();
void drive_till_line_backward();
void found_something();
void drive_forward();
void turn_left();
void turn_right();
void bringback();
void bringback2cube();

// Created on Mo März 24 2014
//Motors
#define Motor_Left 0
#define Motor_Right 2
#define Motor_Up 3
#define Turnspeed 80
#define Drivespeed 100
#define Drivespeed_slow 5
#define Drivespeed_middle 40
//Servos
#define	Servo_Left 0
#define Servo_Right 1
#define Servo_Back 3

#define Servo_Back_Up 2000
#define Servo_Back_Down 0
#define Servo_Left_Open 600
#define Servo_Left_Closed 250
#define Servo_Right_Open 1600
#define Servo_Right_Closed 2000
#define Servo_Right_Drop 1850

//Button
#define Button_Up 14

//Sensor + Wert
#define Sensor_Up 1
#define Sensor_Up_Value 650
#define Senor_Line_Left 0
#define Senor_Line_Right 6
#define Sensor_Black 700
#define Sensor_Down 7
#define Sensor_Down_Value 800
#define Sensor_Light 2
#define Sensor_Light_on 950

//Camera Channel
#define Yellow_Channel 0

//Motor Werte
#define Motor_up_speed 100
#define Motor_down_speed -80
#define Motor_fast_down_speed -100

//Klauen Werte
int klaue_down = 0;
int klaue_orange=0;

//orange only 
//int klaue_yellow=0;
int klaue_start=0;

//Findungsprozess
int funden=0;
double start=0;
//start 48500
//down 58000
void main()
{

	enable_servos();
	calibrate();
	
	//wait for light
	printf("wait for light oida");
	set_b_button_text("I am the Twilight");
	while(!b_button()){}
	shut_down_in(115);
	start=seconds();
	
	start_position();
	camera_open();
	take_position();
	cube_is_near();
	ao();
	bringback();
	bringback2cube();
}


void calibrate(){
	printf("Please press the claibrate Button");
	set_b_button_text("calibrate");
	msleep(500);
	while(!b_button()){}
	display_clear();
	printf("calibrating");
	
	motor(Motor_Up,Motor_down_speed);
	while(analog(Sensor_Down)> Sensor_Down_Value){}
	ao();
	//wait so it doesn't fuck up
	msleep(500);
	//up for straffes seil
	motor(Motor_Up,Motor_up_speed);
	while(analog(Sensor_Down)< Sensor_Down_Value){}
	ao();
	
	enable_servos();
	claw_close();
	
	set_servo_position(Servo_Back, Servo_Back_Up);	
}

void start_position(){
	msleep(1000);
	
	motor(Motor_Up,Motor_up_speed);
	turn_left(900);
	
	drive_forward(450);
}
void drive_forward(int delay){
	motor(Motor_Left,Drivespeed);
	motor(Motor_Right,Drivespeed);
	msleep(delay);
	freeze(Motor_Left);
	freeze(Motor_Right);
	msleep(100);
}
void drive_backward(int delay){
	motor(Motor_Left,-Drivespeed);
	motor(Motor_Right,-Drivespeed);
	msleep(delay);
	freeze(Motor_Left);
	freeze(Motor_Right);
	msleep(100);
}
void turn_right(int delay){
	motor(Motor_Left,Turnspeed);
	motor(Motor_Right,-Turnspeed);
	msleep(delay);
	freeze(Motor_Left);
	freeze(Motor_Right);
	msleep(100);
}
void turn_left(int delay){
	motor(Motor_Left,-Turnspeed);
	motor(Motor_Right,Turnspeed);
	msleep(delay);
	freeze(Motor_Left);
	freeze(Motor_Right);
	msleep(100);
}
void take_position()
{	
	//zurück 2sec
	drive_backward(1700);

	//90°
	turn_right(900);
	
	camera_update();
	printf("nach c_fix");

	drive_forward(1500);
	
	turn_right(900);
	
	drive_till_line();
}
void drive_till_line(){
	int forward=0;
	while(1){
		if(analog(Senor_Line_Left)>Sensor_Black&&analog(Senor_Line_Right)>Sensor_Black){
			printf("Abgehn\n");
			break;
		}
		if(analog(Senor_Line_Left)>Sensor_Black){
			freeze(Motor_Right);
			freeze(Motor_Left);
			printf("Links\n");
			motor(Motor_Right,Turnspeed);
			msleep(50);
			while(analog(Senor_Line_Right)<Sensor_Black){}
			break;
		}
		if(analog(Senor_Line_Right)>Sensor_Black){
			freeze(Motor_Right);
			freeze(Motor_Left);
			printf("Right\n");
			motor(Motor_Left,Turnspeed);
			msleep(50);
			while(analog(Senor_Line_Left)<Sensor_Black){}
			break;
		}
		else{
			if(forward==0){
				motor(Motor_Left,Drivespeed);
				motor(Motor_Right,Drivespeed);
				printf("Start\n");
				forward=1;
			}	
		}
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
	while(analog(Sensor_Up) > Sensor_Up_Value){}
	freeze(Motor_Up);
	
	motor(Motor_Left,-10);
	motor(Motor_Right,-10);
	msleep(1000);
	freeze(Motor_Left);
	freeze(Motor_Right);
}

void claw_close(){
	set_servo_position(Servo_Left,Servo_Left_Closed);
	set_servo_position(Servo_Right,Servo_Right_Closed);
}
void claw_drop(){
	set_servo_position(Servo_Left,Servo_Left_Open);
	set_servo_position(Servo_Right,Servo_Right_Drop);
}
void claw_open(){
	set_servo_position(Servo_Left,Servo_Left_Open);
	set_servo_position(Servo_Right,Servo_Right_Open);
}
void claw_up(){
	motor(Motor_Up,Motor_up_speed);
	while(analog(Sensor_Up) > Sensor_Up_Value){}
	ao();
}

void claw_down(){
	motor(Motor_Up,Motor_down_speed);
	while(analog(Sensor_Down)>Sensor_Down_Value){}
	ao();
	//wait so it doesn't fuck up
	msleep(500);
	//up for straffes seil
	motor(Motor_Up,Motor_up_speed);
	while(analog(Sensor_Down)< Sensor_Down_Value){}
	ao();
}
void cube_is_near(){
	camera_update();
	msleep(500);
	camera_update();
	claw_open();
	printf("cube is near");
	freeze(Motor_Left);
	freeze(Motor_Right);

	if(get_object_count(0)!=0){
		printf("gleich gefunden\n");
		if(get_object_bbox(0,0).width > 20){
			printf("groß genug");
			funden=1;
			found_something();
		}
	}
	double s=seconds();
	motor(Motor_Left,30);
	while(funden==0){//nichts gefunden
		camera_update();
		printf("nach rechts\n");
			if(get_object_count!=0){
				printf("1 gefunden \n");
				if(get_object_bbox(0,0).width > 20){
					printf("groß genug");
					found_something();
					funden=1;
					break;
				}
			}
			if(seconds()>s+4){
				printf("zeit rechts\n");
				freeze(Motor_Left);
				break;
			}
		}
		s=seconds();
		motor(Motor_Right,30);
		while(funden==0){
			camera_update();
			printf("nach links drehen \n");
			if(get_object_count!=0){
				printf("1 gefunden \n");
				if(get_object_bbox(0,0).width > 20){
					printf("groß genug");
					found_something();
					funden=1;
					break;
				}
			}
			if(seconds()>s+8){
				printf("zeit links\n");
				freeze(Motor_Right);
				break;
			}
		}
	}

void found_something(){
	ao();
	camera_update();
	while(get_object_bbox(0,0).width < 85){
		camera_update();
		if(get_object_center(0,0).x > 120)
		{
			motor(Motor_Left,Drivespeed_middle/2);
			motor(Motor_Right,-Drivespeed_middle/2);
			camera_update();
		}
		else if(get_object_center(0,0).x < 70)
		{
			motor(Motor_Left,-Drivespeed_middle/2);
			motor(Motor_Right,Drivespeed_middle/2);
			camera_update();
		}
		else 
		{
			motor(Motor_Left,Drivespeed_middle/2);
			motor(Motor_Right,Drivespeed_middle/2);
			camera_update();
		}
		msleep(50);
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
	claw_close();
	msleep(1000);
}
void bringback2cube(){
	motor(Motor_Up,Motor_up_speed);
	drive_backward(1000);
	
	turn_right(950);
	drive_forward(1000);
	drive_backward(500);
	turn_right(1300);
	drive_forward(1000);
	
	while(analog(Sensor_Up) > Sensor_Up_Value){}
	freeze(Motor_Up);
	
	camera_update();
	msleep(500);
	camera_update();
	
	cube_is_near();
	drive_backward(1000);
	turn_left(1300);
	motor(Motor_Up,Motor_down_speed);
	drive_forward(2000);
	drive_backward(250);
	turn_left(950);
	
	motor(Motor_Left,Drivespeed_middle*2);
	motor(Motor_Right,Drivespeed_middle*2-3);
	msleep(6000);
	freeze(Motor_Left);
	freeze(Motor_Right);
	
	motor(Motor_Left,-1*Drivespeed_middle*2);
	motor(Motor_Right,-1*Drivespeed_middle*2+3);
	msleep(1000);
	freeze(Motor_Left);
	freeze(Motor_Right);
	
	while(analog(Sensor_Down)>Sensor_Down_Value){
	if(seconds()>start+113)
		claw_drop();
	}
	freeze(Motor_Up);
	
	msleep(2000);
	claw_drop();
}
void bringback(){
	drive_till_line_backward();
	
	motor(Motor_Left,Drivespeed_middle);
	motor(Motor_Right,Drivespeed_middle);
	msleep(500);
	freeze(Motor_Left);
	freeze(Motor_Right);
	
	turn_left(930);
	motor(Motor_Up,Motor_down_speed);
	drive_forward(12000);
	drive_backward(250);
	turn_left(950);


	
	motor(Motor_Left,Drivespeed_middle*2);
	motor(Motor_Right,Drivespeed_middle*2-3);
	msleep(6000);
	freeze(Motor_Left);
	freeze(Motor_Right);
	
	motor(Motor_Left,-1*Drivespeed_middle*2);
	motor(Motor_Right,-1*Drivespeed_middle*2+3);
	msleep(1000);
	freeze(Motor_Left);
	freeze(Motor_Right);
	
	while(analog(Sensor_Down)>Sensor_Down_Value){}
	freeze(Motor_Up);
	
	msleep(2000);
	claw_drop();
	
}
void drive_till_line_backward(){
	int backward=0;
	while(1){
		if(analog(Senor_Line_Left)>Sensor_Black&&analog(Senor_Line_Right)>Sensor_Black){
			printf("Abgehn\n");
			break;
		}
		if(analog(Senor_Line_Left)>Sensor_Black){
			freeze(Motor_Right);
			freeze(Motor_Left);
			printf("Links\n");
			motor(Motor_Right,-Drivespeed_middle);
			msleep(50);
			while(analog(Senor_Line_Right)<Sensor_Black){}
			break;
		}
		if(analog(Senor_Line_Right)>Sensor_Black){
			freeze(Motor_Right);
			freeze(Motor_Left);
			printf("Right\n");
			motor(Motor_Left,-Drivespeed_middle);
			msleep(50);
			while(analog(Senor_Line_Left)<Sensor_Black){}
			break;
		}
		else{
			if(backward==0){
				motor(Motor_Left,-Drivespeed_middle);
				motor(Motor_Right,-Drivespeed_middle);
				printf("Start\n");
				backward=1;
			}	
		}
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
}




