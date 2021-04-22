#include "minilock.h"
#include "exception.h"
#include "logging.h"
#include "base58.h"

//#include <libbase58.h>
//#include <libscrypt.h>
#include <blake2.h>
#include <sodium.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>


// ToDo: remove
#include <experimental/filesystem>
//#include "utils.h"

namespace minilockcpp
{

minilock::~minilock()
{
    log::verbose() << "minilock session closed." << std::endl;
}

minilock::minilock()
{
    set_log_level(this->level);

    log::debug() << "minilock session created." << std::endl;

    if (sodium_init() < 0)
    {
        throw general_error() << "unable to initialize sodium library!";
    }
}

minilock::minilock(const std::string& email, const std::string& password) : email(email)
{
    set_log_level(this->level);

    log::debug() << "minilock session created." << std::endl;

    if (sodium_init() < 0)
    {
        throw general_error() << "unable to initialize sodium library!";
    }
    if(!initialize(email, password))
    {
        throw initialize_error() << "unable to initialize minilock!";
    }
}

void minilock::set_log_level(log::log_level level)
{
    log::set_log_level(level);
}

void minilock::check_initialization()
{
    if (!initialized)
    {
        log::error() << "tried to call a function while minilock is uninitialized!" << std::endl;
        throw initialize_error() << "Initialization Error: Minilock did not initialize correctly. Abort.";
    }
}

bool minilock::initialize(const std::string& email, const std::string& password)
{
    if(initialized) return true;

    this->email = email;
    std::array<uint8_t, blake2s_constant::BLAKE2S_OUTBYTES> password_hash;
    std::array<uint8_t, minilock_constant::uid_checksum_length> id_hash;
    std::array<uint8_t, minilock_constant::uid_checksum_length + minilock_constant::publickey_bytes> id;
    std::array<char, 256> b58_id_buffer;
    size_t b58id_len = b58_id_buffer.size();

    if(blake2s(password_hash.data(), password.c_str(), nullptr, blake2s_constant::BLAKE2S_OUTBYTES, password.size(), 0))
    {
        throw general_error() << "unable to create blake2s hash";
    }

    if(0 != crypto_pwhash_scryptsalsa208sha256_ll(password_hash.data(),
        blake2s_constant::BLAKE2S_OUTBYTES,
        reinterpret_cast<const uint8_t*>(email.c_str()),
        email.size(),
        scrypt_constant::scrypt_cpu_ram_cost,
        scrypt_constant::scrypt_ram_cost,
        scrypt_parallelisation,
        secretkey.data(),
        crypto_box_secretkeybytes()))
    {
        throw general_error() << "unable to generate secret key";
    }

    //if(libscrypt_scrypt(password_hash.data(),
    //                    blake2s_constant::BLAKE2S_OUTBYTES,
    //                    reinterpret_cast<const uint8_t*>(email.c_str()),
    //                    email.size(),
    //                    scrypt_constant::scrypt_cpu_ram_cost,
    //                    scrypt_constant::scrypt_ram_cost,
    //                    scrypt_constant::scrypt_parallelisation,
    //                    secretkey.data(),
    //                    crypto_box_secretkeybytes()))
    //{
    //    throw general_error() << "unable to generate secret key";
    //}

    if(crypto_scalarmult_curve25519_base(publickey.data(), secretkey.data()))
    {
        throw general_error() << "unable to generate public key";
    }

    if(blake2s(id_hash.data(), publickey.data(), nullptr, minilock_constant::uid_checksum_length, minilock_constant::publickey_bytes, 0))
    {
        throw general_error() << "unable to create blake2s hash";
    }

    std::copy(publickey.begin(), publickey.end(), id.begin());
    std::copy(id_hash.begin(), id_hash.end(), id.data() + minilock_constant::publickey_bytes);

    //this->id = base58::base58_encode(id.data(), id.data() + id.size());
    std::vector<uint8_t> vID(id.begin(), id.end());
    base58::base58_encode(vID, this->id);

    initialized = true;

    log::verbose() << "Session initialized!" << std::endl;
    return true;
}

bool minilock::encrypt_file(const std::string& source_filename,
                            bool random_filename,
                            const std::string& source_directory,
                            const std::string& target_directory,
                            const std::vector<std::string>& recipients,
                            bool is_own_recipient)
{
    log::verbose() << "Starting file encryption of " << source_filename << std::endl;

    check_initialization();

    encrypt(source_filename, id, secretkey.data(), random_filename, source_directory, target_directory, recipients, is_own_recipient);

    log::verbose() << "File encryption finished..." << std::endl;

    return true;
}


bool minilock::decrypt_file(const std::string& source_filename,
                            const std::string& source_directory,
                            const std::string& target_directory)

{
    log::verbose() << "Starting file decryption of " << source_filename << std::endl;

    check_initialization();

    decrypt(source_filename, secretkey.data(), source_directory, target_directory);

    log::verbose() << "File decryption finished..." << std::endl;

    return true;
}


}
