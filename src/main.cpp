//#include <QApplication>
//#include "mainwindow.h"
//#include "fuzzer.h"
//#include "target.h"
#include "fuzzer.h"
//#include <stdio.h>

#include "easylogging++.h"

_INITIALIZE_EASYLOGGINGPP


void setupLogger()
{
	el::Configurations defaultConf;
	defaultConf.setToDefault();

	defaultConf.setGlobally(
			el::ConfigurationType::ToStandardOutput, "false");

	defaultConf.setGlobally(
			el::ConfigurationType::Format, "%datetime %msg");
	defaultConf.setGlobally(
			el::ConfigurationType::Filename, "fuzzer.log");
	el::Loggers::reconfigureLogger("default", defaultConf);


	LOG(INFO) << "";
	LOG(INFO) << "----------------------------------------------------";
	LOG(INFO) << "----------------------------------------------------";
	LOG(INFO) << "";

	defaultConf.set(el::Level::Debug, 
			el::ConfigurationType::Format, "%datetime %func %msg");
	defaultConf.set(el::Level::Debug, 
			el::ConfigurationType::Filename, "debug.log");

	el::Loggers::reconfigureLogger("default", defaultConf);
	LOG(DEBUG) << "";
	LOG(DEBUG) << "----------------------------------------------------";
	LOG(DEBUG) << "----------------------------------------------------";
	LOG(DEBUG) << "";
}


int main(int argc, char *argv[])
{
	_START_EASYLOGGINGPP(argc, argv);
	setupLogger();
	Fuzzer* fuzzer = new Fuzzer(argc-1, &argv[1]);
	return 0;
}
