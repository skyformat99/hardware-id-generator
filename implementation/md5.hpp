// http://www.zedwood.com/article/cpp-md5-function
#pragma once

#include <cstdint>
#include <string>

namespace system_info
{

static const int blocksize = 64;

/**
 * @brief Класс для получения MD5 хеша
 */
class MD5
{
public:
   MD5();
   MD5( const std::string& text );
   void Update( const unsigned char *buf, std::uint32_t length );
   void Update( const char *buf, std::uint32_t length );
   MD5& Finalize();
   std::string HexDigest() const;
private:
   void Init();
   void Transform( const std::uint8_t block[ blocksize ] );
   static void Decode( std::uint32_t output[], const std::uint8_t input[], std::uint32_t len );
   static void Encode( std::uint8_t output[], const std::uint32_t input[], std::uint32_t len );

   ////////////////// НИЗКОУРОВНЕВЫЕ ОПЕРАЦИИ //////////////////
   static inline std::uint32_t F( std::uint32_t x, std::uint32_t y, std::uint32_t z );
   static inline std::uint32_t G( std::uint32_t x, std::uint32_t y, std::uint32_t z );
   static inline std::uint32_t H( std::uint32_t x, std::uint32_t y, std::uint32_t z );
   static inline std::uint32_t I( std::uint32_t x, std::uint32_t y, std::uint32_t z );
   static inline std::uint32_t RotateLeft( std::uint32_t x, int n );
   static inline void FF( std::uint32_t& a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac );
   static inline void GG( std::uint32_t& a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac );
   static inline void HH( std::uint32_t& a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac );
   static inline void II( std::uint32_t& a, std::uint32_t b, std::uint32_t c, std::uint32_t d, std::uint32_t x, std::uint32_t s, std::uint32_t ac );
private:
   bool finalized = true;
   std::uint8_t buffer[ blocksize ];
   std::uint32_t count[ 2 ];
   std::uint32_t state[ 4 ];
   std::uint8_t digest[ 16 ];
};

} // end namespace system_info
