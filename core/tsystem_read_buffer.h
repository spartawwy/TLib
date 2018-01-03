// by Xixin Silas Cheng
// a partial read buffer 

#ifndef TSYSTEM_READ_BUFFER_H_
#define TSYSTEM_READ_BUFFER_H_

#include <vector>
#include <tuple>

namespace TSystem
{
namespace utility
{
	class ReadBuffer
	{
	public:
		explicit ReadBuffer( std::size_t buf_size, std::size_t header_len);
		~ReadBuffer();

		// valid buffer 
		char* data() { return &read_buf_[valid_ofset_]; }
		const char* data() const { return &read_buf_[valid_ofset_]; }

		// valid size
		std::size_t size() const { return valid_size_; }

		// parse one message
		// msg  :       the parsed message
		// trans_len:	total bytes in the buffer begin from valid_buffer_
		std::tuple<const char*, std::size_t> ParseOneMessage(std::size_t trans_len);

        std::size_t* p_ofset(){ return &valid_ofset_;}
        std::size_t ofset() { return valid_ofset_; }
        std::vector<char>& read_buf() { return read_buf_;}

	private:

		void RearrageBuffer(std::size_t ofset, std::size_t length); 

		const std::size_t  header_length_;
		
		// data holder
		std::vector<char>  read_buf_;

		// exposed buffer
		std::size_t        valid_ofset_;

		// exposed buffer size
		std::size_t        valid_size_;

		// current buffer position
		std::size_t        cur_pos_;

		// current message body length
		unsigned int       last_header_parsed_;
	};
}
}

#endif // !TSYSTEM_READ_BUFFER_H_
