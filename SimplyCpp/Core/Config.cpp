#pragma once
#include "../stdafx.h"
#include "Config.h"

using namespace SimplyCpp::Core;

wxConfig* SimplyCpp::Core::AppConfig = new wxConfig(_("SimplyCpp-2.0.1"));
std::vector<Project> SimplyCpp::Core::OpenProjects;