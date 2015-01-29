#include "json.h"
using namespace bazinga;

BjValue::BjValue (const BjValue& _v) {
	type = _v.type;
	str = _v.str;
	number = _v.number;
	object = _v.object;
	array = _v.array;
}

BjValue::BjValue () {
	type = BjValue::jNull;
	array = NULL;
	object = NULL;
}

BjValue::BjValue (string& _str) : str() {
	type = BjValue::jString;
	str = _str;

	array = NULL;
	object = NULL;
}

BjValue::BjValue (float _number) {
	type = BjValue::jNumber;
	number = _number;

	array = NULL;
	object = NULL;
}

BjValue::BjValue (bool _bool) {
	if (_bool)
		type = BjValue::jTrue;
	else
		type = BjValue::jFalse;

	array = NULL;
	object = NULL;
}

BjValue::BjValue (BjArray *_array) {
	type = BjValue::jArray;
	array = _array;

	object = NULL;
}

BjValue::BjValue (BjObject *_object) {
	type = Type::jObject;
	object = _object;

	array = NULL;
}

BjValue::~BjValue () {
	if (type == BjValue::jObject && object != NULL)
		delete object;
	else if (type == BjValue::jArray && array != NULL)
		delete array;
}

BjObject::BjObject () {
}

BjObject::BjObject (const BjObject& _o): keys(_o.keys), values (_o.values) {
}

BjObject::~BjObject () {
}

BjValue* BjObject::get (string _key) {
	for (int i=0;i<keys.size();i++) {
		if (_key == keys[i]) {
			return values[i];
		}
	}

	return new BjValue ();
}

BjArray::BjArray () {
}

BjArray::BjArray (const BjArray& _a): array(_a.array) {
}

BjArray::~BjArray () {
}

json::json () {

}

json::~json () {

}

string json::dumpjson (BjObject* _json) {
	string str;
	str += dumpObject (_json);
	return str;
}

string json::dumpObject (BjObject* _obj) {
	string str;
	str += "{\n";

	int i=0;
	for (auto& e :_obj->keys) {
		str += "\"" + e + "\"";
		str += ": ";
		str += dumpValue (_obj->values[i]);
		if (i<_obj->keys.size()-1)
			str += ",";
		str += "\n";
		i++;
	}

	str += "}";
	return str;
}

string json::dumpArray (BjArray* _arr) {
	string str;
	str += "[\n";

	int i=0;
	for (auto& e :_arr->array) {
		str += "\"" +dumpValue (e) + "\"";
		if (i<_arr->array.size()-1)
			str += ",";
		str += ",";
		str += "\n";
		i++;
	}

	str += "]";

	return str;
}

string json::dumpValue (BjValue* _val) {
	string str;
	stringstream ss;

	switch (_val->type) {
		case BjValue::jString:
			str += "\"" + _val->str +  "\"";
		break;
		case BjValue::jNumber:
			ss << _val->number;
			str += ss.str();
		break;
		case BjValue::jObject:
			str += dumpObject(_val->object);
		break;
		case BjValue::jArray:
			str += dumpArray(_val->array);
		break;
		case BjValue::jTrue:
			str += "true";
		break;
		case BjValue::jFalse:
			str += "false";
		break;
		case BjValue::jNull:
			str += "null";
		break;
	}

	return str;
}

BjObject* json::parse (string& _str) {
	long int pos = 0; // Position in the string

	return parseObject(pos,_str);
}

BjObject* json::parseObject (long int& _pos, string& _str) {
	BjObject* object = new BjObject();

	/* A json object starts with a { */
	if (isChar ('{',_pos,_str)) {
		// Following we have a string
		while (isCharLookAhead('"', _pos, _str, 1)) {
			BjValue *keyName = parseString(_pos,_str);
			if (isChar (':',_pos,_str)) {
				BjValue *value = parseValue (_pos,_str);

				object->keys.push_back (keyName->str);
				object->values.push_back (value);

				if (isCharLookAhead(',',_pos,_str, 1))
					isChar(',',_pos,_str);
			} else {
			}
		}

		if (isChar('}',_pos,_str)) {
			return object;
		} else {
		}
	} else {
	}
}

BjValue* json::parseString (long int& _pos, string& _str) {
	string jsonString = "";

	if (isChar ('"', _pos, _str)) {
		while (_str[_pos] != '"') {
			jsonString += _str[_pos++];
		}

		_pos++;

		BjValue *v = new BjValue(jsonString);

		return v;
	} else {
		// Error
	}
}

BjValue* json::parseNumber (long int& _pos, string& _str) {
	float jsonNumber = 0;
	string jsonString = "";

	advance(_pos,_str); // Consume all white spaces

	if (isCharLookAheadNoWhite ('-', _pos, _str, 1)) {
		jsonString += _str[_pos++];
	}

	if (isCharLookAheadNoWhite ('0', _pos, _str, 1)) {
		jsonString += _str[_pos++];
	} else if (isCharsLookAheadNoWhite ("123456789", _pos, _str, 1)) {
		jsonString += _str[_pos++];
		while (isCharsLookAheadNoWhite ("0123456789", _pos, _str, 1))
			jsonString += _str[_pos++];
	} else {

	}

	if (isCharLookAheadNoWhite ('.', _pos, _str, 1)) {
		jsonString += _str[_pos++];

		if (isCharsLookAheadNoWhite ("0123456789", _pos, _str, 1)) {
			jsonString += _str[_pos++];
			while (isCharsLookAheadNoWhite ("0123456789", _pos, _str, 1))
				jsonString += _str[_pos++];
		} else {

		}
	}

	if (isCharsLookAheadNoWhite ("eE", _pos, _str, 1)) {
		jsonString += _str[_pos++];

		if (isCharsLookAheadNoWhite ("+-", _pos, _str, 1))
			jsonString += _str[_pos++];

		if (isCharsLookAheadNoWhite ("0123456789", _pos, _str, 1)) {
			jsonString += _str[_pos++];
			while (isCharsLookAheadNoWhite ("0123456789", _pos, _str, 1))
				jsonString += _str[_pos++];
		} else {

		}
	}

	jsonNumber = atof (jsonString.c_str());

	return new BjValue(jsonNumber);
}

BjArray* json::parseArray (long int& _pos,string& _str) {
	BjArray *array = new BjArray();

	if (isCharLookAhead('[', _pos, _str, 1)) {
		isChar ('[',_pos,_str);

		while (!isCharLookAhead(']', _pos, _str, 1)) {
			BjValue* v = parseValue(_pos, _str);

			array->array.push_back (v);

			if (isCharLookAhead(',', _pos, _str, 1)) {
				isChar (',', _pos, _str);
			}
		}

		if (isChar (']', _pos, _str)) {
			return array;
		} else {
			// Array dont end with ]???
		}
	} else {
		// Array didn't start with [ ???
	}
}


BjValue* json::parseValue (long int& _pos, string& _str) {
	if (isCharLookAhead ('"', _pos, _str, 1)) {
		return parseString(_pos,_str);
	} else if (isCharsLookAhead ("0123456789-", _pos, _str, 1)) {
		return parseNumber(_pos,_str);
	}
	else if (isCharLookAhead ('{', _pos, _str, 1)) {
		return new BjValue (parseObject(_pos,_str));
	}
	else if (isCharLookAhead ('[', _pos, _str, 1)) {
		return new BjValue (parseArray(_pos,_str));
	}
	else if (isCharLookAhead ('t', _pos, _str, 1)) {
		advance(_pos,_str);
		string t = "true";
		if (_str.substr (_pos,4) == t) {
			_pos += 4;
			return new BjValue (true);
		}
		else {
			// Error, not 'true'
		}
	}
	else if (isCharLookAhead ('f', _pos, _str, 1)) {
		advance(_pos,_str);
		string f = "false";
		if (_str.substr (_pos,5) == f) {
			_pos += 5;
			return new BjValue (false);
		}
		else {
			// Error, not 'false'
		}
	}
	else if (isCharLookAhead ('n', _pos, _str, 1)) {
		advance(_pos,_str);
		string f = "null";
		if (_str.substr (_pos,4) == f) {
			_pos += 4;
			return new BjValue ();
		}
		else {
			// Error, not 'false'
		}
	}
	else {
		// INVALID
	}
}

bool json::isChar (char _c, long int& _pos, string& _str) {
	return _c == getNextChar(_pos,_str);
}

bool json::isCharLookAhead (char _c, long int& _pos, string& _str, int _n) {
	long int pos = _pos;
	char c;

	for (int i=0;i<_n;i++)
		c = getNextChar(pos,_str);

	return _c == c;
}

bool json::isCharsLookAhead (string _s, long int& _pos, string& _str, int _n) {
	long int pos = _pos;
	char c;

	for (int i=0;i<_n;i++)
		c = getNextChar(pos,_str);

	return any_of(_s.begin(),_s.end(), [=](char _c) {return _c == c;});
}

bool json::isCharLookAheadNoWhite (char _c, long int& _pos, string& _str, int _n) {
	long int pos = _pos;
	char c;

	for (int i=0;i<_n;i++)
		c = getNextCharNoWhite(pos,_str);

	return _c == c;
}

bool json::isCharsLookAheadNoWhite (string _s, long int& _pos, string& _str, int _n) {
	long int pos = _pos;
	char c;

	for (int i=0;i<_n;i++)
		c = getNextCharNoWhite(pos,_str);

	return any_of(_s.begin(),_s.end(), [=](char _c) {return _c == c;});
}

char json::getNextChar (long int& _pos, string& _str) {
	char c;
	do {
		c = _str[_pos++];
	} while (c == ' ' || c == '\t' || c == '\n' || c == '\r');

	return c;
}

char json::getNextCharNoWhite (long int& _pos, string& _str) {
	char c;
	c = _str[_pos++];
	return c;
}

void json::advance (long int& _pos, string& _str) {
	char c;
	do {
		c = _str[_pos++];
	} while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
	_pos--;
}
