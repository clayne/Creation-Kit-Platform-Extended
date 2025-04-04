// Copyright � 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "..\NiAPI\NiMemoryManager.h"
#include "BSString.h"

#include <sstream>
#include <iomanip>

#define WDELIMITERS " \t\n\r\f\v"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		BSString::BSString() :
			m_data(NULL), m_dataLen(0), m_bufLen(0)
		{}

		BSString::BSString(LPCSTR string, WORD size) : BSString() {
			Set(string ? string : "", size);
		}

		BSString::BSString(const std::string& string, WORD size) : BSString() {
			Set(string.empty() ? "" : string.c_str(), size);
		}

		BSString::BSString(const BSString& string, WORD size) : BSString() {
			Set(string, size);
		}

		BSString::BSString(const BSString& string) : BSString() {
			Set(string, string.m_bufLen);
		}

		BSString::~BSString() {
			Clear();
		}

		BOOL BSString::Reserved(WORD size) {
			WORD newLen = size;
			WORD newSize = newLen + 1;
			auto newData = (LPSTR)NiAPI::NiMemoryManager::Alloc(NULL, newSize);
			if (!newData)
				return FALSE;

			if (m_bufLen) {
				strncpy(newData, m_data, newLen);
				NiAPI::NiMemoryManager::Free(NULL, (LPVOID)m_data);
				newData[newLen] = 0;
			}
			else
				newData[0] = 0;

			m_data = newData;
			m_bufLen = newSize;
			m_dataLen = newLen;

			return TRUE;
		}

		BOOL BSString::Set(LPCSTR string, WORD size) {
			pad0C = 0;

			WORD wNeedLen, wNeedBuf;

			wNeedLen = (string) ? (WORD)strlen(string) : 0;
			if (size && (wNeedLen > size))
				wNeedLen = size;

			wNeedBuf = wNeedLen + 1;

			if (wNeedBuf > m_bufLen) {
				if (!wNeedLen)
					goto __dealloc;
				else {
					LPSTR newData = (LPSTR)NiAPI::NiMemoryManager::Alloc(NULL, wNeedBuf, 4);
					memcpy(newData, string, wNeedLen);
					newData[wNeedLen] = 0;

					if (m_data)
						NiAPI::NiMemoryManager::Free(NULL, (LPVOID)m_data);

					WORD wRealBuf = (WORD)NiAPI::NiMemoryManager::Size(NULL, newData);

					m_data = newData;
					m_bufLen = wRealBuf > wNeedBuf ? wRealBuf : wNeedBuf;
					m_dataLen = wNeedLen;
				}
			}
			else {
				if (!string) {
				__dealloc:
					if (m_data) {
						NiAPI::NiMemoryManager::Free(NULL, (LPVOID)m_data);

						m_data = NULL;
						m_bufLen = 0;
						m_dataLen = 0;
					}
				}
				else {
					memcpy(m_data, string, wNeedLen);
					m_dataLen = wNeedLen;
					m_data[wNeedLen] = 0;
				}
			}

			return TRUE;
		}

		BOOL BSString::Set(const BSString& string, WORD size) 
		{
			return Set(string.m_data ? *string : "", size);
		}

		void BSString::Clear() {
			if (m_data) {
				NiAPI::NiMemoryManager::Free(NULL, (LPVOID)m_data);
				m_data = NULL;
				m_dataLen = 0;
				m_bufLen = 0;
			}
		}

		BSString BSString::Reverse() const {
			if (!m_dataLen)
				return "";

			auto s = BSString(m_data);
			return strrev(*s);
		}

		BSString& BSString::Format(LPCSTR format, ...) {
			Clear();

			va_list va;
			va_start(va, format);
			auto size = _vsnprintf(NULL, 0, format, va);
			if (size) {
				m_bufLen = size + 1;
				m_dataLen = size;
				m_data = (LPSTR)NiAPI::NiMemoryManager::Alloc(NULL, m_bufLen);
				if (!m_data) {
					m_bufLen = 0;
					m_dataLen = 0;
				}
				else {
					vsprintf(m_data, format, va);
					m_data[size] = 0;
				}
			}
			va_end(va);

			return *this;
		}

		BSString& BSString::FormatVa(LPCSTR format, va_list ap) {
			auto size = _vsnprintf(NULL, 0, format, ap);
			if (size) {
				m_bufLen = size + 1;
				m_dataLen = size;
				m_data = (LPSTR)NiAPI::NiMemoryManager::Alloc(NULL, m_bufLen);
				if (!m_data) {
					m_bufLen = 0;
					m_dataLen = 0;
				}
				else {
					vsprintf(m_data, format, ap);
					m_data[size] = 0;
				}
			}

			return *this;
		}

		INT BSString::Compare(LPCSTR string, BOOL ignoreCase) const {
			if (ignoreCase)
				return _stricmp(m_data, string);
			else
				return strcmp(m_data, string);
		}

		BSString& BSString::Append(LPCSTR str) {
			if (str) {
				WORD dwLen = (WORD)strlen(str);
				if (dwLen) {
					if (m_dataLen) {
						if (Reserved(m_dataLen + dwLen + 1))
							strcat(m_data, str);
					}
					else
						Set(str);
				}
			}

			return *this;
		}

		BSString& BSString::Append(LPCSTR str, WORD len)
		{
			if (str)
			{
				WORD dwLen = (WORD)strlen(str);
				if (dwLen) 
				{
					if (m_dataLen) 
					{
						if (dwLen <= len)
						{
							if (Reserved(m_dataLen + dwLen + 1))
								strcat(m_data, str);
						}
						else
						{
							if (Reserved(m_dataLen + len + 1))
								strncat(m_data, str, len);
						}
					}
					else
						Set(str, len);
				}
			}

			return *this;
		}

		BSString& BSString::Append(const BSString& string) 
		{
			if (!string.IsEmpty())
			{
				WORD dwLen = (WORD)string.m_dataLen;
				if (dwLen) 
				{
					if (m_dataLen) 
					{
						if (Reserved(m_dataLen + dwLen + 1))
							strcat(m_data, string.m_data);
					}
					else
						Set(string.m_data);
				}
			}

			return *this;
		}

		BSString& BSString::Append(const BSString& string, WORD len)
		{
			if (!string.IsEmpty())
			{
				WORD dwLen = (WORD)string.m_dataLen;
				if (dwLen)
				{
					if (m_dataLen)
					{
						if (dwLen <= len)
						{
							if (Reserved(m_dataLen + dwLen + 1))
								strcat(m_data, string.m_data);
						}
						else
						{
							if (Reserved(m_dataLen + len + 1))
								strncat(m_data, string.m_data, len);
						}
					}
					else
						Set(string.m_data, len);
				}
			}

			return *this;
		}

		BSString& BSString::Append(CHAR ch) {
			CHAR Buf[2] = { ch, 0 };
			return Append(Buf);
		}

		BSString& BSString::AppendFormat(LPCSTR format, ...) {
			BSString fmt;
			va_list va;
			va_start(va, format);
			fmt.FormatVa(format, va);
			va_end(va);

			return fmt.IsEmpty() ? *this : Append(fmt);
		}

		BSString& BSString::AppendFormatVa(LPCSTR format, va_list ap) {
			BSString fmt;
			fmt.FormatVa(format, ap);
			return fmt.IsEmpty() ? *this : Append(fmt);
		}

		BSString& BSString::Copy(WORD start, WORD len) {
			return Assign(m_data, start, len);
		}

		BSString& BSString::Assign(LPCSTR str, WORD start, WORD len) {
			if (str) {
				if (!len)
					len = (WORD)strlen(str);

				Set(*BSString(str + start), len);
			}

			return *this;
		}

		BSString& BSString::AssignUnsafe(LPCSTR str, WORD start, WORD len) {
			if (str) {
				if (!len)
					len = (WORD)strlen(str);

				Set(str + start, len);
			}

			return *this;
		}

		BSString& BSString::Erase(WORD start, WORD len) {
			if ((!start && !len) || (start >= len))
				return *this;

			if (!start) {
				if (len >= m_dataLen)
					Clear();
				else
					return Assign(m_data, len);
			}
			else {
				if (len >= m_dataLen)
					return Assign(m_data, 0, start);
				else
					return Assign(m_data, 0, start).Append(m_data + (start + len));
			}

			return *this;
		}

		WORD BSString::FindLastOf(CHAR ch, WORD offset) const {
			if (!m_data)
				return srNone;

			LPSTR Ret = strrchr(m_data + offset, ch);
			return Ret ? (WORD)(Ret - m_data) : srNone;
		}

		WORD BSString::FindFirstOf(CHAR ch, WORD offset) const {
			if (IsEmpty())
				return srNone;

			LPSTR Ret = strchr(m_data + offset, ch);
			return Ret ? (WORD)(Ret - m_data) : srNone;
		}

		WORD BSString::FindLastOf(LPCSTR chs, WORD offset) const {
			WORD Ret = Reverse().FindFirstOf(chs, offset);
			return Ret == srNone ? srNone : m_dataLen - Ret;
		}

		WORD BSString::FindFirstOf(LPCSTR chs, WORD offset) const {
			if (IsEmpty() || !chs)
				return srNone;

			LPSTR Ret = strpbrk(m_data + offset, chs);
			return Ret ? (WORD)(Ret - m_data) : srNone;
		}

		BSString BSString::UpperCase() const {
			if (IsEmpty())
				return "";

			auto s = BSString(m_data);
			UpperCase(*s);
			return s;
		}

		BSString BSString::LowerCase() const {
			if (IsEmpty())
				return "";

			auto s = BSString(m_data);
			LowerCase(*s);
			return s;
		}

		void BSString::UpperCase(LPCSTR str) {
			if (!str)
				return;

			strupr(const_cast<LPSTR>(str));
		}

		void BSString::LowerCase(LPCSTR str) {
			if (!str)
				return;

			strlwr(const_cast<LPSTR>(str));
		}

		WORD BSString::Find(LPCSTR substr, EFlags flags) const {
			if (IsEmpty() || !substr)
				return srNone;

			LPSTR Ret = NULL;
			if (flags == sfInsensitiveCase)
				Ret = strstr(*LowerCase(), *(BSString(substr).LowerCase()));
			else
				LPSTR Ret = strstr(m_data, substr);

			return Ret ? (WORD)(Ret - m_data) : srNone;
		}

		BSString BSString::Trim() const {
			if (IsEmpty())
				return "";

			return CreationKitPlatformExtended::Utils::Trim(m_data).c_str();
		}

		BSString BSString::FormatString(LPCSTR format, ...) {
			BSString fmt;
			va_list va;

			va_start(va, format);
			fmt.FormatVa(format, va);
			va_end(va);

			return fmt;
		}

		BSString BSString::Utils::ExtractFilePath(const BSString& fname) {
			WORD it = fname.FindLastOf("/\\");

			if (it != srNone)
				return BSString(*fname, it);

			return fname;
		}

		BSString BSString::Utils::ExtractFileName(const BSString& fname) {
			WORD it = fname.FindLastOf("/\\");

			if (it != srNone)
				return BSString(*fname + it);

			return fname;
		}

		BSString BSString::Utils::ExtractFileExt(const BSString& fname) {
			WORD it = fname.FindLastOf('.');

			if (it != srNone)
				return BSString(*fname + it);

			return "";
		}

		BSString BSString::Utils::GetCurrentPath() {
			constexpr auto SIZE = 1024;
			CHAR szTemp[SIZE];

			if (::GetCurrentDirectoryA(SIZE, szTemp))
				return BSString(szTemp) + "\\";

			return "";
		}

		BSString BSString::Utils::GetFileNameModule(const BSString& mname) {
			constexpr auto SIZE = 1024;
			CHAR szTemp[SIZE];

			if (::GetModuleFileNameA(GetModuleHandleA(mname.IsEmpty() ? NULL : *mname), szTemp, MAX_PATH))
				return szTemp;

			return "";
		}

		BSString BSString::Utils::GetApplicationPath() 
		{
			return ExtractFilePath(GetFileNameModule(""));
		}

		BSString BSString::Utils::GetPluginsPath()
		{
			return GetApplicationPath() + "CKPEPlugins\\";
		}

		BSString BSString::Utils::ChangeFileExt(const BSString& fname, const BSString& ext) {
			WORD it = fname.FindLastOf('.');

			if (it != srNone)
				return BSString(*fname, it).Append(ext);

			return BSString(fname).Append(ext);
		}

		BSString BSString::Utils::GetRelativeDataPath() 
		{
			return "Data\\";
		}

		BSString BSString::Utils::GetDataPath()
		{
			return GetApplicationPath() + "Data\\";
		}

		BOOL BSString::Utils::FileExists(const BSString& fname) 
		{
			return CreationKitPlatformExtended::Utils::FileExists(*fname);
		}

		BOOL BSString::Utils::DirectoryExists(const BSString& fname) 
		{
			return CreationKitPlatformExtended::Utils::DirectoryExists(*fname);
		}

		BSString BSString::Transforms::BoolToStr(BOOL value, BOOL inText) {
			if (inText)
				return (value) ? "True" : "False";
			else
				return (value) ? "1" : "0";
		}

		BSString BSString::Transforms::IntToStr(INT64 value) {
			CHAR szBuffer[128];
			return _i64toa(value, szBuffer, 10);
		}

		BSString BSString::Transforms::UIntToStr(UINT64 value) {
			CHAR szBuffer[128];
			return _ui64toa(value, szBuffer, 10);
		}

		BSString BSString::Transforms::FloatToStr(DOUBLE value) {
			return std::to_string(value);
		}

		BOOL BSString::Transforms::StrToBool(const BSString& value) {
			if (*value && ((value == "1") || (value == "True")))
				return TRUE;

			return FALSE;
		}

		INT64 BSString::Transforms::StrToInt(const BSString& value) {
			LPSTR lpEnd = NULL;
			return strtoll(*value, &lpEnd, 10);
		}

		UINT64 BSString::Transforms::StrToUInt(const BSString& value) {
			LPSTR lpEnd = NULL;
			return strtoull(*value, &lpEnd, 10);
		}

		DOUBLE BSString::Transforms::StrToFloat(const BSString& value) {
			LPSTR lpEnd = NULL;
			return strtod(*value, &lpEnd);
		}

		INT64 BSString::Transforms::HexToInt(const BSString& value) {
			LPSTR lpEnd = NULL;
			return strtoll(*value, &lpEnd, 16);
		}

		UINT64 BSString::Transforms::HexToUInt(const BSString& value) {
			LPSTR lpEnd = NULL;
			return strtoull(*value, &lpEnd, 16);
		}

		BSString BSString::Transforms::IntToHex(INT64 value) {
			std::stringstream stream;
			stream << "0x"
				<< std::setfill('0') << std::setw(sizeof(INT64) * 2)
				<< std::hex << value;
			return stream.str();
		}

		BSString BSString::Transforms::UIntToHex(UINT64 value) {
			std::stringstream stream;
			stream << "0x"
				<< std::setfill('0') << std::setw(sizeof(UINT64) * 2)
				<< std::hex << value;
			return stream.str();
		}

		BSString BSString::Converts::Utf8ToAnsi(const BSString& str) {
			if (Conversion::IsUtf8Valid(*str))
				return Conversion::Utf8ToAnsi(*str).c_str();
			else
				return str;
		}

		BSString BSString::Converts::AnsiToUtf8(const BSString& str) {
			if (!Conversion::IsUtf8Valid(*str))
				return Conversion::AnsiToUtf8(*str).c_str();
			else
				return str;
		}

		BSString BSString::Converts::WideToAnsi(LPWSTR str) {
			return Conversion::WideToAnsi(str).c_str();
		}

		BSStringEx::BSStringEx() :
			m_data(nullptr), m_dataLen(0), m_bufLen(0)
		{}

		BSStringEx::BSStringEx(const char* string, uint32_t size) : BSStringEx() {
			Set(string ? string : "", size);
		}

		BSStringEx::BSStringEx(const std::string& string, uint32_t size) : BSStringEx() {
			Set(string.empty() ? "" : string.c_str(), size);
		}

		BSStringEx::BSStringEx(const BSStringEx& string, uint32_t size) : BSStringEx() {
			Set(string, size);
		}

		BSStringEx::BSStringEx(const BSStringEx& string) : BSStringEx() {
			Set(string, string.m_bufLen);
		}

		BSStringEx::~BSStringEx() {
			Clear();
		}

		bool BSStringEx::Reserved(uint32_t size) {
			uint32_t newLen = size;
			uint32_t newSize = newLen + 1;
			auto newData = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, newSize);
			if (!newData)
				return false;

			if (m_bufLen) {
				strncpy(newData, m_data, newLen);
				NiAPI::NiMemoryManager::Free(nullptr, (LPVOID)m_data);
				newData[newLen] = 0;
			}
			else
				newData[0] = 0;

			m_data = newData;
			m_bufLen = newSize;
			m_dataLen = newLen;

			return true;
		}

		bool BSStringEx::Set(const char* string, uint32_t size) {
			uint32_t wNeedLen, wNeedBuf;

			wNeedLen = (string) ? (uint32_t)strlen(string) : 0;
			if (size && (wNeedLen > size))
				wNeedLen = size;

			wNeedBuf = wNeedLen + 1;

			if (wNeedBuf > m_bufLen) {
				if (!wNeedLen)
					goto __2dealloc2;
				else {
					char* newData = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, wNeedBuf, 4);
					memcpy(newData, string, wNeedLen);
					newData[wNeedLen] = 0;

					if (m_data)
						NiAPI::NiMemoryManager::Free(nullptr, (void*)m_data);

					uint32_t wRealBuf = (uint32_t)NiAPI::NiMemoryManager::Size(nullptr, newData);

					m_data = newData;
					m_bufLen = wRealBuf > wNeedBuf ? wRealBuf : wNeedBuf;
					m_dataLen = wNeedLen;
				}
			}
			else {
				if (!string) {
				__2dealloc2:
					if (m_data) {
						NiAPI::NiMemoryManager::Free(nullptr, (LPVOID)m_data);

						m_data = nullptr;
						m_bufLen = 0;
						m_dataLen = 0;
					}
				}
				else {
					memcpy(m_data, string, wNeedLen);
					m_dataLen = wNeedLen;
					m_data[wNeedLen] = 0;
				}
			}

			return true;
		}

		bool BSStringEx::Set(const BSStringEx& string, uint32_t size)
		{
			return Set(string.m_data ? *string : "", size);
		}

		void BSStringEx::Clear() {
			if (m_data) {
				NiAPI::NiMemoryManager::Free(nullptr, (void*)m_data);
				m_data = nullptr;
				m_dataLen = 0;
				m_bufLen = 0;
			}
		}

		BSStringEx BSStringEx::Reverse() const {
			if (!m_dataLen)
				return "";

			auto s = BSStringEx(m_data);
			return strrev(*s);
		}

		BSStringEx& BSStringEx::Format(const char* format, ...) {
			Clear();

			va_list va;
			va_start(va, format);
			auto size = _vsnprintf(nullptr, 0, format, va);
			if (size) {
				m_bufLen = size + 1;
				m_dataLen = size;
				m_data = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, m_bufLen);
				if (!m_data) {
					m_bufLen = 0;
					m_dataLen = 0;
				}
				else {
					vsprintf(m_data, format, va);
					m_data[size] = 0;
				}
			}
			va_end(va);

			return *this;
		}

		BSStringEx& BSStringEx::FormatVa(const char* format, va_list ap) {
			auto size = _vsnprintf(nullptr, 0, format, ap);
			if (size) {
				m_bufLen = size + 1;
				m_dataLen = size;
				m_data = (char*)NiAPI::NiMemoryManager::Alloc(nullptr, m_bufLen);
				if (!m_data) {
					m_bufLen = 0;
					m_dataLen = 0;
				}
				else {
					vsprintf(m_data, format, ap);
					m_data[size] = 0;
				}
			}

			return *this;
		}

		int32_t BSStringEx::Compare(const char* string, bool ignoreCase) const {
			if (ignoreCase)
				return _stricmp(m_data, string);
			else
				return strcmp(m_data, string);
		}

		BSStringEx& BSStringEx::Append(const char* str) {
			if (str) {
				uint32_t dwLen = (uint32_t)strlen(str);
				if (dwLen) {
					if (m_dataLen) {
						if (Reserved(m_dataLen + dwLen + 1))
							strcat(m_data, str);
					}
					else
						Set(str);
				}
			}

			return *this;
		}

		BSStringEx& BSStringEx::Append(const char* str, uint32_t len)
		{
			if (str)
			{
				uint32_t dwLen = (uint32_t)strlen(str);
				if (dwLen)
				{
					if (m_dataLen)
					{
						if (dwLen <= len)
						{
							if (Reserved(m_dataLen + dwLen + 1))
								strcat(m_data, str);
						}
						else
						{
							if (Reserved(m_dataLen + len + 1))
								strncat(m_data, str, len);
						}
					}
					else
						Set(str, len);
				}
			}

			return *this;
		}

		BSStringEx& BSStringEx::Append(const BSStringEx& string)
		{
			if (!string.IsEmpty())
			{
				uint32_t dwLen = (uint32_t)string.m_dataLen;
				if (dwLen)
				{
					if (m_dataLen)
					{
						if (Reserved(m_dataLen + dwLen + 1))
							strcat(m_data, string.m_data);
					}
					else
						Set(string.m_data);
				}
			}

			return *this;
		}

		BSStringEx& BSStringEx::Append(const BSStringEx& string, uint32_t len)
		{
			if (!string.IsEmpty())
			{
				uint32_t dwLen = (uint32_t)string.m_dataLen;
				if (dwLen)
				{
					if (m_dataLen)
					{
						if (dwLen <= len)
						{
							if (Reserved(m_dataLen + dwLen + 1))
								strcat(m_data, string.m_data);
						}
						else
						{
							if (Reserved(m_dataLen + len + 1))
								strncat(m_data, string.m_data, len);
						}
					}
					else
						Set(string.m_data, len);
				}
			}

			return *this;
		}

		BSStringEx& BSStringEx::Append(char ch) {
			CHAR Buf[2] = { ch, 0 };
			return Append(Buf);
		}

		BSStringEx& BSStringEx::AppendFormat(const char* format, ...) {
			BSStringEx fmt;
			va_list va;
			va_start(va, format);
			fmt.FormatVa(format, va);
			va_end(va);

			return fmt.IsEmpty() ? *this : Append(fmt);
		}

		BSStringEx& BSStringEx::AppendFormatVa(const char* format, va_list ap) {
			BSStringEx fmt;
			fmt.FormatVa(format, ap);
			return fmt.IsEmpty() ? *this : Append(fmt);
		}

		BSStringEx& BSStringEx::Copy(uint32_t start, uint32_t len) {
			return Assign(m_data, start, len);
		}

		BSStringEx& BSStringEx::Assign(const char* str, uint32_t start, uint32_t len) {
			if (str) {
				if (!len)
					len = (uint32_t)strlen(str);

				Set(*BSStringEx(str + start), len);
			}

			return *this;
		}

		BSStringEx& BSStringEx::AssignUnsafe(const char* str, uint32_t start, uint32_t len) {
			if (str) {
				if (!len)
					len = (uint32_t)strlen(str);

				Set(str + start, len);
			}

			return *this;
		}

		BSStringEx& BSStringEx::Erase(uint32_t start, uint32_t len) {
			if ((!start && !len) || (start >= len))
				return *this;

			if (!start) {
				if (len >= m_dataLen)
					Clear();
				else
					return Assign(m_data, len);
			}
			else {
				if (len >= m_dataLen)
					return Assign(m_data, 0, start);
				else
					return Assign(m_data, 0, start).Append(m_data + (start + len));
			}

			return *this;
		}

		uint32_t BSStringEx::FindLastOf(char ch, uint32_t offset) const {
			if (!m_data)
				return srNone;

			char* Ret = strrchr(m_data + offset, ch);
			return Ret ? (uint32_t)(Ret - m_data) : srNone;
		}

		uint32_t BSStringEx::FindFirstOf(char ch, uint32_t offset) const {
			if (IsEmpty())
				return srNone;

			char* Ret = strchr(m_data + offset, ch);
			return Ret ? (uint32_t)(Ret - m_data) : srNone;
		}

		uint32_t BSStringEx::FindLastOf(const char* chs, uint32_t offset) const {
			uint32_t Ret = Reverse().FindFirstOf(chs, offset);
			return Ret == srNone ? srNone : m_dataLen - Ret;
		}

		uint32_t BSStringEx::FindFirstOf(const char* chs, uint32_t offset) const {
			if (IsEmpty() || !chs)
				return srNone;

			char* Ret = strpbrk(m_data + offset, chs);
			return Ret ? (uint32_t)(Ret - m_data) : srNone;
		}

		BSStringEx BSStringEx::UpperCase() const {
			if (IsEmpty())
				return "";

			auto s = BSStringEx(m_data);
			UpperCase(*s);
			return s;
		}

		BSStringEx BSStringEx::LowerCase() const {
			if (IsEmpty())
				return "";

			auto s = BSStringEx(m_data);
			LowerCase(*s);
			return s;
		}

		void BSStringEx::UpperCase(const char* str) {
			if (!str)
				return;

			strupr(const_cast<char*>(str));
		}

		void BSStringEx::LowerCase(const char* str) {
			if (!str)
				return;

			strlwr(const_cast<char*>(str));
		}

		uint32_t BSStringEx::Find(const char* substr, EFlags flags) const {
			if (IsEmpty() || !substr)
				return srNone;

			char* Ret = nullptr;
			if (flags == sfInsensitiveCase)
				Ret = strstr(*LowerCase(), *(BSStringEx(substr).LowerCase()));
			else
				char* Ret = strstr(m_data, substr);

			return Ret ? (uint32_t)(Ret - m_data) : srNone;
		}

		BSStringEx BSStringEx::Trim() const {
			if (IsEmpty())
				return "";

			return CreationKitPlatformExtended::Utils::Trim(m_data).c_str();
		}

		BSStringEx BSStringEx::FormatString(const char* format, ...) {
			BSStringEx fmt;
			va_list va;

			va_start(va, format);
			fmt.FormatVa(format, va);
			va_end(va);

			return fmt;
		}
	}
}