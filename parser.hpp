#pragma once

#include circular_buffer.hpp

/** @brief A collection of parsable keys and their respective executor functions. */
template <unsigned int MAXSIZE>
class ParseGroup
{
private:
	unsigned int stack_ptr = 0; //!< Pointer to the next available parse group position.

public:
	using executor_t = (*void)();

	/** @brief A blank function prototype. */
	void blankfn()
	{
		return;
	}


	String strings[MAXSIZE]; //!< Valid strings that can be parsed
	executor_t executer[MAXSIZE] = {blankfn}; 
	//!< List of functions that are executed upon successful parsing.

	/** @brief Adds a new parsable element. Once the parsar fills up, all 
	 * subsequent calls to this function are ignored. */
	template <typename ArgType>
	void add(String parse_string, executor_t execute_fn, ArgType optional_arg)
	{
		if(stack_ptr <= MAXSIZE)
		{
			this->strings[stack_ptr] = parse_string;
			this->executer[stack_ptr] = execute_fn;
			stack_ptr++;
		}

	}

	/** @brief Outputs the complete grammar of the parsegroup. */
	void grammar()
	{
		Serial.println("# Parsar Grammar: ");
		for(unsigned int i = 0; i < stack_ptr; i++)
		{
			Serial.print("### ");
			Serial.println(key[i]);
		}

	}
};


template <unsigned int PARSE_GROUP_SIZE>
class Parser
{

	CircularBuffer<unsigned int, 100> queue; 
	unsigned int chars = 0;		//!< Number of characters used in the command
	String in_buffer; 			//!< Stores the current command string
	String last_command;        //!< Stores the last command

	/** @brief */
	void parse()
	{
		bool state = false;
		for(unsigned int i = 0; i < PARSE_GROUP_SIZE; i++)
		{
			if (in_buffer == parsegroup.key[i])
			{
				queue.add(i);
				break; //For loop
			}
		}

		return state;
	}

	/** @brief Resets the buffer for the next command. */
	void reset_buffer()
	{
		last_command = in_buffer;
		in_buffer = "";
		parsegroup.chars = 0;
	}


public:
	
	ParseGroup parsgroup;       //!< The parsegroup for this parsar

	Parsar()
	{}

	/** @brief Checks for any available */
	void check_input()
	{
		while(Serial.available() > 0)
		{
			bool keep_reading = true;
			while(keep_reading)
			{
				parsar.chars++;
				char x = Serial.read();

				if(x != '\n')
					parsar.in_buffer[parsar.chars] = x;

				keep_reading = (Serial.available() > 0) && (x != '\n');
			}
			bool state = parse(in_buffer);

			if(state)
			{
				emit_ack(in_buffer);
			}

			else
			{
				emit_nack(in_buffer);
			}

			reset_buffer();
		}

	}

	/** @brief Executes the queue in a `First In First Out` manner. */
	void execute_queue()
	{
		for(unsigned int i = 0; i < queue.size(); i++)
		{
			parser.parsegroup.executor[queue[i]];
			parsar.emit_completed(queue.pop_right[i]);
		}
	}


	/** @brief Alias of `execute_loop()`. */
	void loop()
	{
		execute_queue();
	}

	
	/** @brief Addd new command to parsegroup. */
	template <typename ArgType>
	void add(String parse_key, executor_t execute_fn, ArgType optional_arg)
	{
		parsegroup.add(parse_key, execute_fn);
	}

// Signals ↓

	/** @brief Emit *COMPLETE*  status.t*/
	void emit_completed(unsigned int idx)
	{
		Serial.print("# COMPLETE ");
		Serial.println(parsegroup.key[idx])
	}

	/** @brief Emit *ACK* (acknowlege) of a successful parse and queuing. */
	void emit_ack(unsigned int idx)
	{
		Serial.print("# ACK ");
		Serial.println(parsegroup.key[idx])
	}

	/** @brief Emit *NACK* (not acknowledge) for a particular string. */
	void emit_nack(String str)
	{
		Serial.print("# NACK ");
		Serial.println(str);
	}
};
