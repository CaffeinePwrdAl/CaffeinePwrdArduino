/*************************************************
 * Arduino Nano - 328 Old Bootloader
 *************************************************/


#define NOTE_C  0
#define NOTE_CS 1
#define NOTE_D  2
#define NOTE_DS 3
#define NOTE_E  4
#define NOTE_F  5
#define NOTE_FS 6
#define NOTE_G  7
#define NOTE_GS 8
#define NOTE_A  9
#define NOTE_AS 10
#define NOTE_B  11

/* B0  31  -> D#8 4978 */
/* C1 33 -> D#8 4987 */
const uint16_t freq[] = {
  /*31, */33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, 65, 69, 73, 78, 82, 
  87, 93, 98, 104, 110, 117, 123, 131, 139, 147, 156, 165, 175, 185, 196, 
  208, 220, 233, 247, 262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 
  494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 1047, 1109, 
  1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 2093, 2217, 2349, 
  2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 4186, 4435, 4699, 4978,
};
const uint8_t maxNotes = sizeof(freq) / sizeof(freq[0]);

#define MAKE_NOTE(note, octave) freq[(note + 12 * octave) % maxNotes]

#define DO_REST 1

/*const uint16_t tune[] = {
  MAKE_NOTE(NOTE_C, 5),
  MAKE_NOTE(NOTE_D, 5),
  MAKE_NOTE(NOTE_E, 5),
  MAKE_NOTE(NOTE_F, 5),
  MAKE_NOTE(NOTE_G, 5),
  MAKE_NOTE(NOTE_A, 5),
  MAKE_NOTE(NOTE_B, 5),
  MAKE_NOTE(NOTE_C, 6),
};*/

struct Note {
  uint16_t freq;
  uint16_t notediv;
};

const Note tankdeath[] = {
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
  { MAKE_NOTE(NOTE_FS, 1), 128 },
  { MAKE_NOTE(NOTE_CS, 1), 128 },
  { MAKE_NOTE(NOTE_FS, 1), 128 },
  { MAKE_NOTE(NOTE_CS, 1), 128 },
  { MAKE_NOTE(NOTE_FS, 1), 64 },
  { MAKE_NOTE(NOTE_CS, 1), 64 },
  { MAKE_NOTE(NOTE_FS, 1), 64 },
  { MAKE_NOTE(NOTE_CS, 1), 64 },
  { MAKE_NOTE(NOTE_FS, 1), 128 },
  { MAKE_NOTE(NOTE_CS, 1), 128 },
  { MAKE_NOTE(NOTE_FS, 1), 128 },
  { MAKE_NOTE(NOTE_CS, 1), 128 },
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
  { MAKE_NOTE(NOTE_FS, 1), 256 },
  { MAKE_NOTE(NOTE_CS, 1), 256 },
};

#define DOTTED_NOTE 0x8000
const Note tune[] = {
 
  { MAKE_NOTE(NOTE_FS, 1), 16 },
  { MAKE_NOTE(NOTE_A,  1), 8 },
  { MAKE_NOTE(NOTE_B,  1), 16 },
  { DO_REST, 8},
  
  { MAKE_NOTE(NOTE_FS,  1), 16 },
  { MAKE_NOTE(NOTE_B,  1), 16 },
  { DO_REST, 16},
  { MAKE_NOTE(NOTE_CS, 2), 8 },
  
  { MAKE_NOTE(NOTE_FS,  1), 16 },
  { MAKE_NOTE(NOTE_CS,  2), 8 },
  { MAKE_NOTE(NOTE_D,  2), 16 },
  
  { DO_REST, 16},
  { MAKE_NOTE(NOTE_FS, 1), 8 },
  { MAKE_NOTE(NOTE_CS, 2), 8 },
  
  { MAKE_NOTE(NOTE_B,  1), 16 },
  { MAKE_NOTE(NOTE_A,  1), 16 },
  { MAKE_NOTE(NOTE_FS,  1), 16 },

  { MAKE_NOTE(NOTE_E,  1), 8 | DOTTED_NOTE },//C

  /*{ MAKE_NOTE(NOTE_FS,  1), 8  }, //D

  { MAKE_NOTE(NOTE_D,  2), 16 },
  { MAKE_NOTE(NOTE_CS,  2), 16 },
  { MAKE_NOTE(NOTE_B,  1), 16 },

  { MAKE_NOTE(NOTE_FS, 1), 16 },
  { MAKE_NOTE(NOTE_A,  1), 8 },
  { MAKE_NOTE(NOTE_B,  1), 16 },
  { DO_REST, 8},

  { MAKE_NOTE(NOTE_FS,  1), 16 },
  { MAKE_NOTE(NOTE_B,  1), 16 },
  { DO_REST, 16},
  { MAKE_NOTE(NOTE_CS,  1), 16 },

  { MAKE_NOTE(NOTE_FS,  1), 16 },
  { MAKE_NOTE(NOTE_CS,  2), 8 },
  { MAKE_NOTE(NOTE_D,  2), 1 },*/

};
const uint8_t numNotesInTune = sizeof(tune) / sizeof(tune[0]);

const uint16_t ms_per_whole_note = 2666 /* 4:4 at 90bpm */;
void setup() {
  for (uint8_t thisNote = 0; thisNote < numNotesInTune; thisNote++)
  {
    Note n = tune[thisNote];
    uint16_t   ms = ms_per_whole_note / (n.notediv & 0x7FFF);
    ms += (n.notediv & DOTTED_NOTE) ? (ms>>1) : (0);

    if (n.freq != 0)
    {
      tone(8, n.freq, ms);
      delay(ms);
    }
    else
    {
      delay(ms);
    }
    
    noTone(8);
  }
}

void loop() {
  /*static currentNote = 0;
  tone(tune[currentNote]);
  currentNote = (currentNote + 1) % numNotesInTune;*/
}
