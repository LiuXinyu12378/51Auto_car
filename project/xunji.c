#include "reg52.h"			 	//此文件中定义了单片机的一些特殊功能寄存器

typedef unsigned int u16;	  	//对数据类型进行声明定义
typedef unsigned char u8;
unsigned char code LXY[]={ 	//共阳极0~f数码管编码
0x02,0x9e,0x24,0x0c,0x98,0x48,0x40,0x1e,0x01,0x18
};

sbit M1=P0^0;				  //定义电机接口 *4
sbit M2=P0^1;
sbit M3=P0^2;
sbit M4=P0^3;
sbit Beep=P0^6;				  //定义蜂鸣器接口
sbit led_right=P0^7;		  //右前灯
sbit led_left=P2^0;			  //左前灯
sbit zuo=P3^5;				  //左接收管
sbit you=P3^6;				  //中接收管
sbit zhong=P3^7;			  //又接收管

int timer=0;				  //定时器1ms，timer加1
void delay(u16 i)			  //延时函数
{
	while(i--);	
}

void Beep_();				  //声明函数
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
void main()					   //主函数	 开机先进行两次流水灯，车前的两个灯闪烁一次，然后蜂鸣器响两次 ，启动定时器中断车可以行走，主函数流水灯
{
	int n=0,i;
	Init_all();
	timer0_init();
	while(1)
	{	
   	P1=0xff;				   //8个灯闪烁
	delay(10000);
	P1=0x00;
	delay(10000);
	P2=LXY[n++]&0xfe;
	if(n==10) n=0;
			
	}		
}

void GO_() interrupt 1				  //定时器中断函数
{
int c=4;							 //速度占空比调节 高占空比(10-c)/10
int b=7;							 //拐弯时速度占空比
timer++;
if(timer>10)	timer=0; 	
 TH0=(65536-1000)/256; 				//  1MS定时 1000us
 TL0=(65536-1000)%256; 
 if(zuo==0&zhong==0&you==0)		  		   //000	当三个都检测到白色停止		   1代表监测到黑色，0代表监测到白色
	 {
	   	tingzhi();
		flash_on();
		P1=0x00;
		Beep_();
		   
	 }
 	 	 if(zuo==0&zhong==0&you==1)		  //001		 左边两个探头检测到白色，右边检测到黑色右转
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==b)
	   yuandi_youzhuan();
	 }
	  	 if(zuo==0&zhong==1&you==0)		  //010		  只有中间的探头检测到黑色，前进
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==c)
	   qianjin();
	 }
	  	 if(zuo==0&zhong==1&you==1)		  //011		   左边一个探头检测到白色，右边两个探头检测到黑色右转
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==b)
	 	youzhuan();
	 }
	  	 if(zuo==1&zhong==0&you==0)		  //100		   左边一个检测到黑色，右边两个检测白色左转
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==b)
	 	yuandi_zuozhuan();
	 }
	  	 if(zuo==1&zhong==0&you==1)		  //101			左右均检测到黑色，但中间检测到黑色，理论上这种状况是不存在的，
	 {
	 	if(timer==0)					  //            但是实际可能因为误差存在导致这种情况发生，所以我让它继续前进
		tingzhi();
		if(timer==c)
	 	qianjin();
	 }
	  	 if(zuo==1&zhong==1&you==0)		  //110			左边两个检测到黑色，右边一个检测到白色，左转
	 {
	 	if(timer==0)
		tingzhi();
		if(timer==b)
	 	  zuozhuan();
	 }
	  	 if(zuo==1&zhong==1&you==1)		   //111		 只有当三个探头都检测到黑线时才前进
	 {

		if(timer==0)
		tingzhi();
		if(timer==c)
	  	qianjin();
	 }
}  

void timer0_init()						//定时器初始化
{
   TMOD = 0x01;  						//16位定时器模式
    
 TH0=(65536-1000)/256; 					//  1MS定时 1000us
 TL0=(65536-1000)%256; 
    
    EA = 1;       						//开全局中断
    ET0 = 1;      						//允许T0中断
    TR0 = 1;      						//启动定时器
}

void Init_all()							//行驶前初始化准备
{
u8 i;
			for(i=0;i<8;i++)   			//流水灯程序*2
		{
			P1=~(0x01<<i);	 			//将1右移i位，然后将结果取反赋值到P1口
			delay(3000); 	
		}
			for(i=0;i<8;i++)
		{
			P1=~(0x01<<i);	 			//将1右移i位，然后将结果取反赋值到P1口
			delay(3000); 	
		}
		led_right=0;		 			//前面两个LED亮
		led_left=0;
		   for(i=0;i<200;i++)   		//蜂鸣器响两次
		   {
		   	 Beep_();
		   }
		   delay(5000);
		   for(i=0;i<200;i++)
		   {
		   	 Beep_();
		   }
		led_right=1;		 			//前面两个LED灭
		led_left=1;
}

void Beep_()			  //蜂鸣器尖叫
{
Beep=~Beep;
delay(20);
}

void qianjin()	   			//前进
{
M1=0;M2=1;
M3=0;M4=1;
led_right=0;
led_left=0;
Beep=1;
}

void houtui()	   			//后退	 没调用过
{
M1=1;M2=0;
M3=1;M4=0;

}
void zuozhuan()				//左转
{
M1=0;M2=0;
M3=0;M4=1;
led_left=0;
led_right=1;
Beep=1;
}
void youzhuan()		 		//右转
{
M1=0;M2=1;
M3=0;M4=0;
led_right=0;
led_left=1;
Beep=1;
}
void yuandi_zuozhuan()		//左转	 没调用过
{
M1=1;M2=0;
M3=0;M4=1;
led_left=0;
led_right=1;
Beep=1;
}
void yuandi_youzhuan()		 //右转	  没调用过
{
M1=0;M2=1;
M3=1;M4=0;
led_right=0;
led_left=1;
Beep=1;
}
void tingzhi()		 		//停止
{
M1=0;M2=0;
M3=0;M4=0;

}

void flash_on()				//车前的两个灯闪烁
{
 led_right=0;
 led_left=1;
 delay(10000);
 led_right=1;
 led_left=0;
 delay(10000);
}