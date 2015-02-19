#ifndef BAZINGA_CONSOLE_H
#define BAZINGA_CONSOLE_H

#include "bazinga.h"
#include "text.h"
#include <string>
#include <sstream>
using namespace std;

#define LINEINFO Path(__FILE__).getName()<<": "<<__func__<<": "<<__LINE__<<": "

namespace bazinga {
    // Debugging console.
    // implemented because GUI was giving me a lot of headcache
    // So I needed some way of looking at variables.
    const static string outline = "\n";

    class Console {
    public:
        class Message {
        	float time;
        	string text;
        	int number;
		public:
			Message(string);
        	string getString();
        	string getText();
        	void increaseNumber();
        };	
	private:
		bool manualScrolling;
        int offset;
        int lineHeight;
        vector <Message> messages;
        string line;
    public:
    	Console();

        void init();
        void log(string);
        string toStr(float);
        Console& operator<< (const string&);
        Console& operator<< (const float&);
        void up();
        void down();
        void render();
        void deinit();
    };

    extern Console console;
}

#endif /* BAZINGA_CONSOLE_H */
