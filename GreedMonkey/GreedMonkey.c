void start_position();
void watch_for_cube();
void camera_fix();
void calibrate();
void cube_is_near();
void take_position();
void claw_up();
void claw_down();
void claw_close();
void claw_open();
void drive_till_line();
void found_something();
// Created on Mo März 24 2014
//Motors
#define Motor_Left 0
#define Motor_Right 2
#define Motor_Up 3
#define Turnspeed 80
#define Drivespeed 100
//Servos
#define	Servo_Left 0
#define Servo_Right 1
#define Servo_Back 3

#define Servo_Back_Up 2000
#define Servo_Back_Down 0
#define Servo_Left_Open 1250
#define Servo_Left_Closed 375
#define Servo_Right_Open 1000
#define Servo_Right_Closed 2030

//Button
#define Button_Up 14

//Sensor + Wert
#define Sensor_Up 1
#define Sensor_Up_Value 600
#define Senor_Line_Left 0
#define Senor_Line_Right 6
#define Sensor_Black 700

//Camera Channel
#define Yellow_Channel 0

//Motor Werte
#define Motor_up_speed 100
#define Motor_down_speed -25
#define Motor_fast_down_speed -100

//Klauen Werte
int klaue_down = 0;
int klaue_orange=0;

//orange only 
//int klaue_yellow=0;
int klaue_start=0;

//Findungsprozess
int funden=0;
//start 48500
//down 58000
void main()
{
	calibrate();
	camera_open();
	start_position();
	take_position();
	msleep(2000);

	cube_is_near();
	calibrate();
	
	//motor(Motor_Up,Motor_fast_down_speed);
	//while(!digital(Button_Up)){}
	//ao();
	//wait so it doesn't fuck up
	//msleep(500);
	//up for straffes seil
	//motor(Motor_Up,Motor_up_speed);
	//while(digital(Button_Up)){}
	//ao();
}

void calibrate(){
	
	printf("Please press the claibrate Button");
	set_b_button_text("calibrate");
	msleep(500);
	while(!b_button()){}
	display_clear();
	printf("calibrating");
	
	motor(Motor_Up,Motor_fast_down_speed);
	while(!digital(Button_Up)){}
	ao();
	//wait so it doesn't fuck up
	msleep(500);
	//up for straffes seil
	motor(Motor_Up,Motor_up_speed);
	while(digital(Button_Up)){}
	ao();
	
	enable_servos();
	claw_close();
	
	set_servo_position(Servo_Back, Servo_Back_Up);	
}

void start_position(){
	printf("wait for light oida");
	set_b_button_text("I am the Twilight");
	while(!b_button()){}
	msleep(1000);
	
	motor(Motor_Up,Motor_up_speed);
	motor(Motor_Left,-Turnspeed);
	motor(Motor_Right,Turnspeed);
	msleep(800);
	freeze(Motor_Left);
	freeze(Motor_Right);
	msleep(1200);
	motor(Motor_Left,Drivespeed);
	motor(Motor_Right,Drivespeed);
	msleep(1200);
	freeze(Motor_Left);
	freeze(Motor_Right);
	//set_servo_position(Servo_Left, Servo_Left_Open);
	//set_servo_position(Servo_Right, Servo_Right_Open);
}
void take_position()
{	
	//zurück 2sec
	motor(Motor_Left,-Drivespeed);
	motor(Motor_Right,-Drivespeed);
	msleep(1700);
	freeze(Motor_Left);
	freeze(Motor_Right);

	//90°
	motor(Motor_Left,Turnspeed);
	motor(Motor_Right,-Turnspeed);
	msleep(900);
	freeze(Motor_Left);
	freeze(Motor_Right);
	camera_fix();
	printf("nach c_fix");

	motor(Motor_Left,Drivespeed);
	motor(Motor_Right,Drivespeed);
	msleep(1500);
	
	motor(Motor_Left,Turnspeed);
	motor(Motor_Right,-Turnspeed);
	msleep(850);
	
	drive_till_line();
	
	freeze(Motor_Left);
	freeze(Motor_Right);
	while(analog(Sensor_Up) > Sensor_Up_Value){}
	freeze(Motor_Up);
	
	
}
void drive_till_line(){
	int forward=0;
	while(1){
		if(analog(Senor_Line_Left)>Sensor_Black&&analog(Senor_Line_Right)>Sensor_Black){
			printf("Abgehn\n");
			break;
		}
		if(analog(Senor_Line_Left)>Sensor_Black){
			motor(Motor_Right,0);
			motor(Motor_Left,0);
			printf("Links\n");
			motor(Motor_Right,Turnspeed);
			msleep(500);
			while(analog(Senor_Line_Left)<Sensor_Black){}
			break;
		}
		if(analog(Senor_Line_Right)>Sensor_Black){
			motor(Motor_Right,0);
			motor(Motor_Left,0);
			printf("Right\n");
			motor(Motor_Left,Turnspeed);
			msleep(500);
			while(analog(Senor_Line_Right)<Sensor_Black){}
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
	motor(Motor_Right,-70);
	motor(Motor_Left,-70);
	msleep(1000);
	motor(Motor_Right,0);
	motor(Motor_Left,0);
}

void claw_close(){
	set_servo_position(Servo_Left,Servo_Left_Closed);
	set_servo_position(Servo_Right,Servo_Right_Closed);
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
	while(!digital(Button_Up)){}
	ao();
	//wait so it doesn't fuck up
	msleep(500);
	//up for straffes seil
	motor(Motor_Up,Motor_up_speed);
	while(digital(Button_Up)){}
	ao();
}
void cube_is_near(){
	camera_update();
	printf("cube is near");
	motor(Motor_Left,0);
	motor(Motor_Right,0);

	if(get_object_count!=0){
		printf("gleich gefunden\n");
		if(get_object_confidence(0,0)>0.3){
		printf("groß genug");
		funden=1;
		found_something();
		}
	}
	double s=seconds();
	motor(Motor_Left,20);
	while(funden==0){//nichts gefunden
		camera_update();
		printf("nach rechts\n");
			if(get_object_count!=0){
								printf("1 gefunden \n");
				if(get_object_confidence(0,0)>0.3){
					printf("groß genug");
				found_something();
				funden=1;
				break;
				}
			}
			if(seconds()>s+5){
				printf("zeit rechts\n");
				motor(Motor_Left,0);
				break;
			}
		}
	s=seconds();
		motor(Motor_Right,20);
		while(funden==0){
			camera_update();
			printf("nach links drehen \n");
			if(get_object_count!=0){
				printf("1 gefunden \n");
				if(get_object_confidence(0,0)>0.3){
					printf("groß genug");
					found_something();
					funden=1;
					break;
				}
			}
			if(seconds()>s+8){
				printf("zeit links\n");
				motor(Motor_Right,0);
				break;
			}
		}
		printf("fertig");
	}

void found_something(){
	
	while(get_object_confidence(0,0)>0.5){
		camera_update();
		printf("search: %d\n",get_object_center(0,0).x);
		if(get_object_center(0,0).x > 110)
		{
			printf("it's right\n");
			motor(Motor_Left,5);
			motor(Motor_Right,-5);
			camera_update();
		}
		else if(get_object_center(0,0).x < 80)
		{
			printf("it's left\n");
			motor(Motor_Left,-5);
			motor(Motor_Right,5);
			camera_update();
		}
		else 
		{
			printf("drive\n");
			motor(Motor_Left,5);
			motor(Motor_Right,5);
			camera_update();
		}
		msleep(100);
	
		}
		printf("found beendet");
	}
void camera_fix(){
	printf("camerafix\n");
	//double seconds_now=seconds()+3;
	//while(seconds() < seconds_now){
	//	camera_update();
	//}
	camera_update();
	msleep(3000);
	camera_update();
	printf("camerafix done\n");
}


