#include <CanSatKit.h>

int counter = 1;
bool led_state = false;
const int led_pin = 13;


class FramePrinter : public Print {
 public:
  int size;
  char buf[256];

  void flush() {
    size = 0;
    buf[0] = '\0';
  }

  virtual size_t write(uint8_t x) {
    buf[size++] = x;
    return 1;
  }

  void send() {
    CanSatKitRadio.transmit(buf, size);
    flush();
  }
};

FramePrinter frame;

  
void setup() {
  SerialUSB.begin(115200);
  pinMode(led_pin, OUTPUT);

  CanSatKitRadio.begin(CanSatPins_Radio_CS,
                       CanSatPins_Radio_DIO0,
                       433.0,
                       Bandwidth_125000_Hz,
                       SpreadingFactor_9,
                       CodingRate_4_8);
}

void loop() {
  digitalWrite(led_pin, led_state);
  led_state = !led_state;

  frame.print(counter);
  counter++;
  frame.print(". Hello CanSat!");

  frame.send();

  delay(1000);
}