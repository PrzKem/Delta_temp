int checkPos(int status, float theta1, float theta2, float theta3, float x0, float y0, float z0)
{
 //refer to: https://www.marginallyclever.com/other/samples/fk-ik-test.html

  if((theta1>MAX_THETA1 || theta1<MIN_THETA1 || theta2>MAX_THETA2 || theta2<MIN_THETA2 || theta3>MAX_THETA3 || theta3<MIN_THETA3) && status == 0)
    status = -2;
  if((x0<MIN_X || x0>MAX_X || y0<MIN_Y || y0>MAX_Y || z0<MIN_Z || z0>MAX_Z) && status == 0)
    status = -3;
    
  return status;
}
