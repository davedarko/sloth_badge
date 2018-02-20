#include <avr/sleep.h>
#define nop() asm volatile("nop")
boolean leds_on = true;

byte badly_programmed_counter = 0;
byte animation_counter_max = 5;
byte animation_counter = 0;

// we need two frames with 8 bits for 12 LEDs
byte frame1 = 0;
byte frame2 = 0;

// animation where every third LED is glowing
// I could shift that continuosly and drop 4 bytes
const byte three_apart_length = 6;
const byte three_apart[three_apart_length] PROGMEM = {
  0b10010010, 0b01000000,
  0b01001001, 0b00100000,
  0b00100100, 0b10010000,
};

// this one is huge
// is there a math function to do that instead?
const byte updown_length = 14;
const byte updown[updown_length] PROGMEM = {
  0b00100000, 0b00000000,
  0b01010000, 0b00000000,
  0b10001000, 0b00000000,
  0b00000100, 0b00010000,
  0b00000010, 0b00100000,
  0b00000001, 0b01000000,
  0b00000000, 0b10000000,
};

// that's just blinking each led 
// maybe inverting the data bitwise does the trick
const byte two_apart_length = 4;
const byte two_apart[two_apart_length] PROGMEM = {
  0b10101010, 0b10100000,
  0b01010101, 0b01010000,
};

// should have taken a switch, sleep stuff is huge
ISR(INT0_vect) {
  sleep_disable();
  leds_on = true;
}

void setup()
{
  // this is for the button setup
  PORTB |= (1 << PB1);
  DDRB &= ~(1 << PB1);

  shutdown_chip();
}

void loop()
{
  if (leds_on)
  {
    cli();
    leds_on = false;

    // I call it that, because the millis stuff
    // and the interrupts collided and broke
    badly_programmed_counter = 0;
  }

  // fancy way of checking the button state
  if (!(PINB & (1 << PB1)))
  {
    badly_programmed_counter++;
    my_100ms_delay();

    // check if button was released before time
    // so animation index can be incremented
    // maybe a modulo with animation_counter_max
    // safes space
    if ( (PINB & (1 << PB1)) )
    {
      badly_programmed_counter = 0;
      animation_counter++;
      if (animation_counter == animation_counter_max)
      {
        animation_counter = 0;
      }
      charlie(animation_counter);

      // calling a function instead of writing it down
      // 5 times helps already
      for (byte b = 0; b < 5; b++) my_100ms_delay();
    }

    if (badly_programmed_counter >= 7)
    {
      // blink when shutdown is immanent
      for (byte b = 0; b < 5; b++) blip();
      shutdown_chip();
    }
  }
  else
  {
    animate_leds();
    badly_programmed_counter = 0;
  }
}

// this is probably already at it's best
void charlie (byte var) {
  leds_off();

  switch (var) {
    case 0: // 1
    // this one doesn't matter in Arduino
    // DDRB |= (0b00000101); // output
      DDRB |= (1 << PB0) | (1 << PB2); // output
      PORTB |= (1 << PB0); // HIGH
      break;
    case 1: // 2
      DDRB |= (1 << PB0) | (1 << PB2); // output
      PORTB |= (1 << PB2); // HIGH
      break;
    case 2: // 3
      DDRB |= (1 << PB2) | (1 << PB3); // output
      PORTB |= (1 << PB2); // HIGH
      break;
    case 3: // 4
      DDRB |= (1 << PB2) | (1 << PB3); // output
      PORTB |= (1 << PB3); // HIGH
      break;
    case 4: // 5
      DDRB |= (1 << PB3) | (1 << PB4); // output
      PORTB |= (1 << PB3); // HIGH
      break;
    case 5: // 6
      DDRB |= (1 << PB3) | (1 << PB4); // output
      PORTB |= (1 << PB4); // HIGH
      break;
    case 6: // 7
      DDRB |= (1 << PB0) | (1 << PB3); // output
      PORTB |= (1 << PB0); // HIGH
      break;
    case 7: // 8
      DDRB |= (1 << PB0) | (1 << PB3); // output
      PORTB |= (1 << PB3); // HIGH
      break;
    case 8: // 9
      DDRB |= (1 << PB2) | (1 << PB4); // output
      PORTB |= (1 << PB2); // HIGH
      break;
    case 9: // 10
      DDRB |= (1 << PB2) | (1 << PB4); // output
      PORTB |= (1 << PB4); // HIGH
      break;
    case 10: // 11
      DDRB |= (1 << PB0) | (1 << PB4); // output
      PORTB |= (1 << PB0); // HIGH
      break;
    case 11: // 12
      DDRB |= (1 << PB0) | (1 << PB4); // output
      PORTB |= (1 << PB4); // HIGH
      break;
    default:
      ;

      nop();
  }
}

void shutdown_chip()
{
  animation_counter = 0;
  leds_off();

  /* Clear WDRF in MCUSR */
  MCUSR &= ~(1 << WDRF);
  /* Write logical one to WDCE and WDE */
  /* Keep old prescaler setting to prevent unintentional time-out */
  WDTCR |= (1 << WDCE) | (1 << WDE);
  /* Turn off WDT */
  WDTCR = 0x00;

  ADCSRA &= ~(1 << ADEN); // turn off adc

  GIMSK |= (1 << INT0);
  GIMSK |= (1 << PCIE);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sei(); // Enable global interrupts
  sleep_cpu();
  sleep_disable();
}

void animate_leds()
{
  switch (animation_counter)
  {
    case 0:
      animate_frames(
        three_apart_length,
        three_apart,
        512
      );
      break;

    case 1:
      for (byte acc_counter = 1; acc_counter < 64; acc_counter += 2)
      {
        animate_frames(
          three_apart_length,
          three_apart,
          acc_counter * 8
        );
      }
      break;

    case 2:
      animate_frames(
        updown_length,
        updown,
        512
      );
      break;

    case 3:
      animate_frames(
        two_apart_length,
        two_apart,
        2048
      );
      break;

    case 4:
      for (byte acc_counter = 1; acc_counter < 64; acc_counter += 2)
      {
        animate_frames(
          updown_length,
          updown,
          (63-acc_counter) * 4
        );
      }
      break;

    default:
      leds_off();
      break;
  }
}

void animate_frames(
  byte animation_length,
  const byte* animation_array,
  uint16_t frame_duration
) {
  for (byte i = 0; i < animation_length; i += 2)
  {
    // save the last time you blinked the LED
    frame1 = pgm_read_byte_near(animation_array + i);
    frame2 = pgm_read_byte_near(animation_array + i + 1);
    show_frame(frame_duration);
  }
}

// this just looks ugly, but needs to be done
// for loop through frame bytes to set the LEDs with 
// the charlie plex function
void show_frame(uint16_t byteframe_length)
{
  for (uint16_t j = 0; j < byteframe_length; j++)
  {
    //iterate through bit mask
    for (byte i = 0; i < 12; i++)
    {
      // if bitwise AND resolves to true
      if (
        (
          i < 8 &&
          1 << i & frame1
        ) ||
        (
          i > 7 &&
          1 << (i - 4) & frame2
        )
      )
      {
        charlie(i);
      }
      else
      {
        charlie(255);
      }
    }
  }
}

void leds_off()
{
  DDRB &= ~(0b00011101);
  PORTB &= ~(0b00011101);
}

void blip()
{
  charlie(11);
  // delay(100);
  my_100ms_delay();
  charlie(255);
  // delay(100);
  my_100ms_delay();
}

void my_100ms_delay()
{
  for (uint32_t i=0;i<40000;i++) nop();
}
