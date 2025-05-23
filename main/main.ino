int pinF = A0;
int pinFR = A1;
int pinR = A2;
int pinBR = A3;
int pinB = A4;
int pinBL = A5;
int pinL = A6;
int pinFL = A7;

int pinButton = 2;

int pinRMotor = 4;
int pinLMotor = 5;

int click = false;
bool isRunning = true;
bool mode = true;

int minDiff = 150;
int padding = 50;
int speedL = 150;
int speedR = 150;

bool light(int f, int fr, int r, int br, int b, int bl, int l, int fl) {
  int maxLight = max(max(max(f, fr), max(r, br)), max(max(b, bl), max(l, fl)));
  int minLight = min(min(min(f, fr), min(r, br)), min(min(b, bl), min(l, fl)));

  return (maxLight - minLight) > minDiff;
}

void stayStill() {
  analogWrite(pinRMotor, 0);
  analogWrite(pinLMotor, 0);
}

void turnRight() {
  analogWrite(pinRMotor, 0);
  analogWrite(pinLMotor, speedL);
}

void turnLeft() {
  analogWrite(pinRMotor, speedR);
  analogWrite(pinLMotor, 0);
}

void goStraight() {
  analogWrite(pinRMotor, speedR);
  analogWrite(pinLMotor, speedL);
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(pinF, INPUT);
  pinMode(pinFR, INPUT);
  pinMode(pinR, INPUT);
  pinMode(pinBR, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinBL, INPUT);
  pinMode(pinL, INPUT);
  pinMode(pinFL, INPUT);

  //pinMode(pinButton, INPUT_PULLUP);

  pinMode(pinRMotor, OUTPUT);
  pinMode(pinLMotor, OUTPUT);
}

void loop() {
  /*
  bool button = digitalRead(pinButton);
  if (button == HIGH && !click) {
    isRunning = !isRunning;
    click = true;
  }
  else if (button == LOW) {
    click = false;
  }
  */

  if (Serial1.available()) {
    char cmd = Serial1.read();
    if (cmd == '1') {
      isRunning = true;
    }
    else if (cmd == '2') {
      isRunning = false;
    }
    else if (cmd == '3') {
      mode = true;
    }
    else if (cmd == '4') {
      mode = false;
    }
  }

  if (!isRunning) {
    stayStill();
    Serial.println("off");
    Serial.println(mode);
    return;
  }

  int f = analogRead(pinF);
  int fr = analogRead(pinFR);
  int r = analogRead(pinR);
  int br = analogRead(pinBR);
  int b = analogRead(pinB);
  int bl = analogRead(pinBL);
  int l = analogRead(pinL);
  int fl = analogRead(pinFL);

/*
  Serial.println(f);
  Serial.println(fr);
  Serial.println(r);
  Serial.println(br);
  Serial.println(b);
  Serial.println(bl);
  Serial.println(l);
  Serial.println(fl);
*/

  if (!light(f, fr, r, br, b, bl, l, fl)) {
    stayStill();
    Serial.println("still");
    Serial.println(mode);
    return;
  }

  int front = f;
  int back = b;
  int left = (fl + l + bl)/3;
  int right = (fr + r + br)/3;

  if ((front < back && mode) || (front > back && !mode)) {
    if ((left > right && mode) || (left < right && !mode)) {
      turnLeft();
      Serial.println("left turn");
    }
    else {
      turnRight();
      Serial.println("right turn");
    }
  }
  else {
    if (abs(left - right) < padding) {
      goStraight();
      Serial.println("straight");
    }
    else if ((left > right && mode) || (left < right && !mode)) {
      turnLeft();
      Serial.println("left");
    }
    else {
      turnRight();
      Serial.println("right");
    }
  }

  Serial.println(mode);
}
