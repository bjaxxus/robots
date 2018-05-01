#define I2C_BUS 1
#define I2C_addr 0x70
#define I2C_addr1 0x75
//includes:
#include <rc_usefulincludes.h>
#include <roboticscape.h>
#include <stdio.h>

//global variables:
uint8_t lowbyte;
uint8_t highbyte;
uint8_t centimeterMeas = 0x51;
uint8_t a;
uint8_t b;
rc_imu_data t data;
void process_data();
double angle;
int distance;
int turn;


int combinedmessage;

int main(int argc, char** argv) {
  rc_initialize();
  angle = atof(argv[1]);
  if(angle > 0)
  {
    turn = 1;
  }
  else {
    turn = 0;
  }
  distance = atoi(argv[2]);
  printf("------Initializing complete------ \n");
  printf("------Setup I2C------ \n");
  rc_i2c_init(I2C_BUS,I2C_addr);
  rc_i2c_init(I2C_BUS,I2C_addr1);
  printf("------Setup Motors------ \n");
  rc_enable_motors();
  if(rc_enable_motors()) return -1;
  rc_set_state(RUNNING);
  rc_set_led(GREEN,ON);
  rc_set_led(RED,ON);
  rc_set_motor_free_spin(1);
  rc_set_motor_free_spin(2);
  printf("------Motors ready------ \n")
  rc_imu_config_t conf = rc_default_imu_config();
  conf.dmp_sample_rate = 20;
  conf.enable_magnetometer = 1;
  //setup interrupt for dmp operation
  if(rc_initialize_imu_dmp(&data,conf)){
    printf("rc_initialize_imu_dmp failed \n");
    return -1;
  }
  rc_set_imu_interrupt_func(&process_data);
  //turn the device
  if(turn){
    rc_set_motor(1,0.3);
    rc_set_motor(2,-0.3);

  }
  else{
    rc_set_motor(1,-0.3);
    rc_set_motor(2,0,3);
  }
  //wait now:
  while(rc_get_state()!=EXITING){
    rc_usleep(100000);
  }
  int movement = 0;
  while(movement < distance){
    rc_set_motor(1,0.2);
    rc_set_motor(2,0.2);
    rc_usleep(1000000);
    movement++;
  }
  rc_set_motor_brake_all();


  rc_i2c_set_device_address(I2C_BUS, I2C_addr);
  rc_i2c_write_bytes(I2C_BUS,0,1,&centimeterMeas);
  rc_usleep(100000);
  a = rc_i2c_read_bytes(I2C_BUS,2,1,&highbyte);
  b = rc_i2c_read_bytes(I2C_BUS,3,1,&lowbyte);
  combinedmessage = (highbyte<<8)|lowbyte;
  printf("Measurement:   %i \n");
  rc_i2c_set_device_address(I2C_BUS, I2C_addr1);
  rc_i2c_write_bytes(I2C_BUS,0,1,&centimeterMeas);
  rc_usleep(100000);
  a = rc_i2c_read_bytes(I2C_BUS,2,1,&highbyte);
  b = rc_i2c_read_bytes(I2C_BUS,3,1,&lowbyte);
  combinedmessage = (highbyte<<8)|lowbyte;
  printf("Measurement:   %i \n");


  rc_i2c_close(1);
  rc_cleanup();
  return 0;

}

void process_data() {
  printf("r");
  printf(" ");
  printf("Angle = %6.1fn",angle);
  printf("Distance = %2dn",distance);
  printf(" %6.1f |", data.compass_heading_raw*RAD_TO_DEG);
  printf(" %6.1f |", data.compass_heading*RAD_TO_DEG);
  if (turn)
  {
    if ((angle - data.compass_heading*RAD_TO_DEG) < 1.0)
    {
      rc_set_motor_brake_all();
      rc_set_state(EXITING);
    }
  }
  else
  if ((-angle + data.compass_heading*RAD_TO_DEG) < 1.0)
  {
    rc_set_motor_brake_all();
    rc_set_state(EXITING);
  }
  fflush(stdout);
  return;
  }
}
