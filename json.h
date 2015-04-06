#ifndef BAZINGA_JSON_H
#define BAZINGA_JSON_H

#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

class BjArray;
class BjObject;

class BjValue {
public:
	enum Type {jString,jNumber,jObject,jArray,jTrue,jFalse,jNull};
	Type type;

	string 		str;
	float 		number;
	BjArray 	*array;
	BjObject 	*object;

	BjValue ();
	BjValue (const BjValue&);
	BjValue (string&);
	BjValue (float);
	BjValue (bool);
	BjValue (BjArray*);
	BjValue (BjObject*);
	~BjValue ();
};

class BjArray {
public:
	vector <BjValue*> array;

	BjArray ();
	BjArray (const BjArray&);
	~BjArray ();
};

class BjObject {
public:
	vector <string> keys;
	vector <BjValue*> values;

	BjObject ();
	BjObject (const BjObject&);
	~BjObject ();

	BjValue* get(string);
};

namespace bazinga {
	class json {
	public:
		json ();
		~json ();

		static BjObject* parse (string&);
		static BjObject* parseObject (long int&,string&);
		static BjValue* parseString (long int&,string&);
		static BjValue* parseValue (long int&,string&);
		static BjValue* parseNumber (long int&,string&);
		static BjArray* parseArray (long int&,string&);

		static string dumpjson (BjObject*);
		static string dumpObject (BjObject*);
		static string dumpArray (BjArray*);
		static string dumpValue (BjValue*);
	private:
		static bool isChar (char,long int&,string&);
		static bool isCharLookAhead (char,long int&,string&,int);
		static bool isCharsLookAhead (string,long int&,string&,int);
		static bool isCharLookAheadNoWhite (char,long int&,string&,int);
		static bool isCharsLookAheadNoWhite (string,long int&,string&,int);
		static void advance (long int&,string&);
		static char getNextChar (long int&, string&);
		static char getNextCharNoWhite (long int&, string&);
	};
}

#endif /* BAZINGA_JSON_H */
