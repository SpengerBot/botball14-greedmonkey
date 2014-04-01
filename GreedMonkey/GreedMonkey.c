void start_position();
void watch_for_cube();
void camera_fix();
void calibrate();
void cube_is_near();
void take_position();
void claw_up();
void claw_down();
void claw_close();
void claw_close_without_cube();
void claw_open();
void drive_till_line();
void drive_till_line_backward();
void found_something();
void drive_forward();
void turn_left();
void turn_right();
void bringback();

// Created on Mo M�rz 24 2014
//Motors
#define Motor_Left 0
#define Motor_Right 2
#define Motor_Up 3
#define Turnspeed 80
#define Drivespeed 100
#define Drivespeed_slow 5
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
	enable_servos();
	calibrate();
	start_position();
	camera_open();
	take_position();
	cube_is_near();
	//end var hier
	ao();
	bringback();
	calibrate();
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
	claw_close_without_cube();
	
	set_servo_position(Servo_Back, Servo_Back_Up);	
}

void start_position(){
	printf("wait for light oida");
	set_b_button_text("I am the Twilight");
	while(!b_button()){}
	msleep(1000);
	
	motor(Motor_Up,Motor_up_speed);
	turn_left(900);
	
	drive_forward(450);
	//set_servo_position(Servo_Left, Servo_Left_Open);
	//set_servo_position(Servo_Right, Servo_Right_Open);
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
	//zur�ck 2sec
	drive_backward(1700);

	//90�
	turn_right(850);
	
	camera_fix();
	printf("nach c_fix");

	drive_forward(1500);
	
	turn_right(850);
	
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
void claw_close_without_cube(){
	set_servo_position(Servo_Left,Servo_Left_Closed+200);
	set_servo_position(Servo_Right,Servo_Right_Closed-200);
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
	freeze(Motor_Left);
	freeze(Motor_Right);

	if(get_object_count(0)!=0){
		printf("gleich gefunden\n");
		if(get_object_bbox(0,0).width > 20){
			printf("gro� genug");
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
					printf("gro� genug");
					found_something();
					funden=1;
					break;
				}
			}
			if(seconds()>s+5){
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
					printf("gro� genug");
					found_something();
					funden=1;
					break;
				}
			}
			if(seconds()>s+8){
				printf("zeit links\n");
				freeze(Motor_Right);
				//program end
				break;
			}
		}
	}

void found_something(){
	ao();
	camera_fix();
	claw_open();
	while(get_object_bbox(0,0).width < 85){
		camera_update();
		if(get_object_center(0,0).x > 120)
		{
			motor(Motor_Left,Drivespeed_slow);
			motor(Motor_Right,-Drivespeed_slow);
			camera_update();
		}
		else if(get_object_center(0,0).x < 70)
		{
			motor(Motor_Left,-Drivespeed_slow*2);
			motor(Motor_Right,Drivespeed_slow*2);
			camera_update();
		}
		else 
		{
			motor(Motor_Left,Drivespeed_slow);
			motor(Motor_Right,Drivespeed_slow);
			camera_update();
		}
		msleep(50);
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
	claw_close();
	msleep(1000);
	
}
	
void bringback(){
	drive_till_line_backward();
	
	turn_left(950);
	drive_forward(11000);
	drive_backward(500);
	turn_left(850);
	msleep(1000);
	drive_backward(1500);
	msleep(3000);
	drive_forward(5000);
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
			motor(Motor_Right,-Drivespeed_slow*4);
			msleep(50);
			while(analog(Senor_Line_Right)<Sensor_Black){}
			break;
		}
		if(analog(Senor_Line_Right)>Sensor_Black){
			freeze(Motor_Right);
			freeze(Motor_Left);
			printf("Right\n");
			motor(Motor_Left,-Drivespeed_slow*4);
			msleep(50);
			while(analog(Senor_Line_Left)<Sensor_Black){}
			break;
		}
		else{
			if(backward==0){
				motor(Motor_Left,-Drivespeed_slow*4);
				motor(Motor_Right,-Drivespeed_slow*4);
				printf("Start\n");
				backward=1;
			}	
		}
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
}
void camera_fix(){
	printf("camerafix\n");
	//double seconds_now=seconds()+3;
	//while(seconds() < seconds_now){
	//	camera_update();
	//}
	camera_update();
	msleep(2000);
	camera_update();
	printf("camerafix done\n");
}


