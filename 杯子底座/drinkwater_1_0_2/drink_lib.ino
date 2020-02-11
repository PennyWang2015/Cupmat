
//判断是否正在喝水
boolean drinking(){
  int vib = getVib();       //读取压力传感器值
  Serial.println(vib);
  if (vib < _vlev) {
    return true;
  }
  return false;       //没喝水
}

//获取压力传感器值, 连测3次, 返回最大值
int getVib(){
  int vib = analogRead(sensorPin);delay(10);
  int vib2 = analogRead(sensorPin);delay(10);
  int vib3 = analogRead(sensorPin);
  if(vib2>vib){vib=vib2;}
  if(vib3>vib){vib=vib3;}
  return vib;
}

//灯带打开, 可指定颜色(strip.Color(0, 0, 255))
void stripON(uint32_t c){
  colorWipe(c, 10);                       //某些灯带如果延迟是0的话, 会控制失误
}
//灯带关闭
void stripOFF(){
  colorWipe(strip.Color(0, 0, 0), 10);    //某些灯带如果延迟是0的话, 会控制失误
  colorWipe(strip.Color(0, 0, 0), 10);
}
//灯带提醒喝水
void flashAlert() {
  colorWipe(strip.Color(255, 0, 0), 30);      // Red
  if (drinking()){return;}          //中途检测到杯子拿开, 停止闪灯
  colorWipe(strip.Color(0, 255, 0), 20);      // Green
  if (drinking()){return;}          //中途检测到杯子拿开, 停止闪灯
  colorWipe(strip.Color(0, 0, 255), 10);      // Blue
  if (drinking()){return;}          //中途检测到杯子拿开, 停止闪灯
  colorWipe(strip.Color(255, 255, 255), 20);  // white
  if (drinking()){return;}          //中途检测到杯子拿开, 停止闪灯
  colorWipe(strip.Color(0, 0, 0), 100);       // off
  if (drinking()){return;}          //中途检测到杯子拿开, 停止闪灯
  rainbow(20);
}

// Fill the dots one after the other with a color
//逐个点亮(可指定颜色/间隔时间)
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
//逐个点亮(可指定颜色/数量/间隔时间)
void colorWipeNum(uint32_t c, uint16_t num, uint8_t wait) {
  for (uint16_t i = 0; i < num; i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

/**
   非阻塞的计时函数.检查多久没喝水
   param delaytime 需要计算的时间，单位：毫秒
*/
boolean countTime(long delaytime) {
  if (_t_count == 0) {
    _t_count = millis();
  }
  if ((millis() - _t_count) >= delaytime) {
    _t_count = millis();
    return true;
  }
  return false;
}

/**
   非阻塞的计时函数.2
   param delaytime 需要计算的时间，单位：毫秒
*/
boolean countTime_2(long delaytime) {
  if (_t_count_2 == 0) {
    _t_count_2 = millis();
  }
  if ((millis() - _t_count_2) >= delaytime) {
    _t_count_2 = millis();
    return true;
  }
  return false;
}

/**
   非阻塞的计时函数.3
   param delaytime 需要计算的时间，单位：毫秒
*/
boolean countTime_3(long delaytime) {
  if (_t_count_3 == 0) {
    _t_count_3 = millis();
  }
  if ((millis() - _t_count_3) >= delaytime) {
    _t_count_3 = millis();
    return true;
  }
  return false;
}
