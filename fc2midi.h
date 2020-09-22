

typedef struct
{
  int8_t pitch_transpose;
  int8_t sound_transpose;
  int8_t prev_fc_note;
  int8_t prev_midi_note;
} fc_chan_t;

void fc_chan_clear( fc_chan_t *fc_chan);
void fc_chan_config( fc_chan_t *fc_chan, int8_t pitch_transpose, int8_t sound_tranpose );
void fc_chan_row_process( fc_chan_t *fc_chan, int8_t note, int8_t instr_num );


