#ifndef DIRECT7SEG_H__
#define DIRECT7SEG_H__

#include <arduino.h>

struct AnodeCommon;
struct CathodeCommon;

template<byte VALUE, typename DEVICE_TYPE> struct D7SegBits;
template<byte VALUE> struct D7SegBits<VALUE, AnodeCommon> { static const int value = VALUE ^ 0xff; };
template<byte VALUE> struct D7SegBits<VALUE, CathodeCommon> { static const int value = VALUE; };

template<typename DEVICE_TYPE> struct D7SegLevel;
template<>
struct D7SegLevel<AnodeCommon>
{
  static const bool Enable = false;
  static const bool Disable = true;
};
template<>
struct D7SegLevel<CathodeCommon>
{
  static const bool Enable = true;
  static const bool Disable = false;
};

template<typename DEVICE_TYPE>
struct D7SegConstant {
  static const byte Digits[16];

  static const bool Enable = D7SegLevel<DEVICE_TYPE>::Enable;
  static const bool Disable = D7SegLevel<DEVICE_TYPE>::Disable;
};

template<typename DEVICE_TYPE>
const byte D7SegConstant<DEVICE_TYPE>::Digits[16] = {
  D7SegBits<B00111111, DEVICE_TYPE>::value, // 0
  D7SegBits<B00000110, DEVICE_TYPE>::value, // 1
  D7SegBits<B01011011, DEVICE_TYPE>::value, // 2
  D7SegBits<B01001111, DEVICE_TYPE>::value, // 3
  D7SegBits<B01100110, DEVICE_TYPE>::value, // 4
  D7SegBits<B01101101, DEVICE_TYPE>::value, // 5
  D7SegBits<B01111101, DEVICE_TYPE>::value, // 6
  D7SegBits<B00000111, DEVICE_TYPE>::value, // 7
  D7SegBits<B01111111, DEVICE_TYPE>::value, // 8
  D7SegBits<B01101111, DEVICE_TYPE>::value, // 9
  D7SegBits<B01110111, DEVICE_TYPE>::value, // A
  D7SegBits<B01111100, DEVICE_TYPE>::value, // b
  D7SegBits<B00111001, DEVICE_TYPE>::value, // C
  D7SegBits<B01011110, DEVICE_TYPE>::value, // d
  D7SegBits<B01111001, DEVICE_TYPE>::value, // E
  D7SegBits<B01110001, DEVICE_TYPE>::value  // F
};

template<int FIG1_PIN, int FIG2_PIN, typename DEVICE_TYPE>
class D7Seg {
public:
  typedef D7SegConstant<DEVICE_TYPE> Constant;

  static const int Fig1Pin = FIG1_PIN;
  static const int Fig2Pin = FIG2_PIN;

  void init() const {
    DDRD = B11111111;
    PORTD = 0;

    pinMode(Fig1Pin, OUTPUT);
    pinMode(Fig2Pin, OUTPUT);
  }

  void showDec(byte value, bool dot1, bool dot2, unsigned long interval = 0) const {
    byte fig1 = (value / 10) % 10;
    byte fig2 = value % 10;

    showSegments(Constant::Digits[fig1] ^ dotBit(dot1), Constant::Digits[fig2] ^ dotBit(dot2), interval);
  }

  void showHex(byte value, bool dot1, bool dot2, unsigned long interval = 0) const {
    byte fig1 = value / 16;
    byte fig2 = value % 16;

    showSegments(Constant::Digits[fig1] ^ dotBit(dot1), Constant::Digits[fig2] ^ dotBit(dot2), interval);
  }

  void showSegments(byte fig1, byte fig2, unsigned long interval = 0) const {
    showSegment(Fig1Pin, Fig2Pin, fig1);
    delay(interval);
    showSegment(Fig2Pin, Fig1Pin, fig2);
    delay(interval);
  }

  void showSegment(int pinOn, int pinOff, byte fig) const {
    digitalWrite(pinOn, Constant::Enable);
    digitalWrite(pinOff, Constant::Disable);
    PORTD = fig;
  }

private:
  byte dotBit(bool dot) const {
    return dot ? 0x80 : 0x00;
  }
};

#endif//DIRECT7SEG_H__
