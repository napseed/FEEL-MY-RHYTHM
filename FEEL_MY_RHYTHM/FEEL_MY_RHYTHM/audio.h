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
FMOD_CHANNEL* channel; // ȿ������ ä�� 1�� ���� �ϴ�
FMOD_CHANNEL* channel2; // ������� ä�� 2�� ���� �ϴ�

void AudioSystem();

void AudioCreate(const char*, FMOD_SOUND**);

void AudioPlay(FMOD_SOUND* sound, FMOD_CHANNEL** channel);

void AudioSong(FMOD_CHANNEL* channel);

unsigned GetAudioPlaybackTime(FMOD_CHANNEL* channel);
