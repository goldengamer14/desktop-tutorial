const int led = D1;
const int IR = D2;

void setup()
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(IR, INPUT);
}

void loop()
{
  int sensor = digitalRead(IR);
  if (sensor == LOW)
  {
    digitalWrite(led, HIGH);
    Serial.println("HIGH");
  }
  else
  {
    digitalWrite(led, LOW);
    Serial.println("LOW");
  }
  delay(1000);
}

