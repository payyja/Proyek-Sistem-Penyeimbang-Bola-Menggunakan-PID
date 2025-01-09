#include <Servo.h>

Servo servo;

#define trig 2
#define echo 3

#define kp 15
#define ki 0.035
#define kd 20

double priError = 0;
double toError = 0;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  servo.attach(5); // Pin servo
  Serial.begin(9600);
  servo.write(90); // Posisi awal servo
}

void loop() {
  PID();
  delay(100); // Kurangi delay agar servo lebih responsif
}

long distance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long t = pulseIn(echo, HIGH);
  long cm = t / 29 / 2; // Konversi waktu pantulan ke jarak (cm)

  // Batasi jarak untuk stabilitas
  if (cm > 30) cm = 30; // Maksimum 30 cm
  if (cm < 2) cm = 2;     // Minimum 2 cm

  return cm;
}

void PID() {
  int dis = distance();

  int setP = 11; // Setpoint (target jarak)
  double error = setP - dis;

  double Pvalue = error * kp;
  double Ivalue = toError * ki;
  double Dvalue = (error - priError) * kd;

  double PIDvalue = Pvalue + Ivalue + Dvalue;

  // Update error untuk iterasi berikutnya
  priError = error;
  toError += error;

  // Cetak nilai error ke Serial Monitor
  Serial.print(error);
  Serial.println();
  // Serial.print(PIDvalue);

  // Map nilai PID ke rentang servo
  int Fvalue = (int)PIDvalue;
  Fvalue = map(Fvalue, -150, 150, 0, 180); // Rentang servo 0° hingga 180°

  // Batasi nilai servo agar valid
  if (Fvalue < 0) Fvalue = 0;
  if (Fvalue > 180) Fvalue = 180;

  // Gerakkan servo
  servo.write(Fvalue);
}
