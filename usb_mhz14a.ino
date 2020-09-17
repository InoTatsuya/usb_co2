#include <SoftwareSerial.h>

#define CALIB_TIME 30 // per min

//SoftwareSerial mySerial(14, 15); // RX, TX for WNPink
//SoftwareSerial mySerial(16, 10); // RX, TX for ProMicro
SoftwareSerial mySerial(6, 5); // RX, TX for Seeduino XIAO

static byte command_get_co2[] = {
  0xFF,  // Byte 0
  0x01,  // Byte 1
  0x86,  // Byte 2
  0x00,  // Byte 4
  0x00,  // Byte 5
  0x00,  // Byte 6
  0x00,  // Byte 7
  0x00,  // Byte 8
  0x79   // Byte 9
};

static byte command_abc_off[] = {
  0xFF,  // Byte 0
  0x01,  // Byte 1
  0x79,  // Byte 2
  0x00,  // Byte 4
  0x00,  // Byte 5
  0x00,  // Byte 6
  0x00,  // Byte 7
  0x00,  // Byte 8
  0x86   // Byte 9
};

static byte command_abc_on[] = {
  0xFF,  // Byte 0
  0x01,  // Byte 1
  0x79,  // Byte 2
  0xA0,  // Byte 4
  0x00,  // Byte 5
  0x00,  // Byte 6
  0x00,  // Byte 7
  0x00,  // Byte 8
  0xE6   // Byte 9
};

static byte command_zero_calibration[] = {
  0xFF,  // Byte 0
  0x01,  // Byte 1
  0x87,  // Byte 2
  0x00,  // Byte 4
  0x00,  // Byte 5
  0x00,  // Byte 6
  0x00,  // Byte 7
  0x00,  // Byte 8
  0x78   // Byte 9
};

void usb_mhz14a_init(){
  mySerial.begin(9600);
  delay(10);
  usb_mhz14a_abc_off();
  delay(10);
}

int usb_mhz14a_get_co2(){
  mySerial.write(command_get_co2, sizeof(command_get_co2));

  delay(10);

  int c = 0;
  int message[9];

  while (mySerial.available()) {
    int b = mySerial.read();
    if ((c == 0 && b == 0xFF) || (c >= 1 && c <= 8)) {
      message[c] = b;
      c++;
    }
  }

  int high_byte = message[2];
  int low_byte = message[3];
  int value = high_byte * 256 + low_byte;

  return value;
}

void usb_mhz14a_abc_off(){
  mySerial.write(command_abc_off, sizeof(command_abc_off));

  delay(10);
}

void usb_mhz14a_abc_on(){
  mySerial.write(command_abc_off, sizeof(command_abc_off));

  delay(10);
}

void usb_mhz14a_zero_calibration(){
  int cnt = 0;

  for( cnt = 0; cnt < ( CALIB_TIME * 60 ); cnt++){
    usb_mhz14a_get_co2();
    delay(1000);
  }

  mySerial.write(command_zero_calibration, sizeof(command_zero_calibration));

  delay(10);
}
