#include "BufferedReaderUTF8.h"

/** possible line ternimators - new line */
const char cBufferedReaderUTF8::TERMINATOR_NL = '\n';
/** possible line ternimators - Carriage return */
const char cBufferedReaderUTF8::TERMINATOR_CR = '\r';

cBufferedReaderUTF8::cBufferedReaderUTF8() {
	VASSERT_MSG( true, "constructor not valid" );
}

cBufferedReaderUTF8::cBufferedReaderUTF8( IVFileInStream* pInputStream ) {
	VASSERT_MSG( pInputStream != NULL, "The input stream cannot be null" );

	m_pInputStream = pInputStream;
	m_buffer = new char[BUFFER_SIZE_MAX];
	m_bufferPos = 0;
	m_bufferSize = 0;
	fillBuffer();
}

cBufferedReaderUTF8::~cBufferedReaderUTF8() {
	if ( m_buffer != NULL ) {
		delete [] m_buffer;			
		m_buffer = NULL;
	}
}

VString cBufferedReaderUTF8::readLine() {
	VASSERT_MSG( m_buffer != NULL, "The buffer cannot be null" );
	VASSERT_MSG( m_pInputStream != NULL, "The input stream cannot be null" );
	
	//Get the line    
  const unsigned int WSTR_SIZE = 40;
  wchar_t wstr[WSTR_SIZE];          //buffer for our wide characters
  unsigned int wstr_pos = 0;        //wide character buffer position
	VString line = VString("",200);   //our finished line
	bool eof_eol = false;
	while ( eof_eol == false ) {		
		wchar_t c = readUTF8Char();
		
		if ( c == TERMINATOR_NL ) {
			eof_eol = true;	//end of line
		} else if ( c == TERMINATOR_CR || c == 0 ) {
			//Skip over
		} else {
			//append the current wide char
			//convert to wchar_t*
			wstr[wstr_pos] = c;
      wstr_pos++;
		}
        
		//check for end of file
		if ( isBufferEmpty() && m_pInputStream->IsEOF() ) {
			eof_eol = true;
		}
        
    //append wide line buffer and start again if we reached the end of the line
    //or if we ran out of space in our wide line buffer
    if ( wstr_pos == WSTR_SIZE-2 || eof_eol == true ) {
      wstr[wstr_pos] = 0;       //zero terminate the wide string buffer
      line += wstr;             //append wide string buffer to our VString			
      wstr_pos = 0;             //start again
    }
    
		VASSERT_MSG( line.GetLen() < 10000, "The size of the line read by the UTF8 reader has gone over 10000 characters" )	
	}

	return line;
}

wchar_t cBufferedReaderUTF8::readUTF8Char() {
	wchar_t output;
	char c = readByte();
	if ( (c & 0x80) == 0 ) {
		//single byte char
		output = c;	
	} else if ( (c & 0xE0) == 0xE0 ) {
		//triple byte char
		output = ( ((c & 0xF) << 12) | ((readByte() & 0x3F) << 6) | (readByte() & 0x3F) );	
	} else if ( (c & 0xC0) == 0xC0 ) {
		//double byte char
		output = ((c & 0x1F) << 6) | (readByte() & 0x3F);	
	} else {
		VASSERT_ALWAYS_MSG( true, "UTF-8 chars with more than 3 bytes are not supported" );
	}
	return output;
}

char cBufferedReaderUTF8::readByte() {
	if ( isBufferEmpty() ) {
		fillBuffer();
	}
	char output = m_buffer[m_bufferPos];
	m_bufferPos++;
	return output;
}

/** @return true if the buffer is empty, false if there is data to be read */
bool cBufferedReaderUTF8::isBufferEmpty() {
	bool empty = false;
	if ( m_bufferPos >= m_bufferSize ) {
		empty = true;
	}
	return empty;
}

/** fill the buffer with as many bytes as possible, wiping out all current buffer data and reset the buffer pointer */
void cBufferedReaderUTF8::fillBuffer() {
	VASSERT_MSG( m_pInputStream != NULL, "The input stream cannot be null" );

	//Fill the buffer	
	size_t bytesread = m_pInputStream->Read(m_buffer, BUFFER_SIZE_MAX);
	m_bufferPos = 0;
	m_bufferSize = bytesread;
}
