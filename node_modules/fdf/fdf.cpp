#include <node.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace v8;

bool createFDF_(const std::string& fileName, const std::map<std::string, std::string>& values)
{
	std::fstream fs(fileName, std::fstream::out);
	if (!fs.is_open())
		return false;

	std::stringstream fdfStream;

	// FDF header
	fdfStream << "%FDF-1.2" << std::endl;
	fdfStream << "1 0 obj<</FDF<< /Fields[" << std::endl;

	// FDF content
	for (const auto& value : values) {
		fdfStream << "<</T(" << value.first << ")/V(";

		for (const unsigned char byte : value.second) {
			if (byte == 0x28 || byte == 0x29 || byte == 0x5C)
				fdfStream << "\\" << std::string(&byte, &byte + 1);
			else if (byte < 32 || 126 < byte)
				fdfStream << "\x00" << std::string(&byte, &byte + 1);
			else
				fdfStream << std::string(&byte, &byte + 1);

		}
		fdfStream << ")>>" << std::endl;
	}

	// FDF footer
	fdfStream << "] >> >>" << std::endl;
	fdfStream << "endobj" << std::endl;
	fdfStream << "trailer" << std::endl;
	fdfStream << "<</Root 1 0 R>>" << std::endl;
	fdfStream << "%%EOF" << std::endl;

	fs << fdfStream.str();
	return true;
}

Handle<Value> createFDF(const Arguments& args)
{
	HandleScope scope;

	if (args.Length() != 2)
		return ThrowException(Exception::Error(String::New("The number of arguments must be 2!")));
	
	if(!args[0]->IsString())
		return ThrowException(Exception::Error(String::New("The first argument must be a string!")));

	if (!args[1]->IsObject())
		return ThrowException(Exception::Error(String::New("The second argument must be an object!")));

	if (args[0]->ToString().IsEmpty())
		return ThrowException(Exception::Error(String::New("The first argument's string is empty!")));

	if (args[1]->ToObject().IsEmpty())
		return ThrowException(Exception::Error(String::New("The second argument's object is empty!")));

	std::string fileName = *String::Utf8Value(args[0]->ToString());
	std::map<std::string, std::string> entries;

	Local<Object> jsEntries = args[1]->ToObject();
	Local<Array> properties = jsEntries->GetPropertyNames();
	for (uint32_t i = 0; i < properties->Length(); i++)
	{
		const Local<Value> key = properties->Get(i);
		const Local<Value> value = jsEntries->Get(key);

		if (!key->IsString() || !value->IsString())
			return ThrowException(Exception::Error(String::New("The second argument's object keys and values must be a string type!")));

		entries[*String::AsciiValue(key->ToString())] = *String::AsciiValue(value->ToString());
	}

	return scope.Close(Boolean::New(createFDF_(fileName, entries)));
}

void RegisterModule(Handle<Object> target) 
{
	target->Set(String::NewSymbol("createFDF"), FunctionTemplate::New(createFDF)->GetFunction());
}


NODE_MODULE(fdf, RegisterModule);