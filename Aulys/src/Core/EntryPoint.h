#pragma once

#include "Application.h"
#include "Log/Log.h"

extern Aulys::Application* Aulys::CreateApplication();

int main(int argc, char** argv)
{
	Aulys::Log::Init();
	Aulys::Log::mCoreLogset->logger->info("\n\n\n --- Aulys Engine (https://github.com/noidawt/aulys-engine) --- \n\n");
	ALOGIL("Creating app...");
	auto application = Aulys::CreateApplication();
	ALOGIL("... App created, calling application->Run()!");
	application->Run();
	Aulys::Log::flush();
	delete application;
};
