/**
* @file     config.cpp
* @brief    Implementation of the config-class.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#define _CRT_SECURE_NO_WARNINGS

#include "config.h"
#include <cstdio>
#include <inttypes.h>

using namespace memory::app;

Config::Config(void) noexcept
{
    if (!this->load())
        this->set_defaults();
}

Config::~Config(void)
{
    this->save();
}

void Config::set_defaults(void) noexcept
{
    this->set_default_type();
    this->set_default_aligment();
    this->set_default_start_address();
    this->set_default_end_address();
    this->set_default_search_split_size();
    this->set_default_search_limit_size();
}

void Config::set_type(type_t type) noexcept
{
    this->_type = type;
    switch (type)
    {
    case MEMORY_TYPE_INT8:      this->_type_size = sizeof(int8_t);      break;
    case MEMORY_TYPE_UINT8:     this->_type_size = sizeof(uint8_t);     break;
    case MEMORY_TYPE_INT16:     this->_type_size = sizeof(int16_t);     break;
    case MEMORY_TYPE_UINT16:    this->_type_size = sizeof(uint16_t);    break;
    case MEMORY_TYPE_INT32:     this->_type_size = sizeof(int32_t);     break;
    case MEMORY_TYPE_UINT32:    this->_type_size = sizeof(uint32_t);    break;
    case MEMORY_TYPE_INT64:     this->_type_size = sizeof(int64_t);     break;
    case MEMORY_TYPE_UINT64:    this->_type_size = sizeof(uint64_t);    break;
    case MEMORY_TYPE_FLOAT:     this->_type_size = sizeof(float);       break;
    case MEMORY_TYPE_DOUBLE:    this->_type_size = sizeof(double);      break;
    case MEMORY_TYPE_STRING:    this->_type_size = 0;                   break;
    default:
        this->_type = MEMORY_TYPE_INT8;
        this->_type_size = sizeof(int8_t);
        break;
    }
}

bool Config::save(void) const
{
    FILE* file = fopen(CONFIG_PATH, "w");
    if (file == nullptr) return false;

    fprintf(file, "type: %" PRIu32 "\n", this->_type);
    fprintf(file, "alignment: %" PRIu16 "\n", this->_alignment);
    fprintf(file, "start address: 0x%" PRIX64 "\n", this->_start);
    fprintf(file, "end address: 0x%" PRIX64 "\n", this->_end);
    fprintf(file, "search split size: 0x%" PRIX64 "\n", this->_search_split_size);
    fprintf(file, "search limit size: 0x%" PRIX64 "\n", this->_search_limit_size);
    fclose(file);
    return true;
}

bool Config::load(void)
{
    FILE* file = fopen(CONFIG_PATH, "r");
    if (file == nullptr) return false;

    uint32_t t;
    uint16_t alignment;
    address_t start, end;
    size_t sss, sls;

    fscanf(file, "type: %" PRIu32 "\n", &t);
    fscanf(file, "alignment: %" PRIu16 "\n", &alignment);
    fscanf(file, "start address: 0x%" PRIX64 "\n", &start);
    fscanf(file, "end address: 0x%" PRIX64 "\n", &end);
    fscanf(file, "search split size: 0x%" PRIX64 "\n", &sss);
    fscanf(file, "search limit size: 0x%" PRIX64 "\n", &sls);
    fclose(file);

    this->set_type((type_t)t);
    this->set_aligment(alignment);
    this->set_start_address(start);
    this->set_end_address(end);
    this->set_search_split_size(sss);
    this->set_search_limit_size(sls);
    return true;
}
