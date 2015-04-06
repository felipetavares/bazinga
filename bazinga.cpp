#include "gl/glwin.h"
#include "bazinga.h"
#include "filesystem.h"
#include "text.h"
#include "cache.h"
#include "editor.h"
#include "console.h"
#include "render.h"
#include "gui.h"
#include "map.h"
#include "gui/label.h"
#include "gui/button.h"
#include "gui/spacer.h"
#include "gui/slider.h"
#include "gui/progress.h"
#include "gui/entry.h"
#include "gui/bgraph.h"
#include "gui/scroll.h"
#include "gui/textarea.h"
#include "gui/checkbox.h"
#include "gui/filemanager.h"
#include <iostream>
#include <vector>
#include <chrono>
using namespace std;

namespace bazinga {
  double delta = 0;
  double curtime = 0;
  bool consoleFlag = false;
  bool setNewScene = false;
  bool blockNewScene = false;
  bool exitFlag = false;
  bool scriptsEnabled = true;
  Path newScenePath;
  string mainScenePath = "scenes/main.scene";

  Map* activeMap = NULL;

  Map* getActiveMap () {
    return activeMap;
  }

  //void postStepAct(cpSpace *space, cpArbiter *arb, void *data) {
  //  cpShape *a, *b;
  //  cpArbiterGetShapes(arb, &a, &b);
  //}

  cpBool pBeginCollision (cpArbiter *arb, cpSpace *space, void *data) {
    return getActiveMap()->pmBeginCollision(arb, space, data);
  }

  void toggleConsole() {
    consoleFlag = !consoleFlag;
  }

  bool events () {
    SDL_Event event;

    if (exitFlag)
      return false;

   while ( SDL_PollEvent(&event) ) {
      switch (event.type) {
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
          //inputManager->joystickButtonEvent (event);
        break;
        case SDL_JOYAXISMOTION:
          //inputManager->joystickAxisEvent (event);
        break;
        case SDL_KEYDOWN:
        if (event.key.keysym.sym != SDLK_ESCAPE) {
          string keyName = string(SDL_GetKeyName(event.key.keysym.sym));
          transform (keyName.begin(), keyName.end(), keyName.begin(), ::tolower);
          input::keypress(keyName, event.key.keysym.unicode);
        } else {
            auto window = new gui::Window("Opções", 500, 400);
            auto container = new gui::Container(gui::Container::VERTICAL);
            auto line = new gui::Container(gui::Container::HORIZONTAL);
            auto line2 = new gui::Container(gui::Container::HORIZONTAL);
            auto line3 = new gui::Container(gui::Container::HORIZONTAL);
            auto line4 = new gui::Container(gui::Container::HORIZONTAL);
            auto line5 = new gui::Container(gui::Container::HORIZONTAL);
            auto line6 = new gui::Container(gui::Container::HORIZONTAL);
            auto line7 = new gui::Container(gui::Container::HORIZONTAL);

            line->borderLeft = line->borderRight = 0;
            line->borderTop = line->borderBottom = 0;

            line2->borderLeft = line2->borderRight = 0;
            line2->borderTop = line2->borderBottom = 0;

            line3->borderLeft = line3->borderRight = 0;
            line3->borderTop = line3->borderBottom = 0;

            line4->borderLeft = line4->borderRight = 0;
            line4->borderTop = line4->borderBottom = 0;

            line5->borderLeft = line5->borderRight = 0;
            line5->borderTop = line5->borderBottom = 0;

            line6->borderLeft = line6->borderRight = 0;
            line6->borderTop = line6->borderBottom = 0;

            line7->borderLeft = line7->borderRight = 0;
            line7->borderTop = line7->borderBottom = 0;

            line->add(new gui::Label("Sair da demo:"));
            auto button = new gui::Button("Sair");
            line->add(button);

            line2->add(new gui::Label("Frames por segundo:"));
            auto fps = new gui::Label("####");
            line2->add(fps);

            auto bgraph = new gui::BGraph();
            line3->add(bgraph);

            auto enableGrid = new gui::CheckBox("Grade");
            auto enableScripts = new gui::CheckBox("Scripts");
            enableScripts->setChecked(scriptsEnabled);
            auto gridX = new gui::Entry("80");
            auto gridY = new gui::Entry("80");
            line4->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
            line4->add(enableScripts);
            line4->add(enableGrid);
            line4->add(new gui::Label("X:"));
            line4->add(gridX);
            line4->add(new gui::Label("Y:"));
            line4->add(gridY);
            line4->add(new gui::Spacer(gui::Spacer::HORIZONTAL));

            //auto savePath = new gui::Entry();
            //auto saveName = new gui::Entry();
            auto saveAs = new gui::Button("Salvar");
            //auto findFile = new gui::Button(Path("assets/gui/document.png"));
            line5->add(new gui::Spacer(gui::Spacer::HORIZONTAL));
            //line5->add(findFile);
            //line5->add(savePath);
            //line5->add(saveName);
            line5->add(saveAs);
            line5->add(new gui::Spacer(gui::Spacer::HORIZONTAL));

            auto zoomSlider = new gui::Slider(0);
            line6->add(zoomSlider);

            auto newObject = new gui::Button("Novo objeto");
            auto copyObject = new gui::Button("Copiar Objeto");
            auto deleteObject = new gui::Button("Deletar Objeto");
            auto newScene = new gui::Button("Nova cena");
            line7->add(newObject);
            line7->add(copyObject);
            line7->add(deleteObject);
            line7->add(newScene);

            container->add(line);
            container->add(line2);
            container->add(line3);
            container->add(line4);
            container->add(line5);
            container->add(line6);
            container->add(line7);

            window->setRoot(container);
            gui::add(window, -video::windowWidth/2, video::windowHeight/2-400);

            window->onUpdate = [=] (gui::Window* win) {
              stringstream ss;
              ss << floor(1/delta);
              fps->setText(ss.str());
              bgraph->addBar(1/delta);
            };

            zoomSlider->onChange = [] (gui::Widget *wid) {
              auto slider = (gui::Slider*)wid;

              getActiveMap()->zoomX = 10*slider->getPosition();
              getActiveMap()->zoomY = 10*slider->getPosition();
            };
 
            enableScripts->onClick = [] (gui::Widget *wid) {
              auto checkbox = (gui::CheckBox*)wid;

              bazinga::scriptsEnabled = checkbox->isChecked();
            };

            newObject->onClick = [] (gui::Widget *wid) {
              getActiveMap()->newObject(Path("objects/table.obj"));
            };

            copyObject->onClick = [] (gui::Widget *wid) {
              getActiveMap()->edit->copyObject();
            };

            deleteObject->onClick = [] (gui::Widget *wid) {
              getActiveMap()->edit->deleteObject();
            };

            newScene->onClick = [] (gui::Widget *wid) {
              editor::openNewSceneWindow();
            };

            saveAs->onClick = [] (gui::Widget *wid) {
              getActiveMap()->save();
            };

            //findFile->onClick = [savePath] (gui::Widget* wid) {
            //  new gui::FileManager(savePath);
            //};

            button->onClick = [=] (gui::Widget* wid) {
              bazinga::quit();
            };

            enableGrid->onClick = [=] (gui::Widget* wid) {
              getActiveMap()->edit->setGrid(((gui::CheckBox*)wid)->isChecked());
            };

            gridX->onChange = [=] (gui::Widget* wid) {
              gui::Entry *entry = (gui::Entry*)wid;
              try {
                float x = stof(entry->getText());
                vec2 grid = getActiveMap()->edit->getGridSize();
                grid.x = x;
                getActiveMap()->edit->setGridSize(grid);
              } catch (exception e) {

              }
            };

            gridY->onChange = [=] (gui::Widget* wid) {
              gui::Entry *entry = (gui::Entry*)wid;
              try {
                float y = stof(entry->getText());
                vec2 grid = getActiveMap()->edit->getGridSize();
                grid.y = y;
                getActiveMap()->edit->setGridSize(grid);
              } catch (exception e) {
                
              }
            };
        }
        break;
        case SDL_KEYUP:
        if (event.key.keysym.sym != SDLK_ESCAPE) {
          string keyName = string(SDL_GetKeyName(event.key.keysym.sym));
          transform (keyName.begin(), keyName.end(), keyName.begin(), ::tolower);
          input::keyunpress(keyName);
        } else {
          //return false;
        }
        break;
        case SDL_MOUSEMOTION:
          input::mousemove (event.motion.x-video::windowWidth/2,
                            event.motion.y-video::windowHeight/2);
        break;
        case SDL_MOUSEBUTTONDOWN:
          input::mousepress (event.button.button,
                             event.button.x-video::windowWidth/2,
                             event.button.y-video::windowHeight/2);
        break;
        case SDL_MOUSEBUTTONUP:
          input::mouseunpress (event.button.button,
                               event.button.x-video::windowWidth/2,
                               event.button.y-video::windowHeight/2);
        break;
        case SDL_QUIT:
          return false;
      }
    }

    return true;
  }

  void setScene (Path path) {
    if (blockNewScene)
      return;

    bazinga::newScenePath = path;
    bazinga::setNewScene = true;
  }

  bool init () {
    bazinga::video::init();
    bazinga::render::init();
    bazinga::text::init();
    bazinga::input::init();
    //bazinga::audio::init();
    bazinga::gui::init();
    bazinga::console.init();

    // On Linux: nothing
    // On Windows: setup OpenGL above 1.1
    initGL();

    new bazinga::input::Context("main");
    bazinga::input::activateContext("main");

    bazinga::video::setWindowTitleAndIcon("Bazinga! Engine", "Bazinga! Engine");

    auto defaultFont = Path("assets/fonts/texgyrecursor-regular.otf");
    cache::createFont(defaultFont, "default");

    console << LINEINFO << "using '" << defaultFont.getPath() << "' as default font" << outline;

    auto v = bazinga::cache::getVertexShader(bazinga::Path("shaders/text-vert.glsl"));
    auto f = bazinga::cache::getFragmentShader(bazinga::Path("shaders/text-frag.glsl"));
    bazinga::cache::createShaderProgram (v, f, "text");
    bazinga::cache::getShaderProgram("text")->loadUniforms({"sampler","color"});

    setScene(Path(mainScenePath));

    return true;
  }

  void gameLoop () {
    //console << "gameLoop()" << outline;

    chrono::high_resolution_clock::time_point t;
    chrono::high_resolution_clock::time_point startTime;

    startTime = chrono::high_resolution_clock::now();

    while (events()) {
      auto t0 = chrono::high_resolution_clock::now();
      auto elapsed = t0-t;
      t = t0;

      curtime = (chrono::duration_cast<chrono::microseconds>((t0-startTime))).count()/1000000.0;
      delta = ((chrono::duration_cast<chrono::microseconds>(elapsed)).count()/1000000.0);

      //if (delta > 1.0/30.0) {
      //  delta = 1.0/30.0;
      //}

      if (activeMap) {
        activeMap->update();

        activeMap->render();

        gui::render();
      }

      if (consoleFlag) {
        console.render();
      }

      video::render();

      if (bazinga::setNewScene) {
        bazinga::setNewScene = false;

        blockNewScene = true;

        video::onFinish = [=] () {
          if (activeMap) {
            delete activeMap;
          }

          activeMap = new Map(newScenePath);
          activeMap->init();

          video::fadeFrom(video::Color(0, 0, 0), 0.5);

          blockNewScene = false;
        };

        video::fadeTo(video::Color(0, 0, 0), 0.5);
      }
    }
  }

  void deinit () {
    if (activeMap) {
      delete activeMap;
    }

    bazinga::gui::deinit();
    //bazinga::audio::deinit();
    bazinga::input::deinit();
    bazinga::cache::deinit();
    bazinga::text::deinit();
    bazinga::render::deinit();
    bazinga::video::deinit();
    bazinga::console.deinit();
  }

  void quit () {
    exitFlag = true;
  }
}

void copyMaps () {
#ifdef _WIN32
  // List map dir
  bool ok;
  auto path = bazinga::Path(string(getenv("APPDATA")+string("/LOVE/data/maps/")));
  auto dir = bazinga::fs::listDirectory(path, ok);

  if (ok) {
    console << LINEINFO << "copying the following scenes to scenes/" << outline;

    for (auto e :dir) {
      if (!bazinga::fs::isDir(e)) {
        console << "\t - " << e.getPath() << " - ";
        if (bazinga::fs::copyFile(e, bazinga::Path("scenes/"+e.getName()), true)) {
          console << "ok" << outline;
        } else {
          console << "fail" << outline;
        }
      }
    }
  } else {
    console << LINEINFO << "" << path.getPath() << outline;
  }
#endif
}

#ifdef _WIN32
int WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
 #else
int main (int argc, char** argv) {
#endif /* _WIN32 */
  console << "Bazinga! Engine compiled in " << __DATE__ << " " << __TIME__ << outline;
  console << outline;

  copyMaps();

  try {
    if (bazinga::init())
      bazinga::gameLoop();
    bazinga::deinit();
  } catch (exception e) {
    console << LINEINFO << e.what() << outline;
  }

  return 0;
}
