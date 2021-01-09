#pragma once

#include <irrKlang.h>

irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();
irrklang::ISound* walk_sound = engine->play2D("asset/Audio/FootstepsWalk.wav", true, true);
irrklang::ISound* run_sound = engine->play2D("asset/Audio/FootstepsRun.wav", true, true);
