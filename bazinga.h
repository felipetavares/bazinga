#ifndef BAZINGA_H
#define BAZINGA_H

#include "filesystem.h"
#include "json.h"
#include "object.h"
#include "map.h"
#include "video.h"
#include "input.h"

namespace bazinga {
	extern double delta;
	void startModules ();
	void gameLoop();
	void endModules ();

	Map* getActiveMap();
}

#endif /* BAZINGA_H */
