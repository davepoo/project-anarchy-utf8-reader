#ifndef _CBUFFEREDREADERUTF8H_	
#define _CBUFFEREDREADERUTF8H_

/** used to read UTF-8 text from a filestream */
class cBufferedReaderUTF8 {
public:
	cBufferedReaderUTF8();
	cBufferedReaderUTF8( IVFileInStream* pInputStream );
	~cBufferedReaderUTF8();

	/** Reads a line of text. A line is considered to be terminated by a carriage return (\r) followed immediately by a linefeed (\n). 
	* @return A String containing the contents of the line, not including any line-termination characters, or NULL if the end of the stream has been reached */
	VString readLine();
protected:
private:
	/** possible line ternimators - new line */
	const static char TERMINATOR_NL;
	/** possible line ternimators - Carriage return */
	const static char TERMINATOR_CR;
	/** the size of the buffer we are using */
	const static unsigned int BUFFER_SIZE_MAX = 256;

	/** the position we have read to in the buffer */
	unsigned int m_bufferPos;
	/** The buffer we are reading the stream into */
	char* m_buffer;
	/** the number of chars currently in the buffer to be read */
	unsigned int m_bufferSize;
	/** The stream that is being read by this reader */
	IVFileInStream* m_pInputStream;

	/** @return true if the buffer is empty, false if there is data to be read */
	bool isBufferEmpty();
	/** fill the buffer with as many bytes as possible, wiping out all current buffer data and reset the buffer pointer */
	void fillBuffer();

	/** reads a UTF-8 encoded char from the buffer at the current position and decodes it */
	wchar_t readUTF8Char();
	/** This should always be the method to use to read the next byte from the stream
	* @return the next byte out of the buffer and update the position of the buffer pointer */
	char readByte();
};
#endif