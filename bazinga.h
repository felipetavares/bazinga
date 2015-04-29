#ifndef BAZINGA_H
#define BAZINGA_H

#include "filesystem.h"
#include "json.h"
#include "object.h"
#include "video.h"
#include "input.h"
#include "audio.h"

namespace bazinga {
	class Map;

	extern double delta;
	extern double curtime;
	extern bool scriptsEnabled;
	extern string projectPath;
	extern Map* activeMap;
	extern bool exitFlag;

	bool init ();
	void gameLoop();
	void deinit ();

	Map* getActiveMap();
	cpBool pBeginCollision (cpArbiter*, cpSpace*, void*);

	void toggleConsole();
	void setScene(Path);

	// Exposed API
	void quit();
	void reloadProject();
	void openProject();
	void saveScene();
	void toggleFPSWindow();
}

#endif /* BAZINGA_H */
