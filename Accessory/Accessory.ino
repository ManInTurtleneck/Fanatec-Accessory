// --- Fix for Arduino auto-prototype issue ---
struct EncRT;  // forward-declare so auto-generated prototypes can see the type

#include <Joystick.h>

// -------- Pins --------
const uint8_t BTN_PINS[6]     = {4,5,6,7,8,9};   // 6 momentaries -> buttons 0..5
const uint8_t TOGGLE_PIN      = 3;               // -> button 8
const uint8_t ENC_BTN_PINS[2] = {2,10};          // -> buttons 6,7

struct EncPins { uint8_t A, B; bool invert; };
EncPins enc1 = {16, 14, false}; // set invert=true if direction feels backwards
EncPins enc2 = {15, A0, false};

// -------- Joystick mapping --------
enum {
  BTN_1 = 0, BTN_2, BTN_3, BTN_4, BTN_5, BTN_6,   // 0..5 (pins 4..9)
  BTN_ENC1_PUSH,                                   // 6  (pin 2)
  BTN_ENC2_PUSH,                                   // 7  (pin 10)
  BTN_TOGGLE,                                      // 8  (pin 3)
  BTN_ENC1_CCW, BTN_ENC1_CW,                       // 9,10
  BTN_ENC2_CCW, BTN_ENC2_CW,                       // 11,12
};

Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
  13, 0,                  // buttons, hats
  false,false,false,      // X,Y,Z
  false,false,false,      // Rx,Ry,Rz
  false,false,false,false // Rudder,Throttle,Accelerator,Steering
);

// -------- Debounce helpers --------
struct Deb { uint8_t pin; bool state; bool last; unsigned long t; };
const uint16_t DB_MS = 10;
Deb debBtns[6], debEncBtns[2], debToggle;

inline bool pressed(uint8_t pin){ return digitalRead(pin)==LOW; }

void initDeb(Deb& d, uint8_t pin){
  d.pin=pin; d.last=pressed(pin); d.state=d.last; d.t=millis();
}

void serviceDeb(Deb& d, uint8_t joyBtn){
  bool r = pressed(d.pin);
  unsigned long now = millis();
  if (r!=d.last){ d.last=r; d.t=now; }
  if (now - d.t >= DB_MS){
    if (d.state != r){
      d.state = r;
      Joystick.setButton(joyBtn, d.state);
    }
  }
}

// -------- Pulsed outputs for encoder steps --------
struct Pulse { bool on=false; uint8_t btn=0; unsigned long offAt=0; };
Pulse pulses[4];
const uint16_t PULSE_MS = 150;

void pulse(uint8_t b){
  for (auto &p: pulses)
    if (!p.on){ p.on=true; p.btn=b; p.offAt=millis()+PULSE_MS; Joystick.setButton(b,1); return; }
  Joystick.setButton(b,1); Joystick.setButton(b,0); // fallback
}

void servicePulses(){
  unsigned long now=millis();
  for (auto &p: pulses)
    if (p.on && (long)(now-p.offAt)>=0){ Joystick.setButton(p.btn,0); p.on=false; }
}

// -------- Encoder (edge-on-A) --------
struct EncRT { bool lastA=false; unsigned long lastEdgeMs=0; };
EncRT e1, e2;
const uint8_t EDGE_GUARD_MS = 5; // bump to 3-5ms if double steps

void initEnc(const EncPins& e, EncRT& rt){
  pinMode(e.A, INPUT_PULLUP);
  pinMode(e.B, INPUT_PULLUP);
  rt.lastA = (digitalRead(e.A)==LOW);
}

void serviceEnc(const EncPins& e, EncRT& rt, uint8_t btnCCW, uint8_t btnCW){
  bool a = (digitalRead(e.A)==LOW);
  if (a != rt.lastA){
    unsigned long now = millis();
    if (now - rt.lastEdgeMs >= EDGE_GUARD_MS){
      rt.lastEdgeMs = now;
      bool b = (digitalRead(e.B)==LOW);
      bool cw = (b != a);       // direction
      if (e.invert) cw = !cw;
      pulse(cw ? btnCW : btnCCW);
    }
    rt.lastA = a;
  }
}

void setup(){
  // Inputs
  for (uint8_t i=0;i<6;i++){ pinMode(BTN_PINS[i], INPUT_PULLUP); initDeb(debBtns[i], BTN_PINS[i]); }
  for (uint8_t i=0;i<2;i++){ pinMode(ENC_BTN_PINS[i], INPUT_PULLUP); initDeb(debEncBtns[i], ENC_BTN_PINS[i]); }
  pinMode(TOGGLE_PIN, INPUT_PULLUP); initDeb(debToggle, TOGGLE_PIN);

  // Encoders
  initEnc(enc1, e1);
  initEnc(enc2, e2);

  // Start HID
  Joystick.begin(true);
}

void loop(){
  // Buttons/toggle
  for (uint8_t i=0;i<6;i++) serviceDeb(debBtns[i], BTN_1+i);
  serviceDeb(debEncBtns[0], BTN_ENC1_PUSH);
  serviceDeb(debEncBtns[1], BTN_ENC2_PUSH);
  serviceDeb(debToggle,     BTN_TOGGLE);

  // Encoders → pulses
  serviceEnc(enc1, e1, BTN_ENC1_CCW, BTN_ENC1_CW);
  serviceEnc(enc2, e2, BTN_ENC2_CCW, BTN_ENC2_CW);

  servicePulses();
  delay(1);
}
