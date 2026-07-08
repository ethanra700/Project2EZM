#pragma once

#include <string>
#include <vector>

#include "GymRecord.h"

std::vector<GymRecord> loadRecords(const std::string& filePath);
