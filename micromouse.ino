//The device works on principle of vector addition and vector space 
#define Rm1 4 //right motor
#define Rm3 6 //right motor
#define Lm2 5 //left motor
#define Lm4 3 //left motor
#define trigl 10 //left side ultrasound sensor trigpin
#define echol 11 //left side ultrasound sensor echopin
#define trigr 9 //right side ultrasound sensor trigpin
#define echor 8 //right side ultrasound sensor echopin
#define trigf 2 //front side ultrasound sensor trigpin
#define echof 7 //front side ultrasound sensor echopin
int t,d,L=10,R=10,F=8,z=4,fwdl[50],indpt[50],ind=0,sumval=0,ci=0,cj=0,cindpt=0,looper=0,y,f=4,g=1;
/* t=time, d=distance , L=max left side distance,R=max right side distance,F=max front side distance, fwdl = distance moved , indpt note the lattice points where there were more than 1 path, ind is index of each stack
ci,cj gives the count of i cap and j cap, cindpt = to count number of cind, looper to report the kind of loop,z variable changes on turning and accounts for direction of movement */
char dir[50];// to note the direction
unsigned long nend, nstart;// start and end time for turning
int tellval(int a)// this function returns magnitude of movement , positive if i cap, j cap and negative -i cap, -j cap
{
  if(z==4)
  {
    return a;
  }
   else if(z==1)
  {
    return a;
  }
   else if(z==2)
  {
    return -1*a;
  }
   else if(z==3)
  {
    return -1*a;
  }
  
}
char telldir(int b)// depending on value of z it gives the direction
{
  if(b==4)
  {
return 'j';    
  }
  if(b==1)
  {
return 'i';
  }
  if(b==2)
  {
return 'j';
  }
  if(b==3)
  {
return "i";
  }
}
void fwd()// amount of forward movement
{
  nstart=millis();
  while(shape()==8)
  {
     digitalWrite(Rm1,HIGH);
  digitalWrite(Rm3,LOW);
  analogWrite(Lm2,235);
  analogWrite(Lm4,0);
  }
  nend=millis();
   fwdl[ind]+=tellval((nend-nstart)/1000);
   dir[ind]=telldir(z);
}
void Tr()// right turn
{
  nstart=millis();
nend=millis();
while(((nend-nstart)/1000)<=0.15)
{
   digitalWrite(Rm1,LOW);
  digitalWrite(Rm3,LOW);
  analogWrite(Lm2,235);
  analogWrite(Lm4,0);
  nend=millis();
}
}
void Tl()//left turn
{
  nstart=millis();
nend=millis();
while(((nend-nstart)/1000)<=0.15)
{
  digitalWrite(Rm1,HIGH);
  digitalWrite(Rm3,LOW);
  digitalWrite(Lm2,LOW);
  digitalWrite(Lm4,LOW);
  nend=millis();
}
    

}
void uT()// U turn
{
  nstart=millis();
nend=millis();
while(((nend-nstart)/1000)<=0.15)
{
   digitalWrite(Rm1,HIGH);
  digitalWrite(Rm3,LOW);
  analogWrite(Lm2,0);
  analogWrite(Lm4,235);
  nend=millis();
}
}

void stop()// stops the device
{
   digitalWrite(Rm1,LOW);
  digitalWrite(Rm3,LOW);
  digitalWrite(Lm2,LOW);
  digitalWrite(Lm4,LOW);
}
int valchange(int val)// since z++ in case of right turn ad z-- in case of left turn and z-2 in case of u turn, 
{
  if(val>4)
  {
    return 1;
  }
  else if(val<1)
  {
    return 4;
  }
  else
  {
    return val;
  }
}
int Lside()// returns left side distance
{
  digitalWrite(trigl,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigl,LOW);
  t=pulseIn(echol,HIGH);
  d=t*0.034/2;
  return d;
}
int Fside()// returns front sode distance
{
  digitalWrite(trigf,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigf,LOW);
  t=pulseIn(echof,HIGH);
  d=t*0.034/2;
  return d;
}
int Rside()// returns right side distance
{
  digitalWrite(trigr,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigr,LOW);
  t=pulseIn(echor,HIGH);
  d=t*0.034/2;
  return d;
}
void TurnR()// along with right turn it moves for a time till both side are obstruction  that is it can sense the the 1 cm thick wall or more
{
{
  ind++;
  Tr();
  z++;
  z= valchange(z);
  while(((nend-nstart)/1000)<=0.5)
{
  digitalWrite(Rm1,HIGH);
  digitalWrite(Rm3,LOW);
  digitalWrite(Lm2,HIGH);
  digitalWrite(Lm4,LOW);
  nend=millis();
}

}
void TurnL()// along with left turn it moves for a time till both side are obstruction that is it can sense the the 1 cm thick wall or more
{
  ind++;
  Tl();
  z--;
  z= valchange(z);
   while(((nend-nstart)/1000)<=0.5)
{
  digitalWrite(Rm1,HIGH);
  digitalWrite(Rm3,LOW);
  digitalWrite(Lm2,HIGH);
  digitalWrite(Lm4,LOW);
  nend=millis();
}

}
void TurnU()// along with U turn it moves for a time till both side are obstruction that is it can sense the the 1 cm thick wall or more
{
  ind++;
  uT();
  z=z-2;
  z= valchange(z);
   while(((nend-nstart)/1000)<=0.5)
{
  digitalWrite(Rm1,HIGH);
  digitalWrite(Rm3,LOW);
  digitalWrite(Lm2,HIGH);
  digitalWrite(Lm4,LOW);
  nend=millis();
}

}
int shape()// returns the type of shape 
{
  if(Lside()>L&&Rside()>R&&Fside()>F)
  {
    //Serial.println("Rev T shape");
    return 1;
  }
  else if(Lside()>L&&Rside()>R&&Fside()<F)
  {
    //Serial.println("T shape");
    return 2;
  }
  else if(Lside()<L&&Rside()>R&&Fside()<F)
  {
    //Serial.println("JR shape");
    return 3;
  }
  else if(Lside()>L&&Rside()<R&&Fside()<F)
  {
    //Serial.println("JL shape");
    return 4;
  }
  else if(Lside()>L&&Rside()<R&&Fside()>F)
  {
    //Serial.println("FL shape");
    return 5;
  }
    else if(Lside()<L&&Rside()>R&&Fside()>F)
  {
    //Serial.println("FR shape");
    return 6;
  }
  else if(Lside()<L&&Rside()<R&&Fside()<F)
  {
    //Serial.println("UT shape");
    return 7;
  }
  else
  {
    //Serial.println("forward");
    return 8;
  }
}
int loopval()// it returns the type of loop, 1st type if without opening and 2nd type with opening in between
{
  for(int x=ind;x>=0;x--)
  {
    sumval+=fwdl[x];// vector addition in stack format from last cell
    if(sumval<=5)// allowance of error of 5 cm
    {
      for(int y=ind;y>=x;y--)
      {
        if(dir[ind]=='i')
        {
          ci+=1;
        }
        else
        {
          cj+=1;
        }
      }
      if(ci==cj)// check same directions in loop
      {
        for(y=ind;y>=x;y--)
        {
          if(indpt[y]>0)
          {
            cindpt++;
          }
        }
        if(cindpt>1)
        {
          return 2;
        } 
        else
        {
          return 1;
        } 
         for(y=ind;y>=x;y--)
        {
          indpt[y]=0;
          dir[y]='0';
          fwdl[y]=0;
        }
        ind=y-1;
       }
    }
  }
  ci=0;
  cj=0;
}
int modval(int a)// returns the  magnitude of movement independent of direction
{
  if(a>=0)
  {
    return a;
  }
  else
  {
    -1*a;
  }
}
int absval(int a)
{
  return a/modval(a);// returns -1 and ++1 so that -i, +i can cancel each other and -j and +j can cancel eachother
}
void moveExcept(int k)// in case of a loop it moves in a different opening other than the openning it had entered
{
  if(k==1)
  {
    TurnR();
  }
  else if(k==2)
  {
    if((z==4)&&(dir[ind]=='i')&&absval(fwdl[ind]==1))
    {
      TurnR();
    }
    if((z==4)&&(dir[ind]=='i')&&absval(fwdl[ind]==-1))
    {
      TurnL();
    }
    if((z==2)&&(dir[ind]=='i')&&absval(fwdl[ind]==1))
    {
      TurnL();
    }
    if((z==2)&&(dir[ind]=='i')&&absval(fwdl[ind]==-1))
    {
      TurnR();
    }
    if((z==1)&&(dir[ind]=='j')&&absval(fwdl[ind]==1))
    {
      TurnL();
    }
    if((z==1)&&(dir[ind]=='j')&&absval(fwdl[ind]==-1))
    {
      TurnR();
    }
    if((z==3)&&(dir[ind]=='j')&&absval(fwdl[ind]==1))
    {
      TurnR();
    }
    if((z==3)&&(dir[ind]=='j')&&absval(fwdl[ind]==-1))
    {
      TurnL();
    }
  }
  else if((k==5)||(k==6))
  {
    ind++;
    dir[ind]=telldir(z);
     while(shape()!=8)
  {
    fwd();
  }
   while(shape()==8)
  {
    fwd();
  }
  }
}
void finddir(int a,char b)// returns value of z for its 2nd time movement 
{
  if(a==1,b=='j')
  {
    return 4;
  }
  else if(a==1,b=='i')
  {
    return 1;
  }
  else if(a==-1,b=='j')
  {
    return 2;
  }
  else if(a==-1,b=='i')
  {
    return 3;
  }
}
void guessdir(int p)// dwepending on value of z it would move on 2nd trial
{
  int bunny =f;
  bunny++;
  valchange(bunny);
  if(bunny==p)
  {
    Tr();
    f++;
    valchange(f);
  }
  bunny=f;
  bunny=bunny-1;
  valchange(bunny);
  if(bunny==p)
  {
    Tl();
    f--;
    valchange(f);
  }
}
void follow(int a, int b)// depending on fwdl stack and dir stack it moves by changing vlue of z
{ 
  if(a==b)
{
  return 0;
}

   while(state()==8)
      {
          digitalWrite(Rm1,HIGH);
  digitalWrite(Rm3,LOW);
  digitalWrite(Lm2,HIGH);
  digitalWrite(Lm4,LOW);
      }
     guessdir(finddir((absval(fwdl[g]),dir[g])));
      while(shape()!=8)
  {
    digitalWrite(Rm1,HIGH);
  digitalWrite(Rm3,LOW);
  digitalWrite(Lm2,HIGH);
  digitalWrite(Lm4,LOW);
  }
 g++;
 follow();
}
void setup()
{
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  pinMode(trigl,OUTPUT);
  pinMode(echol,INPUT);
  pinMode(trigr,OUTPUT);
  pinMode(echor,INPUT);
  pinMode(trigf,OUTPUT);
  pinMode(echof,INPUT);
  pinMode(Rm1,OUTPUT);
  pinMode(Rm3,OUTPUT);
  pinMode(Lm2,OUTPUT);
  pinMode(Lm4,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
 /* Serial.println("R");
  Serial.println(Rside());
  Serial.println("L");
  Serial.println(Lside());
   Serial.println("F");
  Serial.println(Fside());*/
  Serial.println(shape());// depending upon type of opening it decides to move
  if(shape()==8)
  {
    fwd();
  }
  else if(shape()==7)
  {
    TurnU();

  }
  else if(shape()==6)
  {
    TurnR();
    indpt[ind]=ind;
  }
   else if(shape()==5)
  {
    TurnL();
        
    indpt[ind]=ind;
  }
   else if(shape()==4)
  {
    TurnL();
      
  }
   else if(shape()==3)
  {
    TurnR();
    
  }
   else if(shape()==2)
  {
    TurnR();
      
    indpt[ind]=ind;
  }
   else if(shape()==1)
  {
    TurnR();
        
    indpt[ind]=ind;
  }
  // depending on the kind of loop (in case it is inside a loop)the folowing code will take action 
if(loopval()==1)// depending on the kind of loop 
{
  if((shape()==1)||(shape()==2)||(shape()==5)||(shape()==6))
  {
    moveExcept(shape());
  }
}
else if(loopval()==2)
{
looper+=1;
}
else if((looper==1)&&((shape()==1)||(shape()==2)))
{
  TurnL();
  looper=0;
}
else if((looper==1)&&((shape()==5)||(shape()==6)))
{ ind++;
  dir[ind]=telldir(z);
   while(shape()!=8)
  {
    fwd();
  }
   while(shape()==8)
  {
    fwd();
  }
  looper=0;
}
}
