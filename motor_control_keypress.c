/*
Simple dc motor control using keyboard
presses to go forward left and right, brake and reverse
*/


/*
main function to initialize device
and check for exiting condition
switch states for relevant key presses
*/
//includes:
#include <rc_usefulincludes.h>
#include <roboticscape.h>

int main()
{
  char input;
  //initialize cape library
  prinft("Hello BeagleBone \n");
  rc_set_state(RUNNING);
  //bring h-bridges to standby
  rc_enable_motors();
  rc_set_led(GREEN,ON);
  rc_set_led(RED,ON);
  //set LEDS on
  rc_set_motor_free_spin(1);
  rc_set_motor_free_spin(2);
  printf("Motors are now ready \n");
  //turn on terminal to get a single character input
  system("stty raw");
  do {
    printf("> ");
    input = getchar();
    switch(input){
      case 'w': //go forward
      rc_set_motor(1,0.5);
      rc_set_motor(2,0.5);
      break;
      case 'd': //turn right
      rc_set_motor(1,0.5);
      rc_set_motor(2,-0.5);
      break;
      case 'a': //turn left
      rc_set_motor(1,-0.5);
      rc_set_motor(2,0.5);
      break;
      case 's': //go backwards
      rc_set_motor(1,-0.5);
      rc_set_motor(2,-0.5);
      break;
      case 'f': //go to a fullstop
      rc_set_motor_brake_all();
      break;
      case 'q' //QUIT (disables motors)
      rc_disable_motors();
      break;
      default:
      printf("Invalid character \n");

    }
  }
  while(input != 'q');
  printf("Done \n");
  rc_cleanup();
  system("ssty cooked");
  return 0;


}
