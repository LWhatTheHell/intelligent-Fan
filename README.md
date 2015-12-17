#人体感应、温度控制智能无叶风扇
##1.功能
	1.人体感应，人来风起，人走风停
	2.温度显示
	3.根据温度自动调节风扇转速
##2.缺陷
	1.人体静止不动时感应不到
		 热释电红外感应模块特性
	2.温度40秒更新一次，更新时风扇停转1秒
		风扇调速通过定时器产生PWM，由于DS18B20读取温度时对时序要求极高，必须关掉定时器
##3.使用元件
	1.单片机最小系统
		STC89C52RC 1个
		12Mhz晶振 1个
		30pF瓷片电容 2个
		10uF电解电容 1个
		10k欧电阻 1个
	2.DS18B20温度传感器 1个
	3.HC-SR501热释电人体红外感应模块 1个
	4.5611AH共阴数码管 2个
	5.12V四线风扇 2个 （组成一个）
	6.12V电源适配器 1个
	7.AMS1117-5.0降压模块 1个
	8.外壳
##4.连接
	DS18B20输出脚 P3^0
	HC-SR501输出脚 P3^1
	两个数码管按ABCDEFGDP接在P1和P2上
	风扇PWM调速线 P3^2 P3^2