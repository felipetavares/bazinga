#include "console.h"
// To load fonts
#include "cache.h"
// To paint the screen
#include "video.h"
#include <iomanip>
using namespace bazinga;

bazinga::Console bazinga::console = bazinga::Console();

Console::Message::Message (string text) {
    this->text = text;
    time = bazinga::curtime;
    number = 1;
}

string Console::Message::getString () {
    stringstream msgStream;

    msgStream << "@";
    msgStream.precision(4);
    msgStream.setf(ios::left);
    msgStream.width(7);
    msgStream.fill(' ');
    msgStream << time;
    msgStream << " " << text;

    if (number > 1) {
        msgStream << " x" << number;
    }

    return msgStream.str();
}

string Console::Message::getText () {
    return text;
}

void Console::Message::increaseNumber () {
    time = bazinga::curtime;
    number++;
}

Console::Console () {
    offset = 0;
    lineHeight = 16;
    manualScrolling = false;
    line = "";
}

void Console::init () {
}

void Console::log (string message) {
    if (messages.size() > 0 &&
        message == messages[messages.size()-1].getText()) {
        messages[messages.size()-1].increaseNumber();
    } else {
        messages.push_back(Message(message));

        int height = video::windowHeight/lineHeight;

        if (video::windowHeight > 0 && messages.size()>=height && !manualScrolling) {
            offset--;
        }
    }
}

void Console::render () {
    video::setColor1(video::Color(0.8 , 1, 0.5, 1));
    video::fillRect(-video::windowWidth/2, -video::windowHeight/2,
                    video::windowWidth, video::windowHeight);

    auto font = cache::getFont("default");
    font->setSize(16);
    font->setColor(video::Color(0, 0, 0, 0));
    text::setAlign(text::Left);
    text::setBaseline(text::Top);
    text::setFont(font);

    int height = video::windowHeight/lineHeight;
    int start = -offset>=0?-offset:0;
    int end = -offset+height>=0?-offset+height:0;
    int x = -video::windowWidth/2;
    for (int y=start;y<end&&y<messages.size();y++) {
        text::fillText(messages[y].getString(), x, (y+offset)*lineHeight-video::windowHeight/2);
    }
}

void Console::up () {
    manualScrolling = true;
    offset++;
}

void Console::down () {
    offset--;
    int height = video::windowHeight/lineHeight;
    if (messages.size()+offset <= height)
        manualScrolling = false;
}

void Console::deinit() {
}

string Console::toStr (float f) {
    stringstream ss;
    ss << f;
    return ss.str();
}

Console& Console::operator<< (const string& msg) {
    if (msg == outline) {
        log(line);
        line = string();
    } else {
        line += msg;
    }
    return *this;
}

Console& Console::operator<< (const float& msg) {
    line += toStr(msg);
    return *this;
}
