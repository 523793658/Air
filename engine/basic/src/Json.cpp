#include "basic/include/Basic.h"
#include "basic/include/Util.h"
#include "basic/include/Json.hpp"

#include "basic/include/ResIdentifier.h"


namespace Air
{
	JsonPtr Json::parse(std::string jsonstr)
	{
		JsonPtr json = MakeSharedPtr<Json>();
		json->mDoc.Parse(jsonstr.c_str());
		return json;
	}


	JsonValuePtr Json::operator[](std::string key)
	{
		rapidjson::Value& v = mDoc[key.c_str()];
		return MakeSharedPtr<JsonValue>(&v);
	}


	JsonValue::JsonValue(rapidjson::Value* value)
		:mValue(value)
	{
	}

	int JsonValue::operator = (int32_t value)
	{
		mValue->SetInt(value);
		return value;
	}
	float JsonValue::operator = (float value)
	{
		mValue->SetFloat(value);
		return value;
	}


	std::string JsonValue::operator = (std::string value)
	{
		mValue->SetString(value.c_str(), value.length());
		return value;
	}

	bool JsonValue::operator=(bool value)
	{
		mValue->SetBool(value);
		return value;
	}

	uint32_t JsonValue::operator = (uint32_t value)
	{
		mValue->SetUint(value);
		return value;
	}
	int64_t JsonValue::operator = (int64_t value)
	{
		mValue->SetInt64(value);
		return value;
	}
	uint64_t JsonValue::operator = (uint64_t value)
	{
		mValue->SetUint64(value);
		return value;
	}
	double JsonValue::operator = (double value)
	{
		mValue->SetDouble(value);
		return value;
	}

	JsonValue& JsonValue::operator = (JsonValue & value)
	{
		mValue = value.mValue;
		return value;
	}

	int JsonValue::getInt() const
	{
		return mValue->GetInt();
	}


	float JsonValue::getFloat() const
	{
		return mValue->GetFloat();
	}

	bool JsonValue::getBool() const
	{
		return mValue->GetBool();
	}

	std::string JsonValue::getString() const
	{
		return std::string(mValue->GetString(), mValue->GetStringLength());
	}


	JsonPtr syncLoadJson(ResIdentifierPtr res)
	{
		std::vector<char> data;
		data.resize(res->size());
		res->read(&data[0], res->size());
		return Json::parse(std::string(&data[0], data.size()));
	}

}