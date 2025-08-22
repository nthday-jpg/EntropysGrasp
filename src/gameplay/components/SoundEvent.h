#pragma once
#include <string>

struct PlaySoundEvent 
{
    std::string name;
    bool loop = false;
};

struct StopSoundEvent 
{
    std::string name;
};

struct StopAllSoundsEvent {};

struct PlayMusicEvent 
{
    std::string name;
    bool loop = true;
};

struct StopMusicEvent {};
