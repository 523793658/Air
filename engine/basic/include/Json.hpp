#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"
#include "basic/include/PreDeclare.h"
namespace Air
{
	class JsonValue : public rapidjson::Value
	{
	public:
		JsonValue(rapidjson::Value* value);

		int operator = (int32_t value);
		uint32_t operator = (uint32_t value);
		int64_t operator = (int64_t value);
		uint64_t operator = (uint64_t value);
		float operator = (float value);
		double operator = (double value);
		bool operator = (bool value);
		JsonValue& operator = (JsonValue & value);
		std::string operator = (std::string value);

		int getInt() const;
		float getFloat() const;
		bool getBool() const;
		std::string getString() const;

	private:
		rapidjson::Value* mValue;
	};

	class Json
	{
	public:
		static JsonPtr parse(std::string jsonstr);


	public:
		JsonValuePtr operator [](std::string key);
	private:
		rapidjson::Document mDoc;
	};

	JsonPtr syncLoadJson(ResIdentifierPtr path);
}