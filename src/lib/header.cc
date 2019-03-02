#include "header.h"
#include "exception.h"
#include "logging.h"

#include <rapidjson/document.h>
#include <sodium.h>
#include <libbase58.h>
#include <blake2.h>

#include <sstream>

namespace minilockcpp
{

bool header::parse_header(const std::string &json_header,
                          const uint8_t* secretkey)
{
    rapidjson::Document dJsonHeader;
    if( dJsonHeader.Parse(json_header.c_str()).HasParseError())
    {
        std::string s = "JSON Parse Error: Unable to parse JSON Header.\n";
        log::error() << s << std::endl;
        log::debug() << "JSON Header: " << json_header << std::endl;
        throw json_parse_error() << s;
    }

    if(dJsonHeader["version"].GetInt() != 1)
    {
        std::string s = "Invalid minilock version: the file has been encrypted with a different version of minilock (v" + std::string(dJsonHeader["version"].GetString()) + ")";
        log::error() << s << std::endl;
        throw invalid_minilock_version() << s;
    }

    std::string ephemeralPublicKey = base64_decode(dJsonHeader["ephemeral"].GetString());
    const rapidjson::Value& decryptInfoJsonObject = dJsonHeader["decryptInfo"];
    const unsigned int decryptInfoCount = decryptInfoJsonObject.MemberCount();
    std::vector<std::string> b64UniqueNonce;
    std::vector<std::string> decryptObject;

    for (rapidjson::Value::ConstMemberIterator itr = decryptInfoJsonObject.MemberBegin(); itr != decryptInfoJsonObject.MemberEnd(); ++itr)
    {
        b64UniqueNonce.push_back(itr->name.GetString());
        decryptObject.push_back(itr->value.GetString());
    }

    std::string jsonDecryptInfo;
    std::string uniqueNonce;
    bool foundDecryptInfo = false;

    for(size_t i=0; i < decryptInfoCount; i++)
    {
        uniqueNonce = base64_decode(b64UniqueNonce[i]);
        std::string cipher = base64_decode(decryptObject[i]);
        std::vector<uint8_t> decrypted_jsonDecryptInfo(cipher.size() - minilock_constant::padding_bytes, 0);
        if(!crypto_box_open_easy(decrypted_jsonDecryptInfo.data(),
                                 reinterpret_cast<const uint8_t*>(cipher.c_str()), cipher.size(),
                                 reinterpret_cast<const uint8_t*>(uniqueNonce.c_str()),
                                 reinterpret_cast<const uint8_t*>(ephemeralPublicKey.c_str()),
                                 secretkey))
        {
            jsonDecryptInfo = std::string(decrypted_jsonDecryptInfo.begin(), decrypted_jsonDecryptInfo.end());
            foundDecryptInfo=true;
            break;
        }
    }

    if(jsonDecryptInfo.empty() || foundDecryptInfo == false)
    {
        std::string s = "Invalid Authentication! Could not find a corresponding recipient in JSON Header!";
        log::error() << s << std::endl;
        throw invalid_authentication() << s;
    }

    rapidjson::Document dJsonDecryptInfo;
    if( dJsonDecryptInfo.Parse(jsonDecryptInfo.c_str()).HasParseError())
    {
        std::string s = "Unable to parse JSON Header decryptInfo section.";
        log::error() << s << std::endl;
        log::debug() << "decryptInfo: " << jsonDecryptInfo << std::endl;
        throw json_parse_error() << s;
    }

    std::string senderID = dJsonDecryptInfo["senderID"].GetString();
    std::string encrypted_jsonFileInfo = base64_decode(dJsonDecryptInfo["fileInfo"].GetString());
    std::array<uint8_t, minilock_constant::publickey_bytes + minilock_constant::uid_checksum_length> decodedSenderPublic;
    size_t actPos = decodedSenderPublic.size();
    b58tobin(decodedSenderPublic.data(), &actPos, senderID.c_str(), senderID.size());
    std::copy(decodedSenderPublic.begin(), decodedSenderPublic.end(), senderID.begin());
    std::vector<uint8_t> decrypted_jsonFileInfo(encrypted_jsonFileInfo.size() - minilock_constant::padding_bytes);
    if(0 != crypto_box_open_easy(decrypted_jsonFileInfo.data(),
                                 reinterpret_cast<const uint8_t*>(encrypted_jsonFileInfo.c_str()), encrypted_jsonFileInfo.size(),
                                 reinterpret_cast<const uint8_t*>(uniqueNonce.c_str()),
                                 decodedSenderPublic.data(),
                                 secretkey))
    {
        log::error() << "unable to decrypt JSON fileInfo section." << std::endl;
        throw general_decryption_error() << "Decryption Error: unable to decrypt JSON fileInfo section.";
    }

    std::string jsonFileInfo(decrypted_jsonFileInfo.begin(), decrypted_jsonFileInfo.end());

    rapidjson::Document dJsonFileInfo;
    if( dJsonFileInfo.Parse(jsonFileInfo.c_str()).HasParseError())
    {
        std::string s = "Unable to parse JSON Header fileInfo section.";
        log::error() << s << std::endl;
        log::debug() << "fileInfo: " << jsonFileInfo << std::endl;
        throw json_parse_error() << s;
    }

    std::string strFileKey = base64_decode(dJsonFileInfo["fileKey"].GetString());
    std::string strFileNonce = base64_decode(dJsonFileInfo["fileNonce"].GetString());
    std::string strFileHash = base64_decode(dJsonFileInfo["fileHash"].GetString());

    std::vector<uint8_t> fh = std::vector<uint8_t>(strFileHash.begin(), strFileHash.end());
    std::vector<uint8_t> chunkNumber(minilock_constant::chunk_number_bytes, 0);

    std::copy(std::begin(strFileKey), std::end(strFileKey), std::begin(file_key));
    std::copy(std::begin(strFileNonce), std::end(strFileNonce), std::begin(file_nonce));
    std::copy(std::begin(file_nonce), std::end(file_nonce), full_nonce.data());
    std::copy(std::begin(chunkNumber), std::end(chunkNumber), full_nonce.data() + minilock_constant::filenonce_bytes);

    return true;
}

std::string header::generate_header(const std::string& sender_id,
                                    const uint8_t* filehash,
                                    const uint8_t* secretkey,
                                    std::vector<std::string> recipients,
                                    bool is_own_recipient)
{
    std::array<uint8_t, minilock_constant::chunk_number_bytes> chunkNum;
    std::array<uint8_t, minilock_constant::secretkey_bytes> senderEphemeralSecret;
    std::array<uint8_t, minilock_constant::publickey_bytes> senderEphemeralPublic;

    crypto_box_keypair(senderEphemeralPublic.data(), senderEphemeralSecret.data());

    std::copy(file_nonce.begin(), file_nonce.end(), full_nonce.data());
    std::copy(chunkNum.begin(), chunkNum.end(), full_nonce.data() + minilock_constant::filenonce_bytes);

    std::string json_Object = "{\"version\":1,\"ephemeral\":\"" + base64_encode(senderEphemeralPublic.data(), minilock_constant::publickey_bytes) + "\",";
    std::string json_decryptInfo = "\"decryptInfo\":{";
    std::string json_fileInfo = "\"fileInfo\":";
    std::string base64_decryptInfo;

    if(is_own_recipient)
        recipients.insert(recipients.begin(), sender_id);

    for(size_t i = 0; i < recipients.size(); i++)
    {
        std::string strJsonFileInfo = "{\"fileKey\":\"" + base64_encode(file_key.data(), minilock_constant::filekey_bytes)
                                    + "\",\"fileNonce\":\"" + base64_encode(file_nonce.data(), minilock_constant::filenonce_bytes)
                                    + "\",\"fileHash\":\"" + base64_encode(filehash, blake2s_constant::BLAKE2S_OUTBYTES)
                                    + "\"}";

        std::vector<uint8_t> jsonFileInfo(strJsonFileInfo.size());
        std::copy(std::begin(strJsonFileInfo), std::end(strJsonFileInfo), jsonFileInfo.data());
        std::vector<uint8_t> encrypted_jsonFileInfo(jsonFileInfo.size() + minilock_constant::padding_bytes);

        std::array<uint8_t, minilock_constant::fullnonce_bytes> uniqueNonce;
        randombytes_buf(uniqueNonce.data(), uniqueNonce.size());

        std::array<uint8_t, minilock_constant::publickey_bytes> recipientPublic;
        std::vector<uint8_t> decodedRecipientPublic(minilock_constant::publickey_bytes + minilock_constant::uid_checksum_length);
        size_t actPos = decodedRecipientPublic.size();
        b58tobin(decodedRecipientPublic.data(), &actPos, recipients[i].c_str(), recipients[i].length());
        std::copy(decodedRecipientPublic.begin(), decodedRecipientPublic.end(), recipientPublic.begin());

        if( 0 != crypto_box_easy(encrypted_jsonFileInfo.data(), jsonFileInfo.data(), jsonFileInfo.size(), uniqueNonce.data(), recipientPublic.data(), secretkey))
        {
            log::error() << "unable to encrypt JSON fileInfo section." << std::endl;
            throw general_encryption_error() << "Encryption Error: unable to encrypt JSON fileInfo section.";
        }

        std::string base64_uniqueNonce = base64_encode(uniqueNonce.data(), uniqueNonce.size());
        std::string base64_fileInfo = base64_encode(encrypted_jsonFileInfo.data(), static_cast<unsigned int>(encrypted_jsonFileInfo.size()));
        std::string strJsonDecryptInfo_N = "{\"senderID\":\"" + sender_id + "\",\"recipientID\":\"" + recipients[i] + "\"," + json_fileInfo + "\"" + base64_fileInfo + "\"}";

        std::vector<uint8_t> jsonDecryptInfo_N(strJsonDecryptInfo_N.size());
        std::copy(std::begin(strJsonDecryptInfo_N), std::end(strJsonDecryptInfo_N), jsonDecryptInfo_N.data());
        std::vector<uint8_t> encrypted_jsonDecryptInfo_N(jsonDecryptInfo_N.size() + minilock_constant::padding_bytes);

        if( 0 != crypto_box_easy(encrypted_jsonDecryptInfo_N.data(), jsonDecryptInfo_N.data(), jsonDecryptInfo_N.size(), uniqueNonce.data(), recipientPublic.data(), senderEphemeralSecret.data()))
        {
            log::error() << "unable to encrypt JSON decryptInfo section." << std::endl;
            throw general_encryption_error() << "Encryption Error: unable to encrypt JSON decryptInfo section.";
        }

        std::string base64_decryptInfo_N = base64_encode(encrypted_jsonDecryptInfo_N.data(), static_cast<unsigned int>(encrypted_jsonDecryptInfo_N.size()));
        base64_decryptInfo = base64_decryptInfo + "\"" + base64_uniqueNonce + "\":\"" + base64_decryptInfo_N + "\",";
    }

    base64_decryptInfo = base64_decryptInfo.substr(0, base64_decryptInfo.size() - 1) + "}}"; // remove last "," and add "}}"
    json_Object = json_Object + json_decryptInfo + base64_decryptInfo;

    return json_Object;
}

}
