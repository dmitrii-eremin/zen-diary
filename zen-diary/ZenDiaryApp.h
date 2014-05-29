#pragma once

namespace ZenDiary
{
	namespace App
	{
		class ZenDiaryApp : public Interfaces::IApplication
		{
		public:
			ZenDiaryApp();
			virtual ~ZenDiaryApp();

			ZenDiaryApp(const ZenDiaryApp &a) = delete;
			ZenDiaryApp &operator = (const ZenDiaryApp &a) = delete;

			virtual ZD_STATUS Initialize(const std::string &argv) override final;
			virtual ZD_STATUS Deinitialize() override final;
			virtual ZD_STATUS Run() override final;

			virtual ZD_STATUS Terminate() override final;

		private:
			std::atomic_bool m_terminate;
		};
	};
};