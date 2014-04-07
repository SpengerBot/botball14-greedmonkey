void start_position();
void watch_for_cube();
void calibrate();
void cube_is_near();
void take_position();
void claw_up();
void claw_littlebit_up();
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

//Motors
#define Motor_Left 0
#define Motor_Right 2
#define Motor_Up 3
#define Turnspeed 70
#define Drivespeed 100
#define Drivespeed_slow 5
#define Drivespeed_middle 40
//Servos
#define	Servo_Left 0
#define Servo_Right 1
#define Servo_Back 3

#define Servo_Back_Up 1450
#define Servo_Back_Down 10
#define Servo_Left_Open 1550
#define Servo_Left_Closed 970
#define Servo_Right_Open 960
#define Servo_Right_Closed 1390
//Button
#define Button_Up 14

//Sensor + Wert
#define Sensor_Up 1
#define Sensor_Up_Value 700
#define Sensor_Line_Left 0
#define Sensor_Line_Right 6
#define Sensor_Black 900
#define Sensor_Down 7
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

	enable_servos();
	set_servo_position(Servo_Back, Servo_Back_Up);	
	calibrate();
	
	//wait for light
	printf("wait for light oida");
	set_b_button_text("I am the Twilight");
	while(!b_button()){}
	//wait_for_light(Sensor_Light);
	
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
	while(analog(Sensor_Down)> Sensor_Down_Value){}
	freeze(Motor_Up);
	//wait so it doesn't fuck up
	msleep(500);
	//up for straffes seil
	motor(Motor_Up,Motor_up_speed);
	while(analog(Sensor_Down)< Sensor_Down_Value){}
	freeze(Motor_Up);
	claw_close();
}

void start_position(){	
	//start taking claws up
	motor(Motor_Up,Motor_up_speed);
	//90 links
	drive(990,-Turnspeed,Turnspeed);
	//gegen wand für kalibration
	drive(450,Drivespeed,Drivespeed);
}
void drive(int delay,int speed_left, int speed_right){
	motor(Motor_Left,speed_left);
	motor(Motor_Right,speed_right);
	msleep(delay);
	freeze(Motor_Left);
	freeze(Motor_Right);
	msleep(200);
}
void take_position() {	
	//zurück 2sec
	drive(1700,-Drivespeed,-Drivespeed);
	//90 nach rechts
	drive(990,Turnspeed,-Turnspeed);
	//camera fix
	camera_update();
	//oida drive
	drive(1500,Drivespeed,Drivespeed);
	//90 nach rechts
	drive(990,Turnspeed,-Turnspeed);
	drive_till_line();
}
void drive_till_line(){
	motor(Motor_Left,Drivespeed);
	motor(Motor_Right,Drivespeed);
	printf("Start\n");
	while(1){
		if(analog(Senor_Line_Left)>Sensor_Black&&analog(Senor_Line_Right)>Sensor_Black){
			printf("Abgehn\n");
			break;
		} 
		if(analog(Senor_Line_Left)>Sensor_Black) {
			freeze(Motor_Left);
			printf("Links\n");
			motor(Motor_Right,Turnspeed);
			msleep(50);
			while(analog(Senor_Line_Right)<Sensor_Black){}
			break;
		}
		if(analog(Senor_Line_Right)>Sensor_Black) {
			freeze(Motor_Right);
			printf("Right\n");
			motor(Motor_Left,Turnspeed);
			msleep(50);
			while(analog(Senor_Line_Left)<Sensor_Black){}
			break;
		}
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
	//fahr back so claws doesnt break
	msleep(500);
	drive(500,-Drivespeed_middle,-Drivespeed_middle);
	
	while(analog(Sensor_Up) > Sensor_Up_Value){}
	freeze(Motor_Up);
	
}

void claw_close(){
	set_servo_position(Servo_Left,Servo_Left_Closed);
	set_servo_position(Servo_Right,Servo_Right_Closed);
}
void claw_drop(){
	set_servo_position(Servo_Left,Servo_Left_Open);
	set_servo_position(Servo_Right,Servo_Right_Closed);
}
void claw_open(){
	set_servo_position(Servo_Left,Servo_Left_Open);
	set_servo_position(Servo_Right,Servo_Right_Open);
}
void claw_up(){
	motor(Motor_Up,Motor_up_speed);
	while(analog(Sensor_Up) > Sensor_Up_Value){}
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
	while(analog(Sensor_Down)>Sensor_Down_Value){}
	freeze(Motor_Up);
	//wait so it doesn't fuck up
	msleep(500);
	//up for straffes seil
	motor(Motor_Up,Motor_up_speed);
	while(analog(Sensor_Down)< Sensor_Down_Value){}
	freeze(Motor_Up);
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
			if(seconds()>s+2){
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
			if(seconds()>s+4){
				printf("zeit links\n");
				freeze(Motor_Right);
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
	while(get_object_bbox(0,0).width < 85){
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
		msleep(10);
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
	claw_close();
	msleep(1000);
	claw_littlebit_up();
}
void bringback2cube(){
	//claw up
	motor(Motor_Up,Motor_up_speed);
	//zurück 1s 
	drive(1000,-Drivespeed,-Drivespeed);
	//turn right 90
	drive(990,Turnspeed,-Turnspeed);
	//vor to calibrate
	drive(1000,Drivespeed,Drivespeed);
	//back
	drive(500,-Drivespeed,-Drivespeed);
	//turn so little bit b4 cubes
	drive(1350,Turnspeed,-Turnspeed);
	//vor
	drive(900,Drivespeed,Drivespeed);
	//wait for claw up
	while(analog(Sensor_Up) > Sensor_Up_Value){}
	freeze(Motor_Up);
	//camera fix
	camera_update();
	//hide your cubes
	cube_is_near();
	//back
	drive(1000,-Drivespeed,-Drivespeed);
	//turn left more than 90 idk
	drive(1350,-Turnspeed,Turnspeed);
	//start to down motor
	motor(Motor_Up,Motor_down_speed);
	//vor to calibrate
	drive(4000,Drivespeed_middle,Drivespeed_middle);
	//back
	drive(250,-Drivespeed,-Drivespeed);
	//turn more than 90 lulz
	drive(975,-Turnspeed,Turnspeed);
	//light left and shit
	drive(6000,Drivespeed_middle*2,(Drivespeed_middle*2)-7);
	//light back and shit
	drive(1000,-1*Drivespeed_middle*2,(-1*Drivespeed_middle*2)+5);
	//put your bots up in the air
	while(get_servo_position(Servo_Back) > Servo_Back_Down){
		set_servo_position(Servo_Back,get_servo_position(Servo_Back)-10);
		msleep(20);
	}
	//wait for claw down
	while(analog(Sensor_Down)>Sensor_Down_Value){
		if(seconds()>start+111){
			claw_open();
		}
	}
	freeze(Motor_Up);
	msleep(2000);
	claw_open();
}
void bringback(){
	//zurück zur linie
	drive_till_line_backward();
	
	//vor damit nicht insert into create
	drive(1200,Drivespeed_middle,Drivespeed_middle);
	
	//turn left 90
	//drive(1100,-1*Drivespeed_middle-20,Drivespeed_middle+20);
	
    motor(Motor_Left,-Drivespeed_middle*2);
    while(analog(Sensor_Line_Left) < Sensor_Black){
    printf("wait for black: %d\n",analog(Sensor_Line_Left));
    }
    msleep(400);
    while(analog(Sensor_Line_Left) > Sensor_Black){
    printf("wait for white: %d\n",analog(Sensor_Line_Left));}
    msleep(400);
    freeze(Motor_Left);
	
	//start claw down
	motor(Motor_Up,Motor_down_speed);
	
	//gerade but leicht links so it twerks
	drive(10000,Drivespeed,Drivespeed);
	
	drive(3000,Drivespeed_middle,Drivespeed_middle);
	
	//back
	drive(250,-Drivespeed,-Drivespeed);
	
	//turn more than left 90
	drive(970,-Turnspeed,Turnspeed);
	
	//leicht rechts to calibrate
	drive(5000,Drivespeed_middle*2,(Drivespeed_middle*2)-7);
	
	//leicht back rechts to calibrate && würfel fit in tube
	drive(1000,-Drivespeed_middle*2,(-Drivespeed_middle*2)+7);
	
	//put the bot up in the air
	while(get_servo_position(Servo_Back) > Servo_Back_Down){
		set_servo_position(Servo_Back,get_servo_position(Servo_Back)-10);
		msleep(20);
	}
	
	//wait for claw down
	while(analog(Sensor_Down)>Sensor_Down_Value){}
	freeze(Motor_Up);
	
	msleep(2000);
	claw_open();
	msleep(2000);
	
	while(get_servo_position(Servo_Back) < Servo_Back_Up){
		set_servo_position(Servo_Back,get_servo_position(Servo_Back)+10);
		msleep(20);
	}
}
void drive_till_line_backward(){
	motor(Motor_Left,-Drivespeed_middle);
	motor(Motor_Right,-Drivespeed_middle);			
	printf("Start\n");
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
	}
	freeze(Motor_Left);
	freeze(Motor_Right);
}
