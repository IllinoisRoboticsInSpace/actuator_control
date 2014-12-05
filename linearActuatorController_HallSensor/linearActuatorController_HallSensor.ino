//Pin Settings for Input Pins
byte retractedLimitPin = 1;
byte extendedLimitPin = 2;
byte hallSensorPin = 3;
byte autonPositionPin = 4;//Analog Pin
byte autonHoldPin = 5;
byte autonBrakePin = 6;

//Pin Settings for Output Pins
byte motorBrakePin = 7;
byte motorDirectionPin = 8;
byte motorSpeedPin = 9;

//The Actual Position of the Actuator
int actuatorPosition = 0;

//The Desired Position of the Actuator
int desiredPosition = 5;

//The Speed the Motor will be Spinning at
byte constantMotorSpeed = 128;

//The Speed of the Motor When it shouldn't be Moving
byte motorStopSpeed = 0;

//Length of the Actuator in Hall Rotations
int hallActuatorLength = 8192;

//The Direction the Actuator is Moving In
//MotorDirection: true = extend, false = retract
boolean motorDirection = true;

void setup()
{
  //Set up Input Pins
  pinMode(retractedLimitPin, INPUT);
  pinMode(extendedLimitPin, INPUT);
  pinMode(hallSensorPin, INPUT);
  pinMode(autonHoldPin, INPUT);
  pinMode(autonBrakePin, INPUT);
  
  //Motor Brake:  LOW = Motor Operating, HIGH = Motor Stopped
  //Output Pin to Cut Power to Motor
  pinMode(motorBrakePin, OUTPUT);
  
  //Motor Direction:  LOW = Retracting, HIGH = Extending
  //Output Pin to Control H-Bridge Direction
  pinMode(motorDirectionPin, OUTPUT);
  
  //PWM Output Pin to Control Motor Speed
  pinMode(motorSpeedPin, OUTPUT);
  
  //Auto-Zeros the Motor, Ensuring Accurate Positioning
  while(digitalRead(retractedLimitPin) == false)
  {
    digitalWrite(motorDirectionPin, LOW);
    analogWrite(motorSpeedPin, constantMotorSpeed);
  }
  
  digitalWrite(motorDirectionPin, HIGH);
  analogWrite(motorSpeedPin, motorStopSpeed);
  actuatorPosition = 0;
}

void loop()
{
  //Makes the motor move in the correct direction
  digitalWrite(motorDirectionPin, motorDirection);
  
  //Disconnects Power to the Motor is Necessary
  if(digitalRead(autonBrakePin) == HIGH)
    digitalWrite(motorBrakePin, HIGH);
  else
    digitalWrite(motorBrakePin, LOW);
  
  //Records the change in position of the actuator
  if(digitalRead(hallSensorPin) == HIGH && motorDirection == true)
    actuatorPosition++;
  else if(digitalRead(hallSensorPin) == HIGH && motorDirection == false)
    actuatorPosition--;
    
  //Changes the desired position of the actuator, when required
  if(digitalRead(autonHoldPin) == LOW)
    desiredPosition = analogRead(autonPositionPin) * (hallActuatorLength / 1023);
  
  //Changes the direction and speed of the actuator, if necesary
  if(desiredPosition < actuatorPosition && digitalRead(retractedLimitPin) == LOW) //
  {
    motorDirection = false;
    analogWrite(motorSpeedPin, constantMotorSpeed);
  }
  else if (desiredPosition > actuatorPosition && digitalRead(extendedLimitPin) == LOW) //Extending
  {
    motorDirection = true;
    analogWrite(motorSpeedPin, constantMotorSpeed);
  }
}
