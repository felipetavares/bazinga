#include "bazinga.h"

#include <iostream>
#include <vector>
using namespace std;

namespace bazinga {
  void startModules () {
    cout << "startModules()" << endl;

    char *data = fs::getFileData(Path("maps:test.json"));
    string sData = string (data);

    BjObject *object = json::parse (sData);

    Map map = Map(object);

    map.update();

    delete object;
    delete data;
  }

  void gameLoop () {
    cout << "gameLoop()" << endl;
  }

  void endModules () {
    cout << "endModules()" << endl;
  }
}

int main (int argc, char** argv) {
  bazinga::video::init();

  bazinga::startModules();
  bazinga::gameLoop();
  bazinga::endModules();

  bazinga::video::deinit();
  return 0;
}
