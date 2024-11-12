#include "buzzer.h"
#include "melodies.h"

#define LOG_TAG "Buzzer"

Buzzer::Buzzer(){

}
void Buzzer::begin(){
  pinMode(BUZZER_PIN, OUTPUT);
}
void Buzzer::playMelody(Melody *mld){
  // Move playing to separate thread
  if (mld==NULL)
    _playMelody(mldDefault);
  else
    _playMelody(mld);
}
void Buzzer::_playMelody(Melody *mld){
  if (mld == NULL)
  {
    ESP_LOGI("Melody can't be played. Reference to NULL", LOG_TAG);
    return;
  }
  int notes = mld->data_length/2;

  int wholenote = (60000 * 4) / mld->tempo;

  int divider = 0, noteDuration = 0;
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = mld->data[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(BUZZER_PIN, mld->data[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(BUZZER_PIN);
  }
}
Buzzer buzzer;