void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  for (int i = 0; i < 6; ++i){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(i*500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(i*500);
  }

}