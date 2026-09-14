#!/usr/bin/env python3
"""Rebuild MuseFlow's original, deterministic 8-second PCM demonstration asset."""
from pathlib import Path
import math
import struct
import wave

destination = Path(__file__).resolve().parents[1] / 'assets' / 'mock-demo.wav'
sample_rate = 22050
chords = [(146.832, 174.614, 220.000), (130.813, 164.814, 195.998),
          (116.541, 146.832, 174.614), (130.813, 164.814, 220.000)]
frames = bytearray()
for index in range(sample_rate * 8):
    t = index / sample_rate
    chord_index = min(3, int(t / 2))
    local = t % 2
    chord = chords[chord_index]
    envelope = min(1, local / .1) * min(1, (2 - local) / .25)
    pad = sum(math.sin(2 * math.pi * frequency * t) for frequency in chord) / 3
    note_t = t % .5
    frequency = chord[int(local / .5) % 3] * 2
    melody = math.sin(2 * math.pi * frequency * note_t) * math.exp(-note_t * 6) * min(1, note_t / .008)
    fade = min(1, t / .2) * min(1, (8 - t) / .8)
    value = (.23 * pad * envelope + .20 * melody) * fade
    left = int(value * 32767)
    right = int(value * (.96 + .04 * math.sin(t * .7)) * 32767)
    frames.extend(struct.pack('<hh', left, right))
with wave.open(str(destination), 'wb') as audio:
    audio.setnchannels(2)
    audio.setsampwidth(2)
    audio.setframerate(sample_rate)
    audio.writeframes(frames)
print(f'Created {destination.name}: 8.0 s, stereo PCM WAV')
