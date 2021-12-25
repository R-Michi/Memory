/**
* @file     command.h
* @brief    Definitions of the command-decoder-engine.
* @author   Michael Reim / Github: R-Michi
* Copyright (c) 2021 by Michael Reim
*
* This code is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

#pragma once

#include <vector>
#include <string>

namespace memory
{
    class CmdStringList
    {
    private:
        std::vector<std::string> strings;

    public:
        /**
        * @brief Initialization constructor with optional parameter,
        *   which also acts as a default constructor
        */
        inline explicit CmdStringList(const std::vector<std::string>& strings = {}) { this->strings = strings; }

        /** @brief copy constructor */
        inline CmdStringList(const CmdStringList& other) { this->copy(other); }

        /** @brief move constructor, NOTE: the other object that is moved gets cleared.*/
        inline CmdStringList(CmdStringList&& other) { this->move(other); }

        /** @brief destructor */
        virtual ~CmdStringList(void) { /* dtor */ }

        /**
        * @brief Pushes an element to the end of the list.
        * @param[in] arg: element to push
        */
        inline void push_back(const std::string& str) { this->strings.push_back(str); }

        /** @brief Deletes the last element of the list. */
        inline void pop_back(void) noexcept { this->strings.pop_back(); }

        /** @brief Initalizes the list. */
        inline void set(const std::vector<std::string>& strings) { this->strings = strings; }

        /** @brief Cleares the whole list. */
        inline void clear(void) noexcept { this->strings.clear(); }

        /**
        * @brief Get one random element of the list WITHOUT range check.
        * @param[in] pos: postion of the element
        * @return Reference to the element at the position @param[in] pos.
        */
        inline std::string& at(uint32_t pos) noexcept { return this->strings[pos]; }

        /**
        * @brief Get one random element of the list WITHOUT range check.
        * @param[in] pos: postion of the element
        * @return Constant reference to the element at the position @param[in] pos.
        */
        inline const std::string& at(uint32_t pos) const noexcept { return this->strings[pos]; }

        /**
        * @brief Get one random element of the list WITH range check.
        * @param[in] pos: postion of the element
        * @return Reference to the element at the position @param[in] pos.
        * @throw std::out_of_range
        */
        inline std::string& at_secure(uint32_t pos) { return this->strings.at(pos); }

        /**
        * @brief Get one random element of the list WITH range check.
        * @param[in] pos: postion of the element
        * @return Constant reference to the element at the position @param[in] pos.
        * @throw std::out_of_range
        */
        inline const std::string& at_secure(uint32_t pos) const { return this->strings.at(pos); }

        /* @return Number of element within the list. */
        inline uint32_t size(void) const noexcept { return this->strings.size(); }

        /** @brief Copies an object. */
        CmdStringList& copy(const CmdStringList& other);

        /** @brief Moves an object. NOTE: the other object that is moved gets cleared. */
        CmdStringList& move(CmdStringList& other);

        /** @brief Copies an object. */
        inline CmdStringList& operator= (const CmdStringList& other) { return this->copy(other); }

        /** @brief Moves an object. NOTE: the other object that is moved gets cleared. */
        inline CmdStringList& operator= (CmdStringList&& other) { return this->move(other); }

        /** @return 'true' if both objects are equal, 'false' if not. */
        inline bool operator== (const CmdStringList& other) const noexcept { return (this->strings == other.strings); }

        /**
        * @brief Get one random element of the list WITHOUT range check.
        * @param[in] pos: postion of the element
        * @return Reference to the element at the position @param[in] pos.
        */
        inline std::string& operator[] (uint32_t pos) noexcept { return this->strings[pos]; }

        /**
        * @brief Get one random element of the list WITHOUT range check.
        * @param[in] pos: postion of the element
        * @return Constant reference to the element at the position @param[in] pos.
        */
        inline const std::string& operator[] (uint32_t pos) const noexcept { return this->strings[pos]; }
    };

    class CmdArgumentList : public CmdStringList
    {
    public:
        inline explicit CmdArgumentList(const std::vector<std::string>& args = {}) : CmdStringList(args) {}
        inline CmdArgumentList(const CmdArgumentList& other)                { this->copy(other); }
        inline CmdArgumentList(CmdArgumentList&& other)                     { this->move(other); }
        virtual ~CmdArgumentList(void)                                      { /* dtor */ }
        inline CmdArgumentList& operator= (const CmdArgumentList& other)    { this->copy(other); return *this; }
        inline CmdArgumentList& operator= (CmdArgumentList&& other)         { this->move(other); return *this; }
    };

    class CmdOpionList : public CmdStringList
    {
    public:
        constexpr static uint32_t NPOS = 0xFFFFFFFF;

        inline explicit CmdOpionList(const std::vector<std::string>& options = {}) : CmdStringList(options) {}
        inline CmdOpionList(const CmdOpionList& other)              { this->copy(other); }
        inline CmdOpionList(CmdOpionList&& other)                   { this->move(other); }
        virtual ~CmdOpionList(void)                                 { /* dtor */ }
        inline CmdOpionList& operator= (const CmdOpionList& other)  { this->copy(other); return *this; }
        inline CmdOpionList& operator= (CmdOpionList&& other)       { this->move(other); return *this; }

        /**
        * @brief Finds one option in the list.
        * @param[in] op: option to find
        * @param[in] start: start index
        * @return Index of the found option.
        */
        uint32_t find(const std::string& op, uint32_t start) const noexcept;

        /**
        * @brief Finds any option in the list.
        * @param[in] ops: option candidates to search for any
        * @param[in] start: start index
        * @return Index of the option that was found.
        */
        uint32_t find_any(const std::vector<std::string>& ops, uint32_t start) const noexcept;

        /**
        * @brief Finds all of the options.
        * @param[in] ops: option candidates
        * @param[in] start: start index
        * @param[out] indices: indices of all found options
        * @return 'true' if all options were found and false if not.
        */
        bool find_all(const std::vector<std::string>& ops, uint32_t start, std::vector<uint32_t>* indices = nullptr) const noexcept;

        /**
        * @brief Finds all options that are not the candidates.
        * @param[in] ops: option candidates that should be excluded
        * @param[in] start: start index
        * @param[out] indices: indices of all found options
        * @return 'true' if any option was found and 'false' if no option was found.
        */
        bool find_not(const std::vector<std::string>& ops, uint32_t start, std::vector<uint32_t>* indices = nullptr) const noexcept;
    };

    class Command
    {
    private:
        std::string _name;
        CmdArgumentList _args;
        CmdOpionList _options;

    public:
        Command(void) = default;
        virtual ~Command(void) = default;

        /** @param[in] str: command string to decode */
        inline explicit Command(const std::string& str) noexcept { this->phrase(str); }

        /** @param[in] cmd: command to copy */
        inline Command(const Command& cmd) noexcept { this->copy(cmd); }

        /** @param[in] cmd: command to move */
        inline Command(Command&& cmd) noexcept { this->move(cmd); }

        /**
        * @brief Phrases a command and splits it into arguments,
        *   whereby the argument with index 0 is the name of the command.
        *   Similar to argc and argv.
        * @param[in] str: command string
        */
        void phrase(const std::string& str) noexcept;

        /** @return name of the command */
        inline const std::string& name(void) const noexcept { return this->_name; }

        /** @return arguments of the command */
        inline const CmdArgumentList& args(void) const noexcept { return this->_args; }

        /** @return options of the command */
        inline const CmdOpionList& options(void) const noexcept { return this->_options; }

        /**
        * @brief copies a command
        * @param[in] cmd: command to copy
        * @return reference to this command
        */
        Command& copy(const Command& cmd) noexcept;

        /**
        * @brief moves a command
        * @param[in] cmd: command to move
        * @return reference to this command
        */
        Command& move(Command& cmd) noexcept;

        /**
        * @brief copies a command
        * @param[in] cmd: command to copy
        * @return reference to this command
        */
        inline Command& operator= (const Command& cmd) noexcept { return this->copy(cmd); }

        /**
        * @brief moves a command
        * @param[in] cmd: command to move
        * @return reference to this command
        */
        inline Command& operator= (Command&& cmd) noexcept { return this->move(cmd); }

    };

    class CommandEngineBase
    {
    public:
        CommandEngineBase(void) = default;
        virtual ~CommandEngineBase(void) = default;

        /**
        * @brief Ready the command string from the keyboard.
        * @param[out] cmd: decoded command
        */
        void get_command(Command& cmd);

        /**
        * @brief Invokes the command engine.
        * @param[in] cmd: command
        */
        virtual bool on_command(const Command& cmd) = 0;
    };
}
