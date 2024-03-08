#include <fmod.h>
#include "menu.h"
#pragma once
FMOD_SYSTEM* fsystem;
FMOD_SOUND* sound[];
FMOD_RESULT result;
FMOD_CHANNEL* channel;
unsigned int version;

FMOD_SOUND* maintheme;
FMOD_SOUND* fanclub;
FMOD_SOUND* kamui;
FMOD_SOUND* hypeboy;
FMOD_SOUND* delarue;
FMOD_SOUND* tick;
FMOD_SOUND* start;
FMOD_SOUND* gameover;
FMOD_SOUND* clear;
FMOD_CHANNEL* channel; // 효과음은 채널 1에 넣자 일단
FMOD_CHANNEL* channel2; // 배경음은 채널 2에 넣자 일단

void AudioSystem();

void AudioCreate(const char*, FMOD_SOUND**);

void AudioPlay(FMOD_SOUND* sound, FMOD_CHANNEL** channel);

void AudioSong(FMOD_CHANNEL* channel);

unsigned GetAudioPlaybackTime(FMOD_CHANNEL* channel);
