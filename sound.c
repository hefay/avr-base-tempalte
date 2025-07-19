#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "notes.h"

#define SPEAKER_PIN PB0

void setup_timer(long note) {
    // Set PB0 as output
    DDRB |= (1 << SPEAKER_PIN);

    // Calculate the timer compare value for the desired note frequency
    long compare_value = (F_CPU / (2 * note)) - 1;

    // Set Timer/Counter0 to CTC mode
    TCCR0A = (1 << WGM01);

    // Set compare value
    OCR0A = (uint8_t)compare_value;

    // Enable output compare interrupt
    TIMSK0 |= (1 << OCIE0A);

    // Start the timer with a prescaler of 1
    TCCR0B = (1 << CS00);
}

void play(long note, long length) {
    // Setup the timer for the desired note
    setup_timer(note);

    // Enable global interrupts
    sei();

    // Wait for the duration of the note
    for (long i = 0; i < length; i++) {
        _delay_ms(1);
    }

    // Disable the timer
    TCCR0B &= ~(1 << CS00);

    // Disable the interrupt
    TIMSK0 &= ~(1 << OCIE0A);
}

ISR(TIM0_COMPA_vect) {
    // Toggle the speaker pin
    PORTB ^= (1 << SPEAKER_PIN);
}

void play_mario_theme() {
    play(NOTE_E5, EIGHTH_NOTE_MS);
    play(NOTE_E5, EIGHTH_NOTE_MS);
    _delay_ms(EIGHTH_NOTE_MS);
    play(NOTE_E5, EIGHTH_NOTE_MS);
    _delay_ms(EIGHTH_NOTE_MS);
    play(NOTE_C5, EIGHTH_NOTE_MS);
    play(NOTE_E5, EIGHTH_NOTE_MS);
    _delay_ms(EIGHTH_NOTE_MS);
    play(NOTE_G5, QUARTER_NOTE_MS);
    _delay_ms(QUARTER_NOTE_MS);
    play(NOTE_G4, QUARTER_NOTE_MS);
    _delay_ms(QUARTER_NOTE_MS);

    play(NOTE_C5, EIGHTH_NOTE_MS);
    _delay_ms(SIXTEENTH_NOTE_MS);
    play(NOTE_G4, EIGHTH_NOTE_MS);
    _delay_ms(SIXTEENTH_NOTE_MS);
    play(NOTE_E4, EIGHTH_NOTE_MS);
    _delay_ms(SIXTEENTH_NOTE_MS);
    play(NOTE_A4, EIGHTH_NOTE_MS);
    play(NOTE_B4, EIGHTH_NOTE_MS);
    play(NOTE_AS4, SIXTEENTH_NOTE_MS);
    play(NOTE_A4, EIGHTH_NOTE_MS);
    play(NOTE_G4, SIXTEENTH_NOTE_MS);

    play(NOTE_E5, EIGHTH_NOTE_MS);
    play(NOTE_G5, QUARTER_NOTE_MS);
    play(NOTE_A5, HALF_NOTE_MS);
}

int main() {
    while (1) {
        play_mario_theme();
        _delay_ms(1000); // Pause between repetitions
    }
}
