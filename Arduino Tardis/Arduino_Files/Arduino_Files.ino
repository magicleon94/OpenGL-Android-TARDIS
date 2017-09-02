#define X A0
#define Y A1
#define Z A2
float prev_x;
float prev_y;
float prev_z;
float calib_x;
float calib_y;
float calib_z;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(X,INPUT);
  pinMode(Y,INPUT);
  pinMode(Z,INPUT);
  prev_x = calib_x = analogRead(X);
  prev_y = calib_y = analogRead(Y);
  prev_z =calib_z = analogRead(Z);
  

}

void loop() {
  float x = analogRead(X) - calib_x;
  float y = analogRead(Y) - calib_y;
  float z = analogRead(Z) - calib_z;

  Serial.print(x - prev_x);
  Serial.print("\t");
  
  Serial.print(y - prev_y);
  Serial.print("\t");
  
  Serial.print(z - prev_z);
  Serial.print("\n");

  prev_x = x;
  prev_y = y;
  prev_z = z;
  delay(100);
  

}
