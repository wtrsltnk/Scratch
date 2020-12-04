/* 
 * File:   Json.h
 * Author: wouter
 *
 * Created on June 1, 2012, 9:28 AM
 */

#ifndef JSON_H
#define	JSON_H

#include <string>
#include <vector>

namespace json
{
	
	class Parser
	{
	public:
		Parser();
		virtual ~Parser();
	private:

	};
	
	class Array;
	
	class Object
	{
	public:
		Object& getObject(const std::string& index);
		void set(const std::string& index, Object& object);
		Array& getArray(const std::string& index);
		void set(const std::string& index, Array& array);
		std::string& getString(const std::string& index);
		void set(const std::string& index, std::string& str);
		
	private:
		std::vector<Object> objects;
		std::vector<Array> arrays;
		std::vector<std::string> strings;
	};
	
	class Array
	{
	public:
	};
}

#endif	/* JSON_H */

