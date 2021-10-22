#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <chrono>

#define XXH_INLINE_ALL
#include "xxhash.h"

#include <unordered_map>
#include <unordered_set>
#include <set>

#define TEST_COUNT 1000000
#define TEST_STRING "michiamogregandmydckisbig.DAT"
#define TEST_SIZE 31

using namespace std;

INT64 GetHashCode64(const char* String)
{
	unsigned long i, len;
	INT64 ch;
	INT64 result;
	INT64 XOR;
	INT64 temp = 0;

	len = (long)strlen(String);
	result = 0;
	XOR = 0;
	for (i = 0; i < len; i++)
	{
		ch = (INT64)(String[i] * String[i]);
		temp += ch;
		result = ((result << 8) | ch) % 0x00fffffffffffffd;
		XOR ^= ch;
	}

	return (result | (XOR << 48)) + temp;
}

__forceinline INT64 GetHashCode64inl(const char* String)
{
	unsigned long i, len;
	INT64 ch;
	INT64 result;
	INT64 XOR;
	INT64 temp = 0;

	len = (long)strlen(String);
	result = 0;
	XOR = 0;
	for (i = 0; i < len; i++)
	{
		ch = (INT64)(String[i] * String[i]);
		temp += ch;
		result = ((result << 8) | ch) % 0x00fffffffffffffd;
		XOR ^= ch;
	}

	return (result | (XOR << 48)) + temp;
}

INT64 hashdjb264(const char* str)
{
	INT64 hash = 1255381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
} 

__forceinline INT64 hashdjb264inl(const char* str)
{
	INT64 hash = 1255381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

void TEST_HASHING()
{
	while (1)
	{
		//GetHashCode64
		auto start = chrono::steady_clock::now();
		for (int i = 0; i < TEST_COUNT; i++)
		{
			INT64 dwGetHashCode64 = GetHashCode64(TEST_STRING);
			if (dwGetHashCode64 == 0)
			{
				printf("dwGetHashCode64 fail\n");
				break;
			}
		}
		auto end = chrono::steady_clock::now();

		cout << "GetHashCode64 : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< "/"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ns/ms" << endl;

		//GetHashCode64inl
		start = chrono::steady_clock::now();
		for (int i = 0; i < TEST_COUNT; i++)
		{
			INT64 dwGetHashCode64inl = GetHashCode64inl(TEST_STRING);
			if (dwGetHashCode64inl == 0) {
				printf("dwGetHashCode64inl fail\n");
				break;
			}
		}
		end = chrono::steady_clock::now();

		cout << "GetHashCode64inl : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< "/"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ns/ms" << endl;


		//hashdjb264
		start = chrono::steady_clock::now();
		for (int i = 0; i < TEST_COUNT; i++)
		{
			INT64 dwhashdjb264 = hashdjb264(TEST_STRING);
			if (dwhashdjb264 == 0)
			{
				printf("dwhashdjb264 fail\n");
				break;
			}

		}
		end = chrono::steady_clock::now();

		cout << "hashdjb264 : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< "/"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ns/ms" << endl;


		//hashdjb264inl
		start = chrono::steady_clock::now();
		for (int i = 0; i < TEST_COUNT; i++)
		{
			INT64 dwhashdjb264inl = hashdjb264inl(TEST_STRING);
			if (dwhashdjb264inl == 0)
			{
				printf("dwhashdjb264inl fail\n");
				break;
			}
		}
		end = chrono::steady_clock::now();

		cout << "hashdjb264inl : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< "/"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ns/ms" << endl;

		//XXH64
		//XXH64 : 65806800/65 ns/ms
		start = chrono::steady_clock::now();
		for (int i = 0; i < TEST_COUNT; i++)
		{
			XXH64_hash_t dwXXH64 = XXH64(TEST_STRING, (int)strlen(TEST_STRING), 42);
			if (dwXXH64 == 0)
			{
				printf("dwXXH64 fail\n");
				break;
			}
		}
		end = chrono::steady_clock::now();

		cout << "XXH64 : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< "/"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ns/ms" << endl;

		Sleep(2000);

	}
	XXH64_hash_t hash = XXH64(TEST_STRING, TEST_SIZE, 42);
}

void TEST_MAPSEARCH()
{

}

int main()
{
	while (1)
	{
		/////////    ////////
		/// UNORDERED MAP ///
		/////////   /////////
		std::unordered_map< XXH64_hash_t, bool> unorderedmap;
		//XXH64
		auto start = chrono::steady_clock::now();
		char strToInsert[] = TEST_STRING;
		for (int i = 0; i < TEST_COUNT; i++)
		{
			strToInsert[i % 31] = rand() % 255;
			XXH64_hash_t dwXXH64 = XXH64(strToInsert, (int)strlen(TEST_STRING), 42);
			unorderedmap.insert({ dwXXH64, 0 });
			if (dwXXH64 == 0)
			{
				printf("dwXXH64 fail\n");
				break;
			}
		}
		auto end = chrono::steady_clock::now();

		cout << "unordered_map insertion : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t" 
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		start = chrono::steady_clock::now();
		for (const auto& it : unorderedmap)
		{
			if (it.second == true)
			{
				printf("error?\n");
			}
		}
		end = chrono::steady_clock::now();

		cout << "unordered_map access : " 
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		start = chrono::steady_clock::now();
		const auto& it = unorderedmap.find(0);
		if (it != unorderedmap.end())
		{
			printf("error?\n");
		}
		end = chrono::steady_clock::now();

		cout << "unordered_map find : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		/////////    ////////
		/// UNORDERED SET ///
		/////////   /////////

		std::unordered_set< XXH64_hash_t > unorderedset;
		start = chrono::steady_clock::now();
		for (int i = 0; i < TEST_COUNT; i++)
		{
			strToInsert[i % 31] = rand() % 255;
			XXH64_hash_t dwXXH64 = XXH64(strToInsert, (int)strlen(TEST_STRING), 42);
			unorderedset.insert(dwXXH64);
			if (dwXXH64 == 0)
			{
				printf("dwXXH64 fail\n");
				break;
			}
		}
		end = chrono::steady_clock::now();

		cout << "unordered_set insertion : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		start = chrono::steady_clock::now();
		for (const auto& it : unorderedset)
		{
			if (it == 0)
			{
				printf("error?\n");
			}
		}
		end = chrono::steady_clock::now();

		cout << "unordered_set access : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		start = chrono::steady_clock::now();
		const auto& itset = unorderedset.find(0);
		if (itset != unorderedset.end())
		{
			printf("error?\n");
		}
		end = chrono::steady_clock::now();

		cout << "unordered_set find : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		/////////    ////////
		///		 SET      ///
		/////////   /////////

		std::set< XXH64_hash_t > set;
		start = chrono::steady_clock::now();
		for (int i = 0; i < TEST_COUNT; i++)
		{
			strToInsert[i % 31] = rand() % 255;
			XXH64_hash_t dwXXH64 = XXH64(strToInsert, (int)strlen(TEST_STRING), 42);
			set.insert(dwXXH64);
			if (dwXXH64 == 0)
			{
				printf("dwXXH64 fail\n");
				break;
			}
		}
		end = chrono::steady_clock::now();

		cout << "set insertion : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		start = chrono::steady_clock::now();
		for (const auto& it : set)
		{
			if (it == 0)
			{
				printf("error?\n");
			}
		}
		end = chrono::steady_clock::now();

		cout << "set access : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		start = chrono::steady_clock::now();
		const auto& itset1 = set.find(0);
		if(itset1 != set.end())
		{
			printf("error?\n");
		}
		end = chrono::steady_clock::now();

		cout << "set find : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		/////////    ////////
		///		 vector   ///
		/////////   /////////

		std::vector< XXH64_hash_t > vec;
		start = chrono::steady_clock::now();
		for (int i = 0; i < TEST_COUNT; i++)
		{
			strToInsert[i % 31] = rand() % 255;
			XXH64_hash_t dwXXH64 = XXH64(strToInsert, (int)strlen(TEST_STRING), 42);
			vec.push_back(dwXXH64);
			if (dwXXH64 == 0)
			{
				printf("dwXXH64 fail\n");
				break;
			}
		}
		end = chrono::steady_clock::now();

		cout << "vector insertion : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		start = chrono::steady_clock::now();
		for (const auto& it : vec)
		{
			if (it == 0)
			{
				printf("error?\n");
			}
		}
		end = chrono::steady_clock::now();

		cout << "vector access : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

		start = chrono::steady_clock::now();
		const auto& itvec = std::find(vec.begin(), vec.end(), 0);
		if (itvec != vec.end())
		{
			printf("error?\n");
		}
		end = chrono::steady_clock::now();

		cout << "vector find : "
			<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
			<< " ns \t"
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;


		cout << endl;
		Sleep(1000);

	}
	return 0;
}