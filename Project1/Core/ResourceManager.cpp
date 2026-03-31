#include "ResourceManager.h"
#include <fstream>


std::optional<std::reference_wrapper<const std::deque<std::string>>> ResourceManager::GetResource(const std::string& file_path)
{
	// 이미 읽어온 파일이면 해당 내용 반환
	auto it = data.find(file_path);
	if (it != data.end()) {
		return it->second;
	}

	std::deque<std::string>& resource = data[file_path];
	std::ifstream in(file_path);
	
	// 파일이 없다면 nullopt 반환
	if (!in.is_open()) {
		return std::nullopt;
	}
	else {
        std::string line;
        bool is_first_line = true; // 첫 번째 줄인지 확인하는 플래그

        while (std::getline(in, line)) {
            // 첫 줄을 읽었을 때 BOM(0xEF, 0xBB, 0xBF)이 존재하는지 검사하고 제거
            if (is_first_line) {
                if (line.size() >= 3 &&
                    (unsigned char)line[0] == 0xEF &&
                    (unsigned char)line[1] == 0xBB &&
                    (unsigned char)line[2] == 0xBF)
                {
                    line = line.substr(3); // 앞의 3바이트를 날려버림
                }
                is_first_line = false;
            }

            resource.push_back(line);
        }
	} 
	in.close();

	return resource;
}
