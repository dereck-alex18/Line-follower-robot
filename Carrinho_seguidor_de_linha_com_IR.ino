#include <IRremote.h>
#define MA_VEL 11 //ENA
#define MA_DIR1 10 //IN1
#define MA_DIR2 9 //IN2
// Left side motors
#define MB_VEL 6 //ENB
#define MB_DIR1 5 //IN3
#define MB_DIR2 4 //IN4
#define IRpin 13


int linha;
//This function calibrates the IR sensors
void Calibragem();
void control();
void voltas();
IRrecv irrecv(IRpin);
decode_results results;

int X = 150;
int Y = 0;
byte laps = 0, lapsAux = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(MA_DIR1, OUTPUT);
  pinMode(MA_DIR2, OUTPUT);
  pinMode(MB_DIR1, OUTPUT);
  pinMode(MB_DIR2, OUTPUT);
  pinMode(MA_VEL, OUTPUT);
  pinMode(MB_VEL, OUTPUT);
  irrecv.enableIRIn();
  Calibragem();
  voltas();
  delay (2000);
  analogWrite(MA_VEL, 200);
  analogWrite(MB_VEL, 200);
  digitalWrite(MA_DIR2, HIGH);
  digitalWrite (MA_DIR1, LOW);
  digitalWrite(MB_DIR2, HIGH);
  digitalWrite(MB_DIR1, LOW);
  delay(1000);

}    
    
      void loop()
  { 

   
   unsigned int s1 = 0, valorS1;
   unsigned int s2 = 0, valorS2 = 0;
   for(int i = 0; i < 15; i++){
     //Both sensors "read" the color 15 times to detect if the floor is white or not
     delay(2);
     valorS1 += analogRead(A0);
     valorS2 += analogRead(A1);     
    }
    //Then the average is calculated
    s1 += valorS1 / 15;
    s2 += valorS2 / 15;
   
   Serial.print("\t");
   Serial.print(s1);
   Serial.print("\t");
   Serial.println(s2);
   //delay(100);
      
      if(s1<linha && s2<linha){
          //Black line not detected
           control('w');
           Serial.println("Linha preta nao detectada");
           X = 200;
           }
       else if(s1>linha && s2 < linha){
            //Black line was detected by the sensor 1
            Serial.println("Linha preta em S1");
            X = 180;
            Y = 160;
            control('a');
            
       }
        else if(s2 > linha && s1 < linha){
            //Black line was detected by the sensor 2
            Serial.println("Linha preta em S2");
            X = 180;        
            Y = 160;
            control('d');
                    
          }

         else if (s1>linha && s2>linha){
           //Black line was detected by both sensors
           lapsAux++;
           Serial.println("Linha preta detectada em S1 e S2");
           if(lapsAux == laps){ 
           Serial.println("Parou!!");
           X = 0;
           Y = 0;
           control('w');   
         }
            
          else{ 
           control('w');
            X = 200;
          }  
          
        }
     }
    
void Calibragem(){
  
  
   unsigned int s1Aux = 0, valorS1Aux = 0;
   unsigned int s2Aux = 0, valorS2Aux = 0;
   for(int i = 0; i < 50; i++){
     delay(10);
     valorS1Aux += analogRead(A0);
     valorS2Aux += analogRead(A1);     
       
   
   
    }
    s1Aux += valorS1Aux / 50 ;
    s2Aux += valorS2Aux / 50;
   Serial.print("\t");
   Serial.print(s1Aux);
   Serial.print("\t");
   Serial.println(s2Aux);
   
   if(s1Aux < 700){
       linha = 770;
   
   }
    else{
       linha = 900;
   }
}  

void control(char dir){
        switch(dir){
          // The car moves forward
         
          case 'w':
          analogWrite(MA_VEL, X);
          analogWrite(MB_VEL,X);
          digitalWrite(MA_DIR2, HIGH);//MA_DIR2 - Anda para frente
          digitalWrite(MA_DIR1, LOW); //MA_DIR1 - Anda para trás
          digitalWrite(MB_DIR2, HIGH);
          digitalWrite(MB_DIR1, LOW);
             break     ;
        
        // The car moves back
           case 'z':
          analogWrite(MA_VEL, X);
          analogWrite(MB_VEL,X);
          digitalWrite(MA_DIR2, LOW);
          digitalWrite(MA_DIR1, HIGH);
          digitalWrite(MB_DIR2, LOW);
          digitalWrite(MB_DIR1, HIGH);
           break     ;
       
        // The car moves left
          case 'd':
          analogWrite(MA_VEL,X); //Roda direita
          analogWrite(MB_VEL,Y);//Roda esquerda
          digitalWrite(MA_DIR1, LOW); //MA_DIR1 - Anda para trás
          digitalWrite(MA_DIR2, HIGH); //MA_DIR2 - Anda para frente
           digitalWrite(MB_DIR2, LOW);
          digitalWrite(MB_DIR1, HIGH);
          
             break     ;
            // The car moves right
          case 'a':
          analogWrite(MA_VEL, Y);//Roda direita
          analogWrite(MB_VEL,X);//Roda esquerda
          digitalWrite(MB_DIR1, LOW); //MB_DIR1 - Anda para trás
          digitalWrite(MB_DIR2, HIGH);//MB_DIR2 - Anda para frente
          digitalWrite(MA_DIR1, HIGH); //MA_DIR1 - Anda para trás
          digitalWrite(MA_DIR2, LOW);   
             break     ;
           
          }
      
      }
      
void voltas(){
  //This functions handle the number of laps that the car will take
  do{  
  if(irrecv.decode(&results)){
        Serial.print("valor lido: ");
        Serial.println(results.value, HEX);
        
        if(results.value == 0x2068D02F){
            laps++;
        
        }
        
        irrecv.resume();
    }
   
  }
   while(results.value != 0x2068B04F);
   Serial.println(laps);

}
  
