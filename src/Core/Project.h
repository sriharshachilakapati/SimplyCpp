#pragma once
#include "../stdafx.h"
#include "SourceFile.h"

namespace SimplyCpp
{
    namespace Core
    {
        class Project
        {
        public:
            Project() : m_name("Untitled Project") {}
            Project(std::string name) : m_name(name) {}

            void SetName(std::string name);
            std::string GetName();

            std::vector<SourceFile> & GetFiles();

            void LoadFile(std::string fileName);
            void SaveFile(std::string fileName);

        private:
            std::string m_name;
            std::vector<SourceFile> m_files;
        };
    }
}