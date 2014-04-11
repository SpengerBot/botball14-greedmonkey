void start_position();
void watch_for_cube();
void calibrate();
void cube_is_near();
void take_position();
void claw_up();
void claw_up_delayed();
void claw_down();
void claw_close();
void claw_drop();
void claw_open();
void drive_till_line();
void drive_till_line_backward();
void found_something();
void drive();
void bringback();
void bringback2cube();
void wisch();

//Motors
#define Motor_Left 0
#define Motor_Right 2
#define Motor_Up 3
#define Turnspeed 70
#define Drivespeed 100
#define Drivespeed_slow 5
#define Drivespeed_middle 40
//Servos
#define	Servo_Left 1
#define Servo_Right 0
#define Servo_Back 3

#define Servo_Back_Up 1800
#define Servo_Back_Down 10
#define Servo_Left_Open 760
#define Servo_Left_Closed 370
#define Servo_Right_Open 600
#define Servo_Right_Closed 1000
#define Servo_Front_Max 1500
#define Servo_Front_Min 300

//Button
#define Button_Up 14

//Sensor + Wert
#define Sensor_Up 14
//#define Sensor_Up_Value 700
#define Sensor_Line_Left 0
#define Sensor_Line_Right 6
#define Sensor_Black 900
#define Sensor_Down 13
#define Sensor_Down_Value 650
#define Sensor_Light 2 //Test Sensor = 3  Real Sensor = 2

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
	set_servo_position(Servo_Back,Servo_Back_Up);
	set_servo_position(Servo_Left,Servo_Left_Closed);
	set_servo_position(Servo_Right,Servo_Right_Closed);
	enable_servos();
	calibrate();
	
	//wait for light
	//printf("wait for light oida");
	//set_b_button_text("I am the Twilight");
	//while(!b_button()){}
	wait_for_light(Sensor_Light);
	

	//shutdown stuff
	shut_down_in(115);
	start=seconds();
	
	//start position for create
	start_position();
	
	//we see everything
	camera_open();
	
	//drive in front of the cubes
	take_position();
	//watch for cubes
	cube_is_near();
	ao();
	
	//bring the first cube to the pipes
	bringback();
	//take the 2nd cube and bring them to the pipes
	bringback2cube();
}


void calibrate(){
	printf("Please press the claibrate Button");
	set_b_button_text("calibrate");
	while(!b_button()){}
	display_clear();
	printf("calibrating");
	//claws down
	motor(Motor_Up,Motor_down_speed);
	while(!digital(Sensor_Down)){}
	freeze(Motor_Up);
	//wait so it doesn't fuck up
	msleep(500);
	//up for straffes seil
	motor(Motor_Up,Motor_up_speed);
	while(digital(Sensor_Down)){}
	freeze(Motor_Up);
	claw_close();
}

void start_position(){	
	//start taking claws up
	
	//90 links
	drive(970,-Turnspeed,Turnspeed);
	
	msleep(3000);
	motor(Motor_Up,Motor_up_speed);
	
	//gegen wand f�r kalibration
	drive(430,Drivespeed,Drivespeed);
	
}
void drive(int delay,int speed_left, int speed_right){
	motor(Motor_Left,speed_left+5);
	motor(Motor_Right,speed_right);
	msleep(delay);
	freeze(Motor_Left);
	freeze(Motor_Right);
	msleep(200);
}

void wisch() {
	set_servo_position(Servo_Front, Servo_Front_Max);
	msleep(800);
	set_servo_position(Servo_Front, Servo_Front_Min);
}
void take_position() {	
	//zur�ck 2sec
	drive(1700,-Drivespeed,-Drivespeed);
	//90 nach rechts
	drive(950,Turnspeed,-Turnspeed);
	//camera fix
	camera_update();
	//oida drive
	drive(2000,Drivespeed,Drivespeed);
	//90 nach rechts
	drive(925,Turnspeed,-Turnspeed);
	drive_till_line();
}
void drive_till_line(){
	motor(Motor_Left,Drivespeed);
	motor(Motor_Right,Drivespeed);
	printf("Start\n");
	while(1){
		if(analog(Sensor_Line_Left)>Sensor_Black&&analog(Sensor_Line_Right)>Sensor_Black){
			printf("Abgehn\n");
			break;
		} 
		if(analog(Sensor_Line_Left)>Sensor_Black) {
			freeze(Motor_Left);
			printf("Links\n");
			motor(Motor_Right,Turnspeed);
			msleep(50);
			while(analog(Sensor_Line_Right)<Sensor_Black){}
			break;
		}
		if(analog(Sensor_Line_Right)>Sensor_Black) {
			freeze(Motor_Right);
			printf("Right\n");
			motor(Motor_Left,Turnspeed);
			msleep(50);
			while(analog(Sensor_Line_Left)<Sensor_Black){}
			break;
		}
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
	//fahr back so claws doesnt break
	msleep(500);
	drive(500,-Drivespeed_middle,-Drivespeed_middle);
	
	claw_up_delayed();
}

void claw_close(){
	enable_servos();
	set_servo_position(Servo_Left,Servo_Left_Closed);
	set_servo_position(Servo_Right,Servo_Right_Closed);
}
void claw_open(){
	enable_servos();
	set_servo_position(Servo_Left,Servo_Left_Open);
	set_servo_position(Servo_Right,Servo_Right_Open);
}
void claw_up(){
	motor(Motor_Up,Motor_up_speed);
	while(!digital(Sensor_Up)){}
	freeze(Motor_Up);
}
void claw_up_delayed(){
	while(!digital(Sensor_Up)){}
	motor(Motor_Up,Motor_down_speed);
	msleep(1250);
	freeze(Motor_Up);
}
void claw_littlebit_up(){
	motor(Motor_Up,Motor_up_speed);
	msleep(1100);
	freeze(Motor_Up);
	msleep(300);
}
void claw_down(){
	motor(Motor_Up,Motor_down_speed);
	while(!digital(Sensor_Down)){}
	freeze(Motor_Up);
	//wait so it doesn't fuck up
	msleep(500);
	//up for straffes seil
	motor(Motor_Up,Motor_up_speed);
	while(!digital(Sensor_Down)){}
	freeze(Motor_Up);
}
void cube_is_near(){
	camera_update();
	msleep(500);
	camera_update();
	enable_servos();
	claw_open();
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
	motor(Motor_Right,30);
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
			if(seconds()>s+2){
				printf("zeit rechts\n");
				freeze(Motor_Right);
				break;
			}
		}
		s=seconds();
		motor(Motor_Left,30);
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
			if(seconds()>s+4){
				printf("zeit links\n");
				freeze(Motor_Left);
				break;
			}
		}
	}

void found_something(){
	ao();
	camera_update();
	msleep(100);
	camera_update();
	msleep(100);
	camera_update();
	msleep(100);
	while(get_object_bbox(0,0).width < 80){
		camera_update();
		if(get_object_center(0,0).x > 120)
		{
			motor(Motor_Left,Drivespeed_middle/2);
			motor(Motor_Right,-Drivespeed_middle/2);
			camera_update();
		} else if(get_object_center(0,0).x < 70) {
			motor(Motor_Left,-Drivespeed_middle/2);
			motor(Motor_Right,Drivespeed_middle/2);
			camera_update();
		} else {
			motor(Motor_Left,Drivespeed_middle/2);
			motor(Motor_Right,Drivespeed_middle/2);
			camera_update();
		}
		msleep(5);
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
	claw_close();
	msleep(1000);
	claw_up();
}
void bringback2cube(){
	//claw up
	motor(Motor_Up,Motor_up_speed);
	//zur�ck 1s 
	drive(1000,-Drivespeed,-Drivespeed);
	//turn right 90
	drive(970,Turnspeed,-Turnspeed);
	//vor to calibrate
	drive(1000,Drivespeed,Drivespeed);
	//back
	drive(500,-Drivespeed,-Drivespeed);
	//turn so little bit b4 cubes
	drive(1130,Turnspeed,-Turnspeed);
	//vor
	drive(900,Drivespeed,Drivespeed);
	//wait for claw up
	claw_up();
	//camera fix
	camera_update();
	//hide your cubes
	cube_is_near();
	//back
	drive(1000,-Drivespeed,-Drivespeed);
	//turn left more than 90 idk
	drive(1330,-Turnspeed,Turnspeed);
	//start to down motor
	motor(Motor_Up,Motor_down_speed);
	//vor to calibrate
	drive(1000,Drivespeed,Drivespeed);
	
	//langsamer weil wackeldackel
	drive(2000,Drivespeed_middle+20,Drivespeed_middle+20);
	//back
	drive(400,-Drivespeed,-Drivespeed);
	//turn more than 90 lulz
	drive(980,-Turnspeed,Turnspeed);
	//light left and shit
	drive(5000,Drivespeed_middle*2,(Drivespeed_middle*2)-7);
	//light back and shit
	drive(1000,-Drivespeed_middle*2,(-Drivespeed_middle*2)+7);
	//wait for claw down
	while(!digital(Sensor_Down)){
		if(seconds()>start+113){
			claw_open();
			disable_servos();
		}
	}
	freeze(Motor_Up);
	msleep(2000);
	claw_open();
	disable_servos();
}
void bringback(){
	//zur�ck zur linie
	drive_till_line_backward();
	
	//vor damit nicht insert into create
	drive(1200,Drivespeed_middle,Drivespeed_middle);
	
	//turn left 90
	//drive(1100,-1*Drivespeed_middle-20,Drivespeed_middle+20);
	
    motor(Motor_Left,-Drivespeed_middle*2);
    while(analog(Sensor_Line_Left) < Sensor_Black){}
    msleep(400);
    while(analog(Sensor_Line_Left) > Sensor_Black){}
    msleep(100);
    freeze(Motor_Left);
	
	//start claw down
	motor(Motor_Up,Motor_down_speed);
	
	//gerade but leicht links so it twerks
	drive(9000,Drivespeed,Drivespeed);
	
	wisch();
	//vor to calibrate
	drive(5000,Drivespeed_middle+20,Drivespeed_middle+20);
	
	//back
	drive(400,-Drivespeed,-Drivespeed);
	
	//turn more than left 90
	drive(980,-Turnspeed,Turnspeed);
	
	//leicht rechts to calibrate
	drive(5000,Drivespeed_middle*2,(Drivespeed_middle*2)-7);
	
	//leicht back rechts to calibrate && w�rfel fit in tube
	drive(800,-Drivespeed_middle*2,(-Drivespeed_middle*2)+7);
	
	//wait for claw down
	while(!digital(Sensor_Down)){}
	freeze(Motor_Up);
	
	msleep(2000);
	claw_open();
	msleep(2000);
}
void drive_till_line_backward(){
	motor(Motor_Left,-Drivespeed_middle);
	motor(Motor_Right,-Drivespeed_middle);			
	printf("Start\n");
	while(1){
		if(analog(Sensor_Line_Left)>Sensor_Black&&analog(Sensor_Line_Right)>Sensor_Black){
			printf("Abgehn\n");
			break;
		}
		if(analog(Sensor_Line_Left)>Sensor_Black){
			freeze(Motor_Right);
			freeze(Motor_Left);
			printf("Links\n");
			motor(Motor_Right,-Drivespeed_middle);
			msleep(50);
			while(analog(Sensor_Line_Right)<Sensor_Black){}
			break;
		}
		if(analog(Sensor_Line_Right)>Sensor_Black){
			freeze(Motor_Right);
			freeze(Motor_Left);
			printf("Right\n");
			motor(Motor_Left,-Drivespeed_middle);
			msleep(50);
			while(analog(Sensor_Line_Left)<Sensor_Black){}
			break;
		}
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
}
