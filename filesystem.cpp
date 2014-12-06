#include "filesystem.h"

using namespace bazinga;

#ifdef WIN32
#else
#define _stat stat
#endif

Path::Path (string _path) {
	oPath = string(_path);
	setPath (_path);
}

Path::~Path () {

}

void Path::setPath (string _path) {
#ifdef WIN32
	setWindowsPath (_path);
#else
	setLinuxPath (_path);
#endif
}

bool Path::isEqual (Path _path) {
	if (_path.getOriginalPath() == getOriginalPath())
		return true;
	return false;
}

string Path::getOriginalPath () {
	return oPath;
}

void Path::setWindowsPath (string _path) {
	for_each (_path.begin(),_path.end(),[=] (char &c)  -> void {
		if (c == ':' || c == '\\' || c == '/')
			c = '\\';
	});

	path = _path;
}

void Path::setLinuxPath (string _path) {
	for_each (_path.begin(),_path.end(),[=] (char &c)  -> void {
		if (c == ':' || c == '\\' || c == '/')
			c = '/';
	});

	path = _path;
}

string Path::getPath () {
	return path;
}

string Path::getExtension () {
	string extension;
	int i;
	for (i =path.length()-1;path[i] != '.' && i >= 0;i--) {
		extension += path[i];
	}

	if (i==0)
		return ""; // No extension found
	else {
		reverse(extension.begin(),extension.end());
		return extension;
	}
}

void Path::normalize() {
	for (int i=0;i<oPath.size()-1;i++) {
		if (oPath[i] == '.' && oPath[i+1] == ':') {
                        oPath = oPath.substr(0,i)+oPath.substr(i+2, oPath.size()-i-2);
                        i-=2;
		}
	}
	setPath(oPath);
}

fs::fs () {

}

fs::~fs () {

}

bool fs::fileExists (Path _path) {
	struct _stat info;
	return !(bool)_stat (_path.getPath().c_str(),&info);
}

bool fs::deleteFile (Path _path) {
	if (fileExists (_path)) {
		unlink (_path.getPath().c_str());
	}
	return false;
}

bool fs::createFile (Path _path) {
	if (fileExists(_path))
		return false;

#ifdef WIN32
	int fd = open(_path.getPath().c_str(), O_CREAT, S_IRUSR | S_IWUSR);
#else
	int fd = open(_path.getPath().c_str(), O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
#endif

	if (fd < 0)
		return false;

	close (fd);

	return true;
}

bool fs::renameFile (Path _pathA,Path _pathB) {
	if (fileExists (_pathA) && !fileExists(_pathB)) {
		if (rename (_pathA.getPath().c_str(),_pathB.getPath().c_str()) < 0)
			return false;
		return true;
	}
	return false;
}

bool fs::copyFile (Path _pathA,Path _pathB) {
	if (fileExists (_pathA) && !fileExists(_pathB)) {
		if (!createFile (_pathB))
			return false;

		int fdA,fdB;
		// Open file A for reading only B for writing only
		if ((fdA = open(_pathA.getPath().c_str(), O_RDONLY)) < 0)
			return false;
		if ((fdB = open (_pathB.getPath().c_str(), O_WRONLY)) < 0)
			return false;

		function <void (int,int,char*)> cleanup = [] (int fdA,int fdB, char *fileBuffer) -> void {
			delete fileBuffer;
			close (fdA);
			close (fdB);
		};

		char *fileBuffer = new char [bufferSize];
		int nr,nw;

		do {
			nr = read (fdA,fileBuffer,bufferSize);
			nw = write (fdB,fileBuffer,nr);

			if (nr != nw) { // Error writing!
				cleanup (fdA,fdB,fileBuffer);
				return false;
			}
		} while (nr > 0); // While we have bytes to copy

		cleanup (fdA,fdB,fileBuffer);
		return true;
	}
	return false;
}

char* fs::getFileData (Path _path) {
	if (fileExists(_path)) {
		size_t fileSize = getFileSize (_path);
		int bufferSize = 128;
		char* data = new char [fileSize+1];
        char* buffer = new char [bufferSize];
		int fd;
		int nr = -1;
        size_t rs = 0;

        fd = open (_path.getPath().c_str(), O_RDONLY);

        while (nr != 0) {
            nr = read (fd,buffer,bufferSize);
            if (nr < 0) {
                delete data;
                return NULL;
            }
            memcpy (data+rs, buffer, nr);
            rs += nr;
        }

		close (fd);

        delete buffer;
		return data;
	}

	return NULL;
}

bool fs::setFileData (Path _path, const char* _data, size_t _size) {
	if (!fileExists (_path))
		if (!createFile(_path))
			return false;

	int fd,nr;

	fd = open (_path.getPath().c_str(), O_WRONLY |  O_TRUNC);
	nr = write (fd, _data, _size);
	close (fd);

	if (nr < _size)
		return false;

	return true;
}

size_t fs::getFileSize (Path _path) {
	if (fileExists(_path)) {
		struct _stat info;
		if (_stat (_path.getPath().c_str(),&info) < 0) {
			return -1;
		}

		return info.st_size;
	}

	return -1;
}

vector <Path> fs::listDirectory (Path _path,bool &_success) {
	vector <Path> dir;
	_success = true;

	if (isDir(_path)) {
                DIR *d;
                struct dirent *entry;

                d = opendir(_path.getPath().c_str());

                if (d) {
                	string lig = _path.getOriginalPath()[_path.getOriginalPath().size()-1] != ':'?":":"";
			while ((entry = readdir(d)) != NULL) {
                                dir.push_back (Path(_path.getOriginalPath()+lig+string(entry->d_name)));
			}

			closedir(d);
                } else {
			_success = false;
                }
	} else {
		_success = false;
	}

	return dir;
}

bool fs::isDir(Path _path) {
	return true;
	struct _stat info;
	_stat (_path.getPath().c_str(),&info);
	return S_ISDIR(info.st_mode);
}