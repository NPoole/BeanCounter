/***************************************************************
 * 
 ***************************************************************/

// TODO
// - PITCH IN MM (1,2,4,8,12,16)
// - START/STOP/RESET

#define DIGIT1_CATHODE 8
#define DIGIT2_CATHODE 9
#define DIGIT3_CATHODE 10
#define IRLED 11
#define STARTBTN 12
#define SETBTN 13
#define IRSENSOR 14

#define SCANDELAY 500

byte state_previous = 0b00;
byte state_a = 0b01;
byte state_b = 0b10;

String digit1 = "0000000";
String digit2 = "0000000";
String digit3 = "0000000";

// DEVICE MODE
// 0 - IDLE/STOP
// 1 - COUNTING
// 2 - RESET
// 3 - SET PITCH
byte deviceMode = 0;
byte returnMode;

byte pitch = 4;
float ratio = 1.0;

int displayBuf = 0;
int count = 0;
byte numPos[10] = {0,0,0,0,0,0,0,0,0,0};

String segmentLUT[] = {
  "1110111",
  "0100100",
  "1011101",
  "1011011",
  "0111010",
  "1101011",
  "1101111",
  "1010010",
  "1111111",
  "1111010"
};

void setup() {
  // put your setup code here, to run once:

  for(int i = 0; i < 12; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, 0);
  }

  for(int i = 12; i < 14; i++){
    pinMode(i, INPUT_PULLUP);
  }

  pinMode(A9, INPUT);
  pinMode(A6, INPUT);
  digitalWrite(IRLED, 1);

}

void loop() {

  
  switch(deviceMode){
    case 0:
      mode0();
      break;
    case 1:
      mode1();
      break;
    case 2:
      mode2();
      break;
    case 3:
      mode3();
      break;
    default:
      deviceMode = 0;
      break;
  }
  

  /* ADC TEST MODE 
  translateSegments(analogRead(A9));
  updateLED();  
  */
}

void mode0(){

  if(digitalRead(STARTBTN) == 0){
    deviceMode = 2;
    returnMode = 1;
    return;
  }
  digitalWrite(IRLED, 0);
  
  while(deviceMode == 0){

    updateLED();
    
    if(digitalRead(STARTBTN) == 0){
      delay(500);
      deviceMode = 1;
    }
    
    if(digitalRead(SETBTN) == 0){
      delay(500);
      deviceMode = 3;
      returnMode = 0;
    }
    
    translateSegments(displayBuf);    
    
  }
}

void mode1(){

  if(digitalRead(STARTBTN) == 0){
    deviceMode = 2;
    returnMode = 0;
    return;
  }

  digitalWrite(IRLED, 1);
  
  while(deviceMode == 1){
  
    updateLED();
  
    if(digitalRead(STARTBTN) == 0){
      delay(500);
      deviceMode = 0;
    }
    
    if(digitalRead(SETBTN) == 0){
      delay(500);
      deviceMode = 3;
      returnMode = 1;
    }
  
    translateSegments(displayBuf);
    
  }
}

void mode2(){
  while(deviceMode == 2){
    for(int i = 0; i < 100; i++){
      updateLED();
    }
    delay(300);
    if(digitalRead(STARTBTN) == 1){
      count = 0;
      displayBuf = 0;
      deviceMode = returnMode;
    }
  }
}

void mode3(){
  
  while(deviceMode == 3){
    if(digitalRead(STARTBTN) == 0){
      delay(200);
      switch(pitch){
        case 1:
          pitch = 2;
          break;
        case 2:
          pitch = 4;
          break;
        case 4: 
          pitch = 8;
          break;
        case 8:
          pitch = 12;
          break;
        case 12:
          pitch = 16;
          break;
        case 16:
          pitch = 1;
          break;       
      }
      ratio = 4.0/int(pitch);
    }
    if(digitalRead(SETBTN) == 0){
      delay(200);
      deviceMode = returnMode;
      translateSegments(0);
    }
    translateSegments(pitch);
    updateLED();  
  }
}

void translateSegments(int displayBuf){

  numPositions(displayBuf);
  digit3 = segmentLUT[numPos[0]];
  digit2 = segmentLUT[numPos[1]];
  digit1 = segmentLUT[numPos[2]];
  
}

void updateLED(){

  for(int i = 0; i < 7; i++){
    digitalWrite(i, digit1.charAt(i) == '1');
  }
  
  digitalWrite(DIGIT1_CATHODE, 1);
  if(deviceMode==1){updateCount();}else{delayMicroseconds(SCANDELAY);}
  digitalWrite(DIGIT1_CATHODE, 0);
  
  for(int i = 0; i < 7; i++){
    digitalWrite(i, digit2.charAt(i) == '1');
  }

  digitalWrite(DIGIT2_CATHODE, 1);
  if(deviceMode==1){updateCount();}else{delayMicroseconds(SCANDELAY);}
  digitalWrite(DIGIT2_CATHODE, 0);
  
  for(int i = 0; i < 7; i++){
    digitalWrite(i, digit3.charAt(i) == '1');
  }

  digitalWrite(DIGIT3_CATHODE, 1);
  if(deviceMode==1){updateCount();}else{delayMicroseconds(SCANDELAY);}
  digitalWrite(DIGIT3_CATHODE, 0);
  
}

void numPositions(int num){
  
    for(int i = 0; i < 10; i++){
      numPos[i] = 0;
    }
    byte pos = 0;
    while(num != 0){
      numPos[pos] = num % 10;
      num = num / 10;
      pos++;
    }
    return;
}

void updateCount(){

  int adc_a = analogRead(A9);
  int adc_b = analogRead(A6);
  if(adc_a > 150){state_a = 0b01;
  }else if(adc_a < 100){state_a = 0b00;}
  if(adc_b > 150){state_b = 0b10;
  }else if(adc_b < 100){state_b = 0b00;}
  
  byte state_current = state_a + state_b;
  //displayBuf = state_current;

  if(state_current != state_previous){
    if(state_previous == 0b11){
      switch(state_current){
        case 0b01:
          count++;
          displayBuf = count * ratio;
          break;
        case 0b10:
          count--;
          displayBuf = count * ratio;
          break;
      }
    }
    state_previous = state_current;
  }
}
