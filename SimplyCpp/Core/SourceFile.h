#pragma once
#include "../stdafx.h"

namespace SimplyCpp
{
    namespace Core
    {
        class SourceFile
        {
        public:
            SourceFile(std::string filename);

            void SetFileName(std::string filename);

            std::string GetFileName();

        private:
            std::string m_filename;
        };
    }
}