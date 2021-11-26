/***************************************************************
 * THIS CODE IS BAD
 * 
 * DO NOT USE IT, IT IS FOR REFERENCE ONLY. IT WAS WRITTEN 
 * HURRIEDLY AND WITH MANIC FERVOR IN THE DEVIL'S HOUR AS A 
 * SO THAT I COULD POST A PROOF-OF-CONCEPT VIDEO ON BIRDSITE.
 * 
 * I MUST URGE YOU IN THE STRONGEST POSSIBLE TERMS NOT TO SHARE
 * YOUR CONCERN OR OPINION OF THIS CODE WITH ME OR ANYONE ELSE
 * AS I HAVE DISCLAIMED IT HEREIN AS BAD AND WRONG. 
 * 
 * THANK YOU
 */

#define SEG1 1
#define SEG2 2
#define SEG3 3
#define SEG4 4 
#define SEG5 5 
#define SEG6 6
#define SEG7 7
#define DIGIT1 8
#define DIGIT2 9
#define DIGIT3 10
#define IRLED 11
#define STARTBTN 12
#define SETBTN 13
#define IRSENSOR 14

#define SCANDELAY 1

boolean prevOpt = 0;
boolean curOpt = 1;

String digit1 = "0000000";
String digit2 = "0000000";
String digit3 = "0000000";

int count = 0;
byte numPos[10] = {0,0,0,0,0,0,0,0,0,0};
String digitBuffer = "0000000";

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
  digitalWrite(IRLED, 1);

}

void loop() {
  // put your main code here, to run repeatedly:

  updateLED();
  //translateSegments(analogRead(A9)/4);


  if(digitalRead(STARTBTN) == 0){
    count=0;
    delay(100);
    translateSegments(count);
  }
  if(digitalRead(SETBTN) == 0){
    count = 0;
    delay(100);
    translateSegments(count);
  }
  
  
  curOpt = 0;
  if(analogRead(A9) > 600){
    curOpt = 1;
  }
  
  if(curOpt!= prevOpt){
    if(curOpt == 1){
      count++;
    }
    prevOpt = curOpt;
  }

  translateSegments(count);

}

void translateSegments(int count){

  numPositions(count);
  digit3 = segBuf(numPos[0]);
  digit2 = segBuf(numPos[1]);
  digit1 = segBuf(numPos[2]);
  
}

String segBuf(byte num){
  String str;
  switch(num){
    case 0:
      str = "1110111";
      break;
    case 1:
      str = "0100100";
      break;
    case 2:
      str = "1011101";
      break;
    case 3: 
      str = "1011011";
      break;
    case 4: 
      str = "0111010";
      break;
    case 5:
      str = "1101011";
      break;
    case 6:
      str = "1101111";
      break;
    case 7:
      str = "1010010";
      break;
    case 8:
      str = "1111111";
      break;
    case 9:
      str = "1111010";
      break;
    default:
      str = "0000000";
      break;
  }
  return str;
}

void updateLED(){

  for(int i = 0; i < 7; i++){
    digitalWrite(i, digit1.charAt(i) == '1');
  }
  
  digitalWrite(DIGIT1, 1);
  delay(SCANDELAY);
  digitalWrite(DIGIT1, 0);
  
  for(int i = 0; i < 7; i++){
    digitalWrite(i, digit2.charAt(i) == '1');
  }

  digitalWrite(DIGIT2, 1);
  delay(SCANDELAY);
  digitalWrite(DIGIT2, 0);
  
  for(int i = 0; i < 7; i++){
    digitalWrite(i, digit3.charAt(i) == '1');
  }

  digitalWrite(DIGIT3, 1);
  delay(SCANDELAY);
  digitalWrite(DIGIT3, 0);
  
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
