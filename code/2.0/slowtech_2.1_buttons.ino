// This project is based on the Teensy Monster 1.0: https://github.com/FuzzyWobble/TeensyMonster
// License: http://opensource.org/licenses/MIT
// Edit: Imbecillen / Sebastian Galo 2018

// Use name.c as tab in the sketch to show custom midi name
// Distilled code for the project tested with 16 buttons, communication working fine. 
// Maybe chashing due to midi overflow?
// Button mode 4 sending messages directly when pressed instead of released.




// ======================================================================================
// SETTINGS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//CHANNEL_____________________________________________
int channelNumber = 1; //each controller should have a unique channel number between 1 and 10



//DEBOUNCE BUTTONS___________________________________
//debounce is a measurement of the time in which a pushbutton is unresponsive after it is pressed
//this is important to prevent unwanted double clicks
uint32_t pbBounce = 300; //150 millisecond debounce duration - you may want to change this value depending on the mechanics of your pushbuttons



//MULTIPLEXER READING_________________________________
//CD4067BE - http://www.ti.com/lit/ds/symlink/cd4067b.pdf
//'1' for multiplexer I/O you want to send signal, else enter '0'

//MULTIPLEXER 5, DIGITAL FOR PUSHBUTTONS______________
int toReadDigitalMux5[16] = {
  //IC pin number are written below
  1, 1, 1, 1, //0-3
  1, 1, 1, 1, //4-7
  1, 1, 1, 1, //8-11
  1, 1, 1, 1 //12-15
};



//PUSHBUTTON MODE______________________________________
//there are a few different modes in which you may wish for your pushbutton to behave
//'1' - standard mode, when pushbutton is engaged note is turned on, when pushbutton is released, note is turned off
//'2' - on mode, note is only turned on with each click
//'3' - off mode, note is only turned off with each click
//'4' - toggle mode, note is switched between on and off with each click
int pushbuttonMode[32] = {
  //Pin number are written below
  4, 4, 4, 4, //0-3
  4, 4, 4, 4, //4-7
  4, 4, 4, 4, //8-11
  4, 4, 4, 4 //12-15 (NOTE: comma after last row/number for shift function to continue down under)

};






// ======================================================================================
// VARIABLES AND FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//MULTIPLEXER CONTROL SIGNAL____________________________
int s0 = 1; //control pin A
int s1 = 2; //control pin B
int s2 = 3; //control pin C
int s3 = 4; //control pin D



//MULTUPLEXER INHIBITOR PIN_____________________________
int INH_pin = 9; 



//MULTIPLEXER NR 5 _____________________________________
//DIGITAL IN, SIGNAL PIN FOR BUTTONS
//(16 buttons in total)
int SIG_pin5 = 6; //digital mux read pin for muxButtons



//MULTIPLEXER NR 5, DIGITAL IN FOR PUSH BUTTONS_________
unsigned long timeHit[16]; //16*2 = 32 for use with shift button code
boolean buttonState[16]; //array with stored state: if the button was last turned on or off
boolean tempDigitalRead5 = 0;//array to hold previously read values
int controlPin5[] = {s0, s1, s2, s3}; //set contol pins in array
//control array
int muxChannel5[16][4] = {{0, 0, 0, 0}, {1, 0, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 1, 0}, 
{1, 0, 1, 0}, {0, 1, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 1}, {1, 0, 0, 1}, {0, 1, 0, 1}, {1, 1, 0, 1}, 
{0, 0, 1, 1}, {1, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};
//function to read mux
int readMux5(int channel5) {
  //loop through the four control pins
  for (int i = 0; i < 4; i ++) {
    //turn on/off the appropriate control pins according to what channel we are trying to read
    digitalWrite(controlPin5[i], muxChannel5[channel5][i]);
  }
  //read the value of the pin
  int val = digitalRead(SIG_pin5);
  //return the value
  return val;
}






// ======================================================================================
// SETUP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup() {

//AREF_______________________________________________
  // Voltage reference for analog input, MUST BE BEFORE analogRead! 
  analogReference(EXTERNAL);

  
//MULTIPLEXER SETUP _________________________________
  //set multiplexers reading pin
  pinMode(SIG_pin5, INPUT_PULLUP); 

  //set inhibit pin to output and turn off
  pinMode(INH_pin, OUTPUT);
  digitalWrite(INH_pin, LOW); 
  
  //set our control pins to output
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  
  //turn all control pins off (for now)
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

} // SETUP END






// ======================================================================================
// LOOP ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {
  


//MULTIPLEXER 5
//DIGITAL IN FOR BUTTONS ________________________________
  for (int i = 0; i < 16; i++) { //loop through 16 mux channels
      if (toReadDigitalMux5[i] == 1) { //we read the choosen mux channel input
        tempDigitalRead5 = readMux5(i);
        if (pushbuttonMode[i] == 1 && tempDigitalRead5 != buttonState[i]) { //___NORMAL MODE (1)
          delay(20); //just a delay for noise to ensure push button was actually hit
          if (readMux5(i) == tempDigitalRead5) { //check if pushbutton is still the same
            if (tempDigitalRead5 == LOW) { //button pressed, turn note on
              midiSend('p', 1, i); //call note on/off function
            } else { //button released
              midiSend('p', 0, i);
            }
            buttonState[i] = tempDigitalRead5; //update the state (on or off)
          }
        } else { //___ALL OTHER MODES (2,3,4)
          if (readMux5(i) == LOW && (millis() - timeHit[i]) > pbBounce) { //check bounce time
            if (pushbuttonMode[i] == 2) { //mode 2 - only note on
              midiSend('p', 1, i);
            } else if (pushbuttonMode[i] == 3) { //mode 3 - only note off
              midiSend('p', 0, i);
            } else { //mode 4 - toggle
              if (buttonState[i] == 1) { //on->off
                midiSend('p', 0, i);
                buttonState[i] = 0;
              } else { //off->on
                midiSend('p', 1, i);
                buttonState[i] = 1;
              }
            }
            timeHit[i] = millis();
          }
        }
      }
    }







    
} // LOOP END



// ======================================================================================
// COMMUNICATION FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// http://www.pjrc.com/teensy/td_midi.html

//FUNCTIONS TO SEND MIDI______________________________ 
void midiSend(char type, int val, int pin) {
  switch (type) {

    case 'p': //--------------- PUSHBUTTON
        if (val == 1) {
          usbMIDI.sendNoteOn(pin, 127, channelNumber); //!!!
        } else {
          usbMIDI.sendNoteOff(pin, 127, channelNumber); //!!!
        }
      break;
    }
  }


