#ifndef __baseModule_H
#define __baseModule_H

#include <stdio.h>
#include <string>
#include "interrupt.hpp"

namespace module {
	template <class T>
	class BaseModule {
	public:
		static T& getInstance(void) {
			static T _instance;
			return _instance;
		}

		virtual void update(void) = 0;

		void cycle(void) {
			uint32_t start_usec = module::interrupt::getInstance().getElapsedUsec();
			update();
			_cycle_time_usec = module::interrupt::getInstance().getElapsedUsec() - start_usec;
		}

		virtual void printCycleTime(void) {
			printf("    %s	: %ld [us]\r\n", getModuleName().c_str(), _cycle_time_usec);
		}

		std::string getModuleName(void) {
			return _module_name;
		}

		void setModuleName(std::string name_str) {
			_module_name = name_str;
		}

	protected:
		float _delta_t;
		std::string _module_name;
		uint32_t _cycle_time_usec;

		BaseModule():
			_delta_t{0.001f},
			_module_name{"BaseModule"},
			_cycle_time_usec{0}
		{ }

	private:
		BaseModule(const BaseModule&) = delete;
		BaseModule& operator=(const BaseModule&) = delete;
		BaseModule(BaseModule&&) = delete;
		BaseModule& operator=(BaseModule&&) = delete;
	};
}

#endif