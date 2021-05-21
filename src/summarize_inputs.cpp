#include <absl/container/flat_hash_map.h>
#include <absl/container/flat_hash_set.h>

#include <re2/re2.h>

#include <fmt/format.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <istream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

static constexpr size_t k_Sha1sumSize = 20;

using Checksum = std::array<uint8_t, k_Sha1sumSize>;

struct TranslationUnit
{
   Checksum fileChecksum{};

   absl::flat_hash_map<std::string, Checksum> dependencies;
};

enum class ParserState
{
   ExpectingTranslationUnit,
   InTranslationUnit,
   ExpectingDependency,
   InDependency,
};

absl::flat_hash_map<std::string, TranslationUnit> getArtifacts(std::istream& is, const re2::RE2* filter)
{
   absl::flat_hash_map<std::string, TranslationUnit> results;

   ParserState parserState{ParserState::ExpectingTranslationUnit};

   constexpr size_t k_BufferSize = 16384;

   std::vector<char> buffer;
   buffer.resize(k_BufferSize);

   std::string     currentPath;
   TranslationUnit currentUnit;

   // TODO(florin): replace with custom line extractor
   while (is)
   {
      is.getline(buffer.data(), k_BufferSize);

      const auto eosPos = std::find(buffer.begin(), buffer.end(), '\0');
      assert(eosPos != buffer.end());

      const auto lineLength = std::distance(buffer.begin(), eosPos);

      if (lineLength == 0) [[unlikely]]
      {
         if (!currentPath.empty())
         {
            // end of translation unit

            if ((filter != nullptr) && RE2::FullMatch(buffer.data(), *filter))
            {
               // skip
            }
            else
            {
               results.insert_or_assign(std::move(currentPath), std::move(currentUnit));
            }

            currentPath = std::string{};
            currentUnit = TranslationUnit{};
         }

         parserState = ParserState::ExpectingTranslationUnit;
         continue;
      }

      if (parserState == ParserState::ExpectingTranslationUnit) [[unlikely]]
      {
         if (buffer[0] == ' ')
         {
            throw std::logic_error{"Inconsistent state; expecting translation unit but found indent."};
         }

         currentPath = std::string{buffer.data()};

         parserState = ParserState::ExpectingDependency;

         continue;
      }

      if (parserState == ParserState::ExpectingDependency)
      {
         if (buffer[0] != ' ')
         {
            throw std::logic_error{"Inconsistent state; expecting dependency unit but found no indent."};
         }

         if ((filter != nullptr) && RE2::FullMatch(buffer.data(), *filter))
         {
            // skip
         }
         else
         {
            currentUnit.dependencies.emplace(buffer.data(), Checksum{});
         }
      }
   }

   return results;
}

absl::flat_hash_set<std::string>
findUniqueInputs(const absl::flat_hash_map<std::string, TranslationUnit>& translationUnits)
{
   absl::flat_hash_set<std::string> results;

   // TODO(florin): can this be expressed as an accumulate?
   std::for_each(translationUnits.begin(), translationUnits.end(), [&results](const auto& tu) {
      std::for_each(tu.second.dependencies.begin(), tu.second.dependencies.end(), [&results](const auto& dp) {
         results.insert(dp.first);
      });
   });

   return results;
}

int main(int argc, char* argv[])
{
   try
   {
      std::unique_ptr<re2::RE2> filter;

      if (argc > 1)
      {
         filter = std::make_unique<re2::RE2>(argv[1]);
      }

      const auto artifacts = getArtifacts(std::cin, filter.get());

      fmt::print("Found {} translation units\n", artifacts.size());
   }
   catch (const std::exception& ex)
   {
      fmt::print("Exception caught in main: {}\n", ex.what());
   }

   return 0;
}
