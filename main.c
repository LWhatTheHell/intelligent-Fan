#include <reg51.h>
#define LED1 P1
#define LED2 P2
sbit DB=P3^0;				//D18B20
sbit SR=P3^1;			    //SR501
sbit FAN1=P3^2;				//风扇PWM线1
sbit FAN2=P3^3;				//风扇PWM线2
unsigned char number[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};
unsigned char number_negative[]={0xbf,0x86 ,0xdb,0xcf,0xed,0xfd,0x87,0xff,0xe7};
int num=20,highTime=20,lowTime=20;
long int counter=0;

unsigned int temperature();            //读取并显示温度，返回温度值
void init_t();                         // DS18B20 初始化
unsigned short read_t();               // DS18B20 读
void write_t(unsigned short content);  // DS18B20 写

void delayu(unsigned int us){          //延时 17+13n us
	for(;us>0;us--);	
}

void delay(unsigned int ms){
	unsigned int i,j;
	for(i=0;i<ms;i++){
		for(j=0;j<124;j++);
	} 
}

void Timer() interrupt 1 using 0 {
	TR0=0;
	FAN1=~FAN1;
	FAN2=~FAN2;
	if(SR==1){
		if(FAN1&&FAN2){
			TH0=255;
			TL0=255-highTime;	
		}else{
			TH0=255;
			TL0=255-lowTime;
		}
	}else{
		FAN1=FAN2=0;		
	}
	TR0=1;
}

void Timer1() interrupt 3 using 1{
	TR1=0;
	if(counter==1000){
		counter=0;
		TR0=0;
		ET0=0;
		FAN1=FAN2=0;
		num=temperature();
		if(num>40){
			num=40;
		}
		lowTime=40-num;
		highTime=num;
		if(num<0){
			
		}else{
		ET0=1;
		TR0=1;
		}
	}else{
		counter++;
	}
	TR1=1;
}

void main(){
	FAN1=FAN2=0;
	TMOD=0x11;
	TH0=255;
	TL0=254;
	TH1=0x00;
	TL1=0x00;
	EA=1;
	num=temperature();
	if(num>40){
		num=40;
	}
	lowTime=40-num;
	highTime=num;
	if(num<0){
		
	}else{
	ET0=1;
	TR0=1;
	}
	ET1=1;
	TR1=1;

	while(1);							 
}

void init_t(){
	unsigned int i=0;
	DB=0;
	delayu(40);
	DB=1;
	delayu(4);
	delay(35);
	DB=1;
}

void write_t(unsigned short content){
	unsigned char i,o;
	for(i=0;i<8;i++){
		DB=0;
		o++;o++;o++;o++;o++;o++;o++;o++;o++;o++;o++;o++;o++;o++;o++;    //15us
		DB=content&0x01;
		content>>=1;
		delayu(2);							//45us
		DB=1;
	}	
}

unsigned short read_t(){
	unsigned char i,o=0;
	union {unsigned char a[2];unsigned short t;}u;
	u.t=0;
	for(i=0;i<8;i++){
		u.a[1]>>=1;
		DB=0;
		o++;				   //2us
		DB=1;
		o++;o++;o++;o++;		   //4us
		if(DB){
			u.a[1]|=0x80;
		}
		delayu(3);				  //60us
		DB=1;
	}
	for(i=0;i<8;i++){
		u.a[0]>>=1;		  
		DB=0;
		o++;
		DB=1;
		o++;o++;o++;o++;
		if(DB){
			u.a[0]|=0x80;
		}
		delayu(3);
		DB=1;
	}
	return u.t;	
}												  

unsigned int temperature(){
		unsigned int temp=0;
		init_t();
		write_t(0xcc);
		write_t(0x44);
		delay(750);
		init_t();
		write_t(0xcc);
		write_t(0xbe);
		temp = (read_t()/16);
		if(temp<=180){
			LED1=number[(temp%100)/10];
			LED2=number[temp%10];
			return temp;
		}else{
			temp = ~temp+1;
			temp= -temp;
			LED1=number_negative[(temp%100)/10];
			LED2=number_negative[temp%10];
			return -temp;
		}

}