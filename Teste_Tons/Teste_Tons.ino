const int buzzer = 6;
const int buttons[] = {2,3,4,5};
void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  for(int i = 0; i< sizeof(buttons)/ sizeof(buttons[0]); i++){
    pinMode(buttons [i], INPUT_PULLUP);
  }
}
void loop() {

  for(int i = 0; i < sizeof(buttons)/ sizeof(buttons[0]);i++){
    if(digitalRead(buttons[i]) == LOW){
      int tono = 110 * (i + 1);
      tone(buzzer,tono,200 );
  }
}
}
