#ifndef BAZINGA_H
#define BAZINGA_H

#include "filesystem.h"
#include "json.h"
#include "object.h"
#include "map.h"
#include "video.h"
#include "input.h"
#include "audio.h"

namespace bazinga {
	extern double delta;
	extern double curtime;

	bool init ();
	void gameLoop();
	void deinit ();

	Map* getActiveMap();
	cpBool pBeginCollision (cpArbiter*, cpSpace*, void*);

	void toggleConsole();
	void setScene(Path);

	void quit();
}

#endif /* BAZINGA_H */