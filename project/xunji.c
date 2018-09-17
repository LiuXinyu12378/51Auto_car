#include "reg52.h"			 	//���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���

typedef unsigned int u16;	  	//���������ͽ�����������
typedef unsigned char u8;
unsigned char code LXY[]={ 	//������0~f����ܱ���
0x02,0x9e,0x24,0x0c,0x98,0x48,0x40,0x1e,0x01,0x18
};

sbit M1=P0^0;				  //�������ӿ� *4
sbit M2=P0^1;
sbit M3=P0^2;
sbit M4=P0^3;
sbit Beep=P0^6;				  //����������ӿ�
sbit led_right=P0^7;		  //��ǰ��
sbit led_left=P2^0;			  //��ǰ��
sbit zuo=P3^5;				  //����չ�
sbit you=P3^6;				  //�н��չ�
sbit zhong=P3^7;			  //�ֽ��չ�

int timer=0;				  //��ʱ��1ms��timer��1
void delay(u16 i)			  //��ʱ����
{
	while(i--);	
}

void Beep_();				  //��������
void qianjin();
void houtui();
void zuozhuan();
void youzhuan();
void yuandi_zuozhuan();
void yuandi_youzhuan();
void tingzhi();
void flash_on();
void timer0_init();
void Init_all();
void main()					   //������	 �����Ƚ���������ˮ�ƣ���ǰ����������˸һ�Σ�Ȼ������������� ��������ʱ���жϳ��������ߣ���������ˮ��
{
	int n=0,i;
	Init_all();
	timer0_init();
	while(1)
	{	
   	P1=0xff;				   //8������˸
	delay(10000);
	P1=0x00;
	delay(10000);
	P2=LXY[n++]&0xfe;
	if(n==10) n=0;
			
	}		
}

void GO_() interrupt 1				  //��ʱ���жϺ���
{
int c=4;							 //�ٶ�ռ�ձȵ��� ��ռ�ձ�(10-c)/10
int b=7;							 //����ʱ�ٶ�ռ�ձ�
timer++;
if(timer>10)	timer=0; 	
 TH0=(65536-1000)/256; 				//  1MS��ʱ 1000us
 TL0=(65536-1000)%256; 
 if(zuo==0&zhong==0&you==0)		  		   //000	����������⵽��ɫֹͣ		   1�����⵽��ɫ��0�����⵽��ɫ
	 {
	   	tingzhi();
		flash_on();
		P1=0x00;
		Beep_();
		   
	 }
 	 	 if(zuo==0&zhong==0&you==1)		  //001		 �������̽ͷ��⵽��ɫ���ұ߼�⵽��ɫ��ת
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==b)
	   yuandi_youzhuan();
	 }
	  	 if(zuo==0&zhong==1&you==0)		  //010		  ֻ���м��̽ͷ��⵽��ɫ��ǰ��
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==c)
	   qianjin();
	 }
	  	 if(zuo==0&zhong==1&you==1)		  //011		   ���һ��̽ͷ��⵽��ɫ���ұ�����̽ͷ��⵽��ɫ��ת
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==b)
	 	youzhuan();
	 }
	  	 if(zuo==1&zhong==0&you==0)		  //100		   ���һ����⵽��ɫ���ұ���������ɫ��ת
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==b)
	 	yuandi_zuozhuan();
	 }
	  	 if(zuo==1&zhong==0&you==1)		  //101			���Ҿ���⵽��ɫ�����м��⵽��ɫ������������״���ǲ����ڵģ�
	 {
	 	if(timer==0)					  //            ����ʵ�ʿ�����Ϊ�����ڵ������������������������������ǰ��
		tingzhi();
		if(timer==c)
	 	qianjin();
	 }
	  	 if(zuo==1&zhong==1&you==0)		  //110			���������⵽��ɫ���ұ�һ����⵽��ɫ����ת
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==b)
	 	  zuozhuan();
	 }
	  	 if(zuo==1&zhong==1&you==1)		   //111		 ֻ�е�����̽ͷ����⵽����ʱ��ǰ��
	 {

		if(timer==0)
		tingzhi();
		if(timer==c)
	  	qianjin();
	 }
}  

void timer0_init()						//��ʱ����ʼ��
{
   TMOD = 0x01;  						//16λ��ʱ��ģʽ
    
 TH0=(65536-1000)/256; 					//  1MS��ʱ 1000us
 TL0=(65536-1000)%256; 
    
    EA = 1;       						//��ȫ���ж�
    ET0 = 1;      						//����T0�ж�
    TR0 = 1;      						//������ʱ��
}

void Init_all()							//��ʻǰ��ʼ��׼��
{
u8 i;
			for(i=0;i<8;i++)   			//��ˮ�Ƴ���*2
		{
			P1=~(0x01<<i);	 			//��1����iλ��Ȼ�󽫽��ȡ����ֵ��P1��
			delay(3000); 	
		}
			for(i=0;i<8;i++)
		{
			P1=~(0x01<<i);	 			//��1����iλ��Ȼ�󽫽��ȡ����ֵ��P1��
			delay(3000); 	
		}
		led_right=0;		 			//ǰ������LED��
		led_left=0;
		   for(i=0;i<200;i++)   		//������������
		   {
		   	 Beep_();
		   }
		   delay(5000);
		   for(i=0;i<200;i++)
		   {
		   	 Beep_();
		   }
		led_right=1;		 			//ǰ������LED��
		led_left=1;
}

void Beep_()			  //���������
{
Beep=~Beep;
delay(20);
}

void qianjin()	   			//ǰ��
{
M1=0;M2=1;
M3=0;M4=1;
led_right=0;
led_left=0;
Beep=1;
}

void houtui()	   			//����	 û���ù�
{
M1=1;M2=0;
M3=1;M4=0;

}
void zuozhuan()				//��ת
{
M1=0;M2=0;
M3=0;M4=1;
led_left=0;
led_right=1;
Beep=1;
}
void youzhuan()		 		//��ת
{
M1=0;M2=1;
M3=0;M4=0;
led_right=0;
led_left=1;
Beep=1;
}
void yuandi_zuozhuan()		//��ת	 û���ù�
{
M1=1;M2=0;
M3=0;M4=1;
led_left=0;
led_right=1;
Beep=1;
}
void yuandi_youzhuan()		 //��ת	  û���ù�
{
M1=0;M2=1;
M3=1;M4=0;
led_right=0;
led_left=1;
Beep=1;
}
void tingzhi()		 		//ֹͣ
{
M1=0;M2=0;
M3=0;M4=0;

}

void flash_on()				//��ǰ����������˸
{
 led_right=0;
 led_left=1;
 delay(10000);
 led_right=1;
 led_left=0;
 delay(10000);
}