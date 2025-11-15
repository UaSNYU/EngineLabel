#pragma once

#include <string>
#include <chrono>
#include <fstream>
#include <thread>
#include <algorithm>
#include <iostream>

namespace Ciallo
{
	struct ProfileResult
	{
		std::string name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentorSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	private:
		InstrumentorSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	public:
		Instrumentor()
			:m_CurrentSession(nullptr), m_ProfileCount(0)
		{

		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			m_OutputStream.open(filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentorSession{ name };
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (m_ProfileCount++ > 0)
			{
				m_OutputStream << ",";
			}

			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ",";
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";
			m_OutputStream.flush();

		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\": [";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(std::string name)
			:m_Name(name), m_stopped(false)
		{
			m_StartTimePoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_stopped)
			{
				Stop();
			}
		}

		void Stop()
		{
			auto endTimePoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_Name,start,end,threadID });

			m_stopped = true;
		}

	private:
		std::string m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
		bool m_stopped;
	};
}

#define HZ_PROFILE 1
#if HZ_PROFILE
	#define HZ_PROFILE_BEGIN_SESSION(name, filepath) ::Ciallo::Instrumentor::Get().BeginSession(name, filepath)
	#define HZ_PROFILE_END_SESSION() ::Ciallo::Instrumentor::Get().EndSession()
	#define HZ_PROFILE_SCOPE(name) ::Ciallo::InstrumentationTimer timer##__LINE__(name)
	#define HZ_PROFILE_FUNCTION() HZ_PROFILE_SCOPE(__FUNCSIG__)
#else
#define HZ_PROFILE_BEGIN_SESSION(name, filepath)
#define HZ_PROFILE_END_SESSION()
#define HZ_PROFILE_SCOPE(name)
#define HZ_PROFILE_FUNCTION()
#endif