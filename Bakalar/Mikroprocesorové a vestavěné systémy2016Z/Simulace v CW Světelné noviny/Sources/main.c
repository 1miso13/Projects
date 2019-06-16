/* IMP projekt - Simulace v CW: Svìtelné noviny */
/* Autor: Ondrejó Michal */
/* login: xondre08 */

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#define chars * 8
#define dlzka 8 chars
#define START 612
unsigned char *login[dlzka];
unsigned char *RIGHT;
unsigned char *UP;
unsigned char *init;
unsigned char *bar;
int speed;
int mod;

void setAddr(){  
  int i;
  for(i=0;i<dlzka;i++){
      login[i]= (char *)START+i;
  }
  
  init=(char *)608;
  RIGHT=(char *)609;
  UP=(char *)610; 
  bar=(char *)611;
}

void setlogin(){
  //x
  *login[0]=0;
  *login[1]=66;
  *login[2]=36;
  *login[3]=24;
  *login[4]=24;
  *login[5]=36;
  *login[6]=66;
  *login[7]=0;
  //o
  *login[8]=0;
  *login[9]=60;
  *login[10]=66;
  *login[11]=66;
  *login[12]=66;
  *login[13]=66;
  *login[14]=60;
  *login[15]=0;
  //n
  *login[16]=0;
  *login[17]=126;
  *login[18]=4;
  *login[19]=2;
  *login[20]=2;
  *login[21]=124;
  *login[22]=0;
  *login[23]=0;
  //d
  *login[24]=0;
  *login[25]=56;
  *login[26]=68;
  *login[27]=68;
  *login[28]=40;
  *login[29]=126;
  *login[30]=0;
  *login[31]=0;
  //r
  *login[32]=0;
  *login[33]=0;
  *login[34]=126;
  *login[35]=4;
  *login[36]=2;
  *login[37]=2;
  *login[38]=0;
  *login[39]=0;
  //e
  *login[40]=0 ;
  *login[41]=60 ;
  *login[42]=82 ;
  *login[43]=82;
  *login[44]=82;
  *login[45]=28 ;
  *login[46]=0 ; 
  *login[47]=0 ; 
  //0      
  *login[48]=0 ;     
  *login[49]=0 ;     
  *login[50]=60 ;
  *login[51]=66 ;
  *login[52]=66 ;
  *login[53]=60 ;
  *login[54]=0 ;
  *login[55]=0 ; 
  //8     
  *login[56]=0 ;
  *login[57]=0 ;
  *login[58]=36 ;
  *login[59]=90 ;
  *login[60]=90 ;
  *login[61]=36 ;
  *login[62]=0 ;
  *login[63]=0 ;
}
void main(void) {
  EnableInterrupts; /* enable interrupts */
  /* include your code here */
  setAddr();
  setlogin();
  mod=2;
  *init = 0; 
  *RIGHT = 0;
  *UP = 0;
  *bar = 127;
  for(;;) {
    for(speed = 0; speed <(3500-(*bar * 10)); speed++) 
    {//cyklus ktory spomaluje posuv
       if(*RIGHT == 1) 
       {
          mod=0;
       } 
       else if(*UP == 1)
       {
          mod=1;
       }
       else if(*init == 1) 
       {
          mod=2;
       }
    }      
       if(mod==0){
       //posuv vpravo 
          int i;
          unsigned char tmp;
          tmp = *login[dlzka-1];

          for(i = dlzka-1; i > 0 ; i--) 
          {
            *login[i] = *login[i-1];
          }
          *login[0] = tmp;  
       }
       if(mod==1){
       //posuv hore 
          int i;
          for(i = 0; i <dlzka; i++) 
          {
            if(*login[i] %2 ==0) 
            {
              *login[i] = *login[i] >> 1;
            } 
            else 
            {
              *login[i] = *login[i] >> 1;
              *login[i] = *login[i] + 128;
            }
          }      
       } 
       if(mod==2){
          //znova sa nastavi pociatocny stav
          setlogin();
       }    
    __RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */  
}