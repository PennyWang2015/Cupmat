/**
   名称: 监督喝水 - 炫彩水健康
   作用: 利用传感器感应杯子是否在杯座上
        如果发现超过设定的时间(_t_delay, 单位:毫秒)杯子还在底座, 则判定为超时没喝水, 此时触发提醒装置(LED闪烁)
        当杯子离开底座, 则停止提醒
        杯子离开底座太久(>10min), 自动调低LED亮度
   版本:
      v1.0.2(IDE 1.8.9)
        - 杯子拿开过, 再放上来, 会显示一遍开机时的有杯子LED效果
        - 杯子没拿开, 每2分钟增加1颗LED低亮度点亮
        - 杯子没拿开并开始闪LED, 持续亮灯超过10分钟时, 关闭LED(重新计时30分钟)
        
      v1.0.1
        - 调整LED效果
        - 半小时提醒
        - 杯子拿开灭灯
        
      v1.0.0(beta)
        - 未测试完: 拿开杯子一段时间后调暗灯光

   传感器:
        感应杯子: 数字传感器, 压力传感器/红外距离传感器
        提醒设备: LED/蜂鸣器等
*/
//#include <avr/wdt.h>
#include <Adafruit_NeoPixel.h>
#define sensorPin A0      //压力传感器
int PIN = 10;             //LED

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
void colorWipe(uint32_t c, uint8_t wait);
void rainbow(uint8_t wait);
int getVib();
boolean drinking();
boolean countTime(long delaytime);
boolean countTime_bzout(long delaytime);

int buttonState = 0;
long _t_delay = 1800000;            //没喝水超时上限: 30分钟 = 1800000(30 * 60 * 1000)
long _t_count = 0;                  //杯子没拿计时器
long _t_count_2 = 0;                //计时器.2
long _t_count_3 = 0;                //计时器.3 用于计算连续闪灯持续的时间
byte _brightness = 5;               //LED亮度
boolean _drink = true;
boolean _bzout = false;             //杯子拿开
int _vlev = 40;                    //压力阀值, 原值:100(用于DFrobot产品), 低于此值表示压力过低(杯子拿走) 相当于 正在喝水

void setup() {
  //pinMode(pinSPEAK, OUTPUT);
  //Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //开机, 亮灯 > 1秒 > 熄灭
  stripON(strip.Color(100, 100, 100));    //点亮RGB
  delay(1000);
  colorWipe(strip.Color(0, 0, 0), 375);     //逐颗熄灭
  //stripOFF();                             //关闭RGB

  int vib = getVib();       //读取压力传感器值
  if (vib > _vlev) {
    colorWipe(strip.Color(100, 100, 100), 187);
    delay(100);
    colorWipe(strip.Color(0, 0, 0), 187);     //逐颗熄灭
  }

  //wdt_enable(WDTO_8S);
  //delay(10000);
}

void loop() {
  if (drinking()){
    //无杯子
    stripOFF();                             //关闭RGB

    //重置状态和定时器
    //Serial.println("reset");
    _drink = true;
    _bzout = true;
    _t_count = millis();                    //重置计时器
  }else{
    //有杯子

    //杯子拿开过, 再次放上来时显示效果
    if(_bzout){
      colorWipe(strip.Color(100, 100, 100), 60);
      delay(100);
      colorWipe(strip.Color(0, 0, 0), 60);     //逐颗熄灭
      _bzout = false;
    }
    
    if (countTime(_t_delay)) {
      _drink = false;
      _t_count_3 = millis();
      //Serial.println("***over time for drink***");
    }
    if (!_drink) {
      //有杯子超时(半小时), LED+SPEAK 闪烁+蜂鸣提醒
      flashAlert();

      //持续闪灯超过20分钟, 则关闭闪灯效果, 以防LED过热
      if(countTime_3(600000)){    //30min = 10 * 60 * 1000 = 600000
        //Serial.println("***over time (30min) for not drink***");
        _drink = true;
        stripOFF();                             //关闭RGB
      }
      
      delay(500);
    }else{
      
      //每隔2分钟增加点亮1颗
      long ot = (millis() - _t_count)/1000/60;    //杯子多久没拿开(分钟)
      ot = ot/2;        //计算出需要点亮多少颗(每隔2分钟增加1颗)
      if(ot>0){
//        if(countTime_2(120000)){
//          //_brightness+=5;
//          Serial.print("_brightness:");Serial.println(_brightness);
//        }
        colorWipeNum(strip.Color(_brightness, _brightness, _brightness), ot, 20);  // white
      }
      
    }
  }
}
