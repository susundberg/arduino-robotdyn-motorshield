// 
// This example code works with "ROBOTDYN" shield "Motorshield-L298P/2A/2M" 
// 
// Open serial port and and use +X and -X values to control the speed of the motor
//

static const int PIN_LED=13;

static const int PIN_MOTOR_ENA_A = 8;
static const int PIN_MOTOR_ENA_B = 9;
static const int PIN_MOTOR_SPEED = 10;

void set_motor_state( int value )
{
   int ena_a = 0;
   int ena_b = 0;
   if ( value == 0 )
   {
     Serial.write("Set motor stop\n");
   } 
   else
   {
     Serial.write("Set motor speed ");
     

     if ( value < 0 ) 
     {
       ena_b = 1;
       ena_a = 0;
       value = -value;
       Serial.print("R ");
     }
     else
     {
       Serial.print("F ");
       ena_b = 0;
       ena_a = 1;
     }
     
     Serial.print(value);
     Serial.write("\n");
   }
   digitalWrite(PIN_MOTOR_ENA_A, ena_a ); 
   digitalWrite(PIN_MOTOR_ENA_B, ena_b ); 
   analogWrite(PIN_MOTOR_SPEED, value );
}



void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT); 
  pinMode(PIN_MOTOR_ENA_A, OUTPUT); 
  pinMode(PIN_MOTOR_ENA_B, OUTPUT); 

  set_motor_state( 0 );  
  Serial.write("***********************************\n");  
  Serial.write("Welcome to motor shield tester!\n");
  Serial.write("***********************************\n");
  Serial.write( ">" );
}


bool process_number( const char* buffer, int buffer_n, int* convert )
{
    char* end_ptr;
    int value = strtol( (const char*)buffer, &end_ptr, 10  );
    if ( end_ptr != buffer + buffer_n )
    {
        Serial.write(" Invalid: '" );
        Serial.write( (const char*)buffer);
        Serial.write("'\n"); 
        return false;
    }
    
    *convert = value;
    return true;
}

char* receive_serial( int* buffer_len)
{
  static char buffer[64];
  static int loop = 0;
  
  // if there's any serial available, read it:
  while (Serial.available() > 0) 
  {
    // do it again:
    buffer[ loop ] = Serial.read();

    if (buffer[loop] == '\n')
    {
      buffer[loop] = 0x00;
      *buffer_len = loop;
      loop = 0;
      return buffer;
    }

    loop += 1;
    if ( loop >= 64 )
    {
      Serial.write("E: Too long line\n");
      loop = 0;
    }
    
  }
  return NULL;
}


void loop() {
  
  static bool state = 0;
  digitalWrite(PIN_LED, state);
  
  
  int   input_n = 0;
  char* input = receive_serial( &input_n );
  
  if ( input == NULL )
     return;
  
  Serial.write(">");
  Serial.write(input);
  Serial.write("\n");
  
  if ( input_n == 0 )
    return;
  
  int value;   
  if ( process_number(input, input_n, &value ) == true )
  {
     // We got proper number, set motor output
     set_motor_state( value );
  }
  state = !state;
  
}








