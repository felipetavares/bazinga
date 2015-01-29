#ifndef BAZINGA_FILESYSTEM_H
#define BAZINGA_FILESYSTEM_H

// Macros to detect in what plataform I'm building
#ifdef MINGW32
#define WIN32
#endif
#ifdef __WIN32__
#define WIN32
#endif
#ifdef _WIN32
#define WIN32
#endif

//#define WIN32

#include <iostream>
#include <fcntl.h>			// Low level file creation
#include <sys/types.h>		// For Linux/Windows
#include <sys/stat.h>		// For Linux/Windows
#include <cstdio>			// Files
#include <unistd.h>			// Deleting
#include <cstring>			// Memcpy
#include <dirent.h>			// Directory listing
#include <algorithm>
#include <vector>
using namespace std;

namespace bazinga {
	class Path {
		string path; // plataform-specific string
		string oPath; // non plataform-specific string
	public:
		Path (string="");
		~Path ();

		void setPath(string);   // set path in non-plataform-specific way (using '/' )
		string getPath(); // get plataform specific path
		string getExtension();
		string getName();
		string getOriginalPath();
		bool isEqual(Path);
		void normalize();
	private:
		void setWindowsPath(string);
		void setLinuxPath(string);
		//setMacOsPath();
		//setAndroidPath();
		//setiOSPath();
	};

	class fs {
		static const int bufferSize = 4096; // 4 K of memory
	public:
		fs ();
		~fs	 ();

		static bool fileExists (Path);
		static bool copyFile (Path,Path,bool);
		static bool renameFile (Path,Path);
		static bool deleteFile (Path);
		static bool createFile (Path);
		static bool isDir (Path);
		static size_t getFileSize (Path);
		static char* getFileData (Path);
		static bool setFileData(Path, const char*, size_t);
		static vector <Path> listDirectory (Path,bool&);
	};
}
#endif /* BAZINGA_FILESYSTEM_H */
