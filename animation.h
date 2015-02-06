#ifndef BAZINGA_ANIMATION_H
#define BAZINGA_ANIMATION_H

namespace bazinga {
	class Animation {
		Path img;
		int w, h;
		float duration;
		float nextFrame;
		int direction;
		int frame;
		bool loop;

		enum Type {Horizontal, Vertical};

		Type type;

		bool ended;
	public:
		Animation (Path);
	
		void render();
		int getWidth();
		int getHeight();

		bool isEnded();
	};
}

#endif /* BAZINGA_ANIMATION_H */