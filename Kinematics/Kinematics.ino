
//parametry robota, w mm
const int f = 150;
const int rf = 100;
const int re = 320;
const int e = 50;

//stale trygonometryczne
const float sqrt3 = sqrt(3.0);
const float pi = 3.141592653;    // PI
const float sin120 = sqrt(3)/2.0;   
const float cos120 = -0.5;        
const float tan60 = sqrt(3);
const float sin30 = 0.5;
const float tan30 = 1/sqrt(3);

int checkPos(int status, float theta1, float theta2, float theta3, float x0, float y0, float z0)
{
 //refer to: https://www.marginallyclever.com/other/samples/fk-ik-test.html
  const float minX = -83.846;
  const float maxX = 83.846;
  const float minY = -83.846;
  const float maxY = 83.846;
  const float minZ = -392.894;
  const float maxZ = -225.203;

  if((theta1>104.04 || theta1<-56.74 || theta2>106.32 || theta2<-64.71 || theta3>106.32 || theta3<-64.71) && status == 0)
    status = -2;
  if((x0<minX || x0>maxX || y0<minY || y0>maxY || z0<minZ || z0>maxZ) && status == 0)
    status = -3;
    
  return status;
}

// forward kinematics: (theta1, theta2, theta3) -> (x0, y0, z0)
// returned status: 0=OK, -1=non-existing position

 int delta_calcForward(float theta1, float theta2, float theta3, float &x0, float &y0, float &z0) 
 {
     float t = (f-e)*tan30/2;
     float dtr = pi/(float)180.0;
 
     theta1 *= dtr;
     theta2 *= dtr;
     theta3 *= dtr;
 
     float y1 = -(t + rf*cos(theta1));
     float z1 = -rf*sin(theta1);
 
     float y2 = (t + rf*cos(theta2))*sin30;
     float x2 = y2*tan60;
     float z2 = -rf*sin(theta2);
 
     float y3 = (t + rf*cos(theta3))*sin30;
     float x3 = -y3*tan60;
     float z3 = -rf*sin(theta3);
 
     float dnm = (y2-y1)*x3-(y3-y1)*x2;
 
     float w1 = y1*y1 + z1*z1;
     float w2 = x2*x2 + y2*y2 + z2*z2;
     float w3 = x3*x3 + y3*y3 + z3*z3;
     
     // x = (a1*z + b1)/dnm
     float a1 = (z2-z1)*(y3-y1)-(z3-z1)*(y2-y1);
     float b1 = -((w2-w1)*(y3-y1)-(w3-w1)*(y2-y1))/2.0;
 
     // y = (a2*z + b2)/dnm;
     float a2 = -(z2-z1)*x3+(z3-z1)*x2;
     float b2 = ((w2-w1)*x3 - (w3-w1)*x2)/2.0;
 
     // a*z^2 + b*z + c = 0
     float a = a1*a1 + a2*a2 + dnm*dnm;
     float b = 2*(a1*b1 + a2*(b2-y1*dnm) - z1*dnm*dnm);
     float c = (b2-y1*dnm)*(b2-y1*dnm) + b1*b1 + dnm*dnm*(z1*z1 - re*re);
  
     // discriminant
     float d = b*b - (float)4.0*a*c;
     if (d < 0) return -1; // non-existing point
 
     z0 = -(float)0.5*(b+sqrt(d))/a;
     x0 = (a1*z0 + b1)/dnm;
     y0 = (a2*z0 + b2)/dnm;
     return 0;
 }
 
 // inverse kinematics
 // helper functions, calculates angle theta1 (for YZ-pane)
 int delta_calcAngleYZ(float x0, float y0, float z0, float &theta) {
     float y1 = -0.5 * tan30 * f; // f/2 * tg 30
     y0 -= 0.5 * 0.57735    * e;    // shift center to edge
     // z = a + b*y
     float a = (x0*x0 + y0*y0 + z0*z0 +rf*rf - re*re - y1*y1)/(2*z0);
     float b = (y1-y0)/z0;
     // discriminant
     float d = -(a+b*y1)*(a+b*y1)+rf*(b*b*rf+rf); 
     if (d < 0) return -1; // non-existing point
     float yj = (y1 - a*b - sqrt(d))/(b*b + 1); // choosing outer point
     float zj = a + b*yj;
     theta = 180.0*atan(-zj/(y1 - yj))/pi + ((yj>y1)?180.0:0.0);
     return 0;
 }
 
 // inverse kinematics: (x0, y0, z0) -> (theta1, theta2, theta3)
 // returned status: 0=OK, -1=non-existing position
 int delta_calcInverse(float x0, float y0, float z0, float &theta1, float &theta2, float &theta3) 
 {
     theta1 = theta2 = theta3 = 0;
     int status = delta_calcAngleYZ(x0, y0, z0, theta1);
     if (status == 0) status = delta_calcAngleYZ(x0*cos120 + y0*sin120, y0*cos120-x0*sin120, z0, theta2);  // rotate coords to +120 deg
     if (status == 0) status = delta_calcAngleYZ(x0*cos120 - y0*sin120, y0*cos120+x0*sin120, z0, theta3);  // rotate coords to -120 deg
     return status;
 }

void printResult(int status, String task, float theta1, float theta2, float theta3, float x0, float y0, float z0)
{
  //status = checkPos(status, theta1, theta2, theta3, x0, y0, z0);
  Serial.print("Status: ");
  Serial.println(status);
  Serial.println(task);
  Serial.println("Theta:");
  Serial.print("\t1: ");
  Serial.println(theta1);
  Serial.print("\t2: ");
  Serial.println(theta2);
  Serial.print("\t3: ");
  Serial.println(theta3);
  Serial.print("Point: ( ");
  Serial.print(x0);
  Serial.print(" ; ");
  Serial.print(y0);
  Serial.print(" ; ");
  Serial.print(z0);
  Serial.println(" )");
  Serial.println();
  Serial.println();
}
 
void setup() {
  Serial.begin(9600);
  int i=0;
  int status=0;
  float theta1, theta2, theta3, x0, y0, z0;
  float pos[3][3] = {
    {0.0, 0.0, -299.809},
    {50.0, 50.0, -300.0},
    {150.0, 150.0, -200}
  };
  float thetas[3][3] = {
    {0.0, 0.0, 0.0},
    {5.0, -10.0, 25.0},
    {-35.0, -35.0, -35.0}
  };
  for(i=0;i<3;i++)
  {
    status = delta_calcInverse(pos[i][0],pos[i][1],pos[i][2],theta1,theta2,theta3);
    printResult(status, "Calc inverse", theta1, theta2, theta3, pos[i][0],pos[i][1],pos[i][2]);
    delay(1000);
  }
    
  for(i=0; i<3;i++)
  {
    status = delta_calcForward(thetas[i][0], thetas[i][1], thetas[i][2], x0,y0,z0);
    printResult(status, "Calc forward", thetas[i][0], thetas[i][1], thetas[i][2], x0,y0,z0);  
    delay(1000);
  }  
}

void loop() {
  // put your main code here, to run repeatedly:

}
