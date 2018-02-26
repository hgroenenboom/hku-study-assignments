#ifndef SYNTH_H
#define SYNTH_H

#include "oscillator.h"
#include "polymidi.h"

using namespace std;

class Synth{
public:
    Synth(WaveTable* wt);

    void process(float* buffer, int buffersize);
	// Process add is dangerous and should only be used if Synth is part of a polySynth
	void processAdd(float* buffer, int buffersize);
	
    void setSamplerate(int sr);
	void setAmplitude(float amp);
	void setWaveType(int type);
	void setMidiNote(float vel, float pitch);
	void setMidiPitch(float midiP);

    int oscType = 1;
	float midiNote = 32;

	WaveTable* wt;
	MVOscillator mvOscillator;
};

class PolySynth{
public:
	PolySynth(WaveTable* wt, int numvoices = 8)
		: wt(wt), 	
		  numVoices(numvoices),
		  polyMidi(numvoices)
	{
		
		synths = new Synth*[numvoices];
		for(int i = 0; i < numVoices; i++) {
			synths[i] = new Synth(wt);
			synths[i]->setAmplitude(0.0f);
		}
		
		normAmp = 1.0f / (float)numVoices;
	}
	
	void process(float* buffer, int buffersize) {
		for(int i = 0; i < buffersize; i++) {
			buffer[i] = 0.0f;
		}
		for(int i = 0; i < numVoices; i++) {
			synths[i]->processAdd(buffer, buffersize);
		}
		for(int i = 0; i < buffersize; i++) {
			buffer[i] *= normAmp;
		}
	}
	
	void newNote(int vel, int pitch) {
		int note = polyMidi.newNote(vel, pitch);
		synths[note]->setMidiNote(vel, pitch);
	}
	
	void setWaveType(int choice) {
		for(int i = 0; i < numVoices; i++) {
			synths[i]->setWaveType(choice);
		}
	}
	
	int numVoices = 8;
	float normAmp = 1.0f;
	Synth** synths;
	WaveTable* wt;
	
	PolyMidi polyMidi;
};

#endif