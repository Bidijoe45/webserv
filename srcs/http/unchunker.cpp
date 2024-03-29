#include <string>
#include <iostream>
#include <cstdlib>

#include "unchunker.hpp"
#include "string_utils.hpp"

namespace ws
{
	Unchunker::Unchunker() : stage_(CHUNK_SIZE), pos_(0), chunk_size_(0), valid_(true), status_(OK) {}

	void Unchunker::parse_size()
	{
		size_t found = this->raw_body_buff_.find("\r\n", pos_);
		if (found == std::string::npos)
		{
			if (this->raw_body_buff_.size() > 0)
				this->pos_ = this->raw_body_buff_.size() - 1;
			this->status_ = INCOMPLETE;
			return;
		}
		this->pos_ = 0;

		std::string line = this->raw_body_buff_.get_next_line();
		if (line.size() == 0)
			throw std::runtime_error("Unchunker: empty size field");

		size_t end_of_size_pos = line.find_first_of(" ;");
		std::string hex_num = line.substr(0, end_of_size_pos);
		if (is_string_hex(hex_num, hex_num.size()) == false)
			throw std::runtime_error("Unchunker: invalid size chars");

		this->chunk_size_ = strtoul(hex_num.c_str(), NULL, 16);

		if (this->chunk_size_ == 0)
			this->stage_ = TRAILER_SECTION;
		else
			this->stage_ = CHUNK_DATA;
	}

	void Unchunker::parse_chunk_data()
	{
		if (this->raw_body_buff_.size() < (this->chunk_size_ + 2)) // +2 because we need to check for \r\n
		{
			this->status_ = INCOMPLETE;
			return;
		}
		this->chunk_data_ = this->raw_body_buff_.flush(this->chunk_size_);

		std::string crlf = this->raw_body_buff_.flush(2);
		if (crlf != "\r\n")
			throw std::runtime_error("Unchunker: chunk data must end with CRLF");
		
		this->unchunked_body_ += this->chunk_data_;
		this->chunk_data_.clear();
		this->stage_ = CHUNK_SIZE;
	}

	void Unchunker::parse_trailer_section()
	{
		size_t found = this->raw_body_buff_.find("\r\n", pos_);
		if (found == std::string::npos)
		{
			this->pos_ = this->raw_body_buff_.size() - 1;
			this->status_ = INCOMPLETE;
			return;
		}
		this->pos_ = 0;

		std::string line = this->raw_body_buff_.get_next_line();
		if (line.size() == 0)
			this->stage_ = COMPLETED;
	}

	void Unchunker::unchunk(const std::string &raw_body) // ignoring unrecognised chunk extensions and trailer headers
	{
		this->raw_body_buff_.append(raw_body);
		this->status_ = OK;
		try
		{
			while (this->status_ == OK && this->stage_ != COMPLETED)
			{
				switch (this->stage_)
				{
					case CHUNK_SIZE:
						this->parse_size();
						break;
					case CHUNK_DATA:
						this->parse_chunk_data();
						break;
					case TRAILER_SECTION:
						this->parse_trailer_section();
						break;
					default:
						break;
				}
			}
		}
		catch (const std::runtime_error &e)
		{
			this->stage_ = COMPLETED;
			this->valid_ = false;
			std::cout << e.what() << std::endl;
		}
	}

	Unchunker::Stage Unchunker::get_stage() const
	{
		return this->stage_;
	}

	std::string Unchunker::get_unchunked_body() const
	{
		return this->unchunked_body_;
	}

	bool Unchunker::is_valid() const
	{
		return this->valid_;
	}

	void Unchunker::reset()
	{
		this->stage_ = CHUNK_SIZE;
		this->raw_body_buff_.clear();
		this->unchunked_body_.clear();
		this->pos_ = 0;
		this->chunk_size_ = 0;
		this->chunk_data_.clear();
		this->valid_ = true;
		this->status_ = OK;
	}
}
