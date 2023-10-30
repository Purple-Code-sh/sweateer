
int startPin = 2;

int side_L = 10;
int front_L = 8;
int front_R = 4;
int side_R = 7;

int motorR_1 = 3;
int motorR_2 = 6;

int motorL_1 = 5;
int motorL_2 = 9;

int v_Front;
int v_Side;

int proportionalUsed;

void setup()
{
  Serial.begin(9600);

  pinMode(startPin, INPUT);
  pinMode(side_L, INPUT);
  pinMode(side_R, INPUT);
  pinMode(front_L, INPUT);
  pinMode(front_R, INPUT);

  pinMode(motorL_1, OUTPUT);
  pinMode(motorL_2, OUTPUT);
  pinMode(motorR_1, OUTPUT);
  pinMode(motorR_2, OUTPUT);

  v_Front = 0;
  v_Side = 0;

  proportionalUsed = 0;

  //  delay(5000);
  //  motorsTest1();
  //  delay(9900);
  //  delay(9900);
  //  delay(9900);

}

void loop()
{
start:
    if (digitalRead(startPin) == HIGH) {
  Serial.println("Encendido");

  // ---------- Front ----------

  v_Front = frontSensors();
  if (v_Front == 3)
  {
    if (proportionalUsed == 0) {
      Serial.println("Enfrente - ambos");
      goForward_proportional(80);
      proportionalUsed = 1;
      goto start;
    } else if (proportionalUsed == 1) {
      goForward(255, 255, 30);
      goto start;
    }
  }
  else if (v_Front == 1)
  {
    Serial.println("Enfrente - izquierda");
    goLeft(false, 90, 30, 1);
    proportionalUsed = 0;
    goto start;
  }
  else if (v_Front == 2)
  {
    Serial.println("Enfrente - derecha");
    goRight(false, 90, 30, 1);
    proportionalUsed = 0;
    goto start;
  }

  // ---------- Side ----------

  v_Side = sideSensors();
  if (v_Side == 1)
  {
    Serial.println("Lado - izquierda");
    goFull_left();
    proportionalUsed = 0;
    goto start;
  }
  else if (v_Side == 2)
  {
    Serial.println("Lado - derecha");
    goFull_right();
    proportionalUsed = 0;
    goto start;
  }
  else
  {
    Serial.println("Ninguno");
    goForward_proportional(20);
    delay(150);
    proportionalUsed = 0;
    goto start;
  }
    } else {
      Serial.println("Apagado");
      stopi(false);
      proportionalUsed = 0;
      //FLAG.write(90);
    }

} // <<<---loop end


// ------------------------- Sensors Functions ------------------------- .

int frontSensors()
{
  int frontL = digitalRead(front_L);
  int frontR = digitalRead(front_R) * 2;

  int addition_Front = frontL + frontR;

  return addition_Front;
}

int sideSensors()
{
  int sideL = digitalRead(side_L);
  int sideR = digitalRead(side_R) * 2;

  int addition_Side = sideL + sideR;

  return addition_Side;
}

// ------------------------- Stop Functions ------------------------- .

void stopi(bool reverse)
{
  if (reverse == false)
  {
    digitalWrite(motorL_2, HIGH);
    digitalWrite(motorL_1, HIGH);

    digitalWrite(motorR_2, HIGH);
    digitalWrite(motorR_1, HIGH);
  } else if (reverse == true)
  {
    digitalWrite(motorL_2, LOW);
    digitalWrite(motorL_1, LOW);

    digitalWrite(motorR_2, LOW);
    digitalWrite(motorR_1, LOW);
  }
}

// ------------------------- Motors Functions ------------------------- .

void goForward(int powerL_f, int powerR_f, int workTime_f)
{
  analogWrite(motorL_1, powerL_f);
  digitalWrite(motorL_2, LOW);

  analogWrite(motorR_1, powerR_f);
  digitalWrite(motorR_2, LOW);

  delay(workTime_f);
  stopi(false);
}

void goBack(int powerL_b, int powerR_b, int workTime_b)
{
  digitalWrite(motorL_1, LOW);
  analogWrite(motorL_2, powerL_b);

  digitalWrite(motorR_1, LOW);
  analogWrite(motorR_2, powerR_b);

  delay(workTime_b);
  stopi(true);
}

void goRight(bool curve_r, int power_r, int workTime_r, int relation_r)
{
  if (curve_r == true)
  {
    analogWrite(motorL_1, power_r);
    digitalWrite(motorL_2, LOW);

    analogWrite(motorR_1, round(power_r / relation_r));
    digitalWrite(motorR_2, LOW);

    delay(workTime_r);
    stopi(false);
  }
  else if (curve_r == false)
  {
    analogWrite(motorL_1, power_r);
    digitalWrite(motorL_2, LOW);

    digitalWrite(motorR_1, LOW);
    analogWrite(motorR_2, power_r);

    delay(workTime_r);
    stopi(false);
  }
}

void goLeft(bool curve_l, int power_l, int workTime_l, int relation_l)
{
  if (curve_l == true)
  {
    analogWrite(motorL_1, round(power_l / relation_l));
    digitalWrite(motorL_2, LOW);

    analogWrite(motorR_1, power_l);
    digitalWrite(motorR_2, LOW);

    delay(workTime_l);
    stopi(false);
  }
  else if (curve_l == false)
  {
    digitalWrite(motorL_1, LOW);
    analogWrite(motorL_2, power_l);

    analogWrite(motorR_1, power_l);
    digitalWrite(motorR_2, LOW);

    delay(workTime_l);
    stopi(false);
  }
}

void goFull_left() {
  digitalWrite(motorL_1, LOW);
  digitalWrite(motorL_2, HIGH);

  digitalWrite(motorR_1, HIGH);
  digitalWrite(motorR_2, LOW);
}

void goFull_right() {
  digitalWrite(motorL_1, HIGH);
  digitalWrite(motorL_2, LOW);

  digitalWrite(motorR_1, LOW);
  digitalWrite(motorR_2, HIGH);
}

// ------------------------- Proportional Functions ------------------------- .

void goForward_proportional(int workTime_fp) {
  for (int i = 5; i < 255; i = i + 50) {
    if (i < 200) {
      goForward(i, i, round(workTime_fp / 4));
    } else {
      goForward(i, i, workTime_fp);
    }
  }
}

// ------------------------- Test Functions ------------------------- .

void motorsTest1()
{
  goBack(70, 220, 350);
  delay(2000);
  goForward(255, 255, 80);
  delay(2000);
  goRight(false, 255, 500, 1);
  delay(2000);
  goLeft(false, 255, 300, 1);
  delay(1000);
  stopi(false);
}
