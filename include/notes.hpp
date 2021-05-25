#ifndef NOTES_H
#define NOTES_H

typedef struct {
  float pitch = 0;
  float velocity = 0;
  char channel = 0;

  float step = 0;
  float phase = 0;
}Note;

#endif